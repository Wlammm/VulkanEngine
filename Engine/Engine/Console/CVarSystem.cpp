#include "EnginePch.h"
#include "Engine/Console/CVarSystem.h"
#include "Engine/Utils/String.hpp"

#include <charconv>
#include <vector>

#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"

namespace
{
    // Strips the surrounding double quotes that metadata string arguments keep, e.g. "give" -> give.
    std::string StripQuotes(const std::string& inValue)
    {
        if (inValue.size() >= 2 && inValue.front() == '"' && inValue.back() == '"')
            return inValue.substr(1, inValue.size() - 2);
        return inValue;
    }

    // Reduces a reflected type name to a canonical form we can switch on, dropping const/ref/ptr
    // decorations and whitespace. "const std::basic_string<char> &" -> "std::basic_string<char>".
    std::string NormalizeTypeName(const std::string& inTypeName)
    {
        std::string result;
        result.reserve(inTypeName.size());
        for (char c : inTypeName)
        {
            if (c != ' ' && c != '&' && c != '*')
                result += c;
        }
        if (result.starts_with("const"))
            result = result.substr(5); // length of "const"
        return result;
    }

    // True if the (normalized) type name refers to a std::string. clang may spell it as the short
    // "std::basic_string<char>" or the fully expanded form with traits/allocator template args.
    bool IsStdString(const std::string& inNormalizedType)
    {
        return inNormalizedType == "std::basic_string<char>"
            || inNormalizedType.starts_with("std::basic_string<char,")
            || inNormalizedType.starts_with("std::basic_string<char>");
    }

    bool ParseBool(const std::string& inToken, bool& outValue)
    {
        const std::string lowered = String::ToLowerCopy(inToken);
        if (lowered == "true" || lowered == "1") { outValue = true; return true; }
        if (lowered == "false" || lowered == "0") { outValue = false; return true; }
        return false;
    }

    template <typename NumberType>
    bool ParseNumber(const std::string& inToken, NumberType& outValue)
    {
        const char* begin = inToken.data();
        const char* end = inToken.data() + inToken.size();
        const std::from_chars_result result = std::from_chars(begin, end, outValue);
        return result.ec == std::errc() && result.ptr == end;
    }
}

CVarSystem& CVarSystem::Get()
{
    return Engine::GetEngineSystem<CVarSystem>();
}

void CVarSystem::Init()
{
    RegisterReflectedCheats();
    RegisterBuiltInCommands();

    LOG("CVarSystem initialized with %d command(s). Type 'help' to list them.", myCommands.size());
}

void CVarSystem::RegisterReflectedCheats()
{
    for (const Type* type : ReflectionSystem::GetAllTypes())
    {
        for (const Method* method : type->GetMethodsWithMetadata("Cheat"))
        {
            ConsoleCommand command;
            command.myOwningType = type;
            command.myMethod = method;

            const List<std::string> nameArgs = method->GetMetadataArgs("Name");
            command.myName = nameArgs.IsEmpty() ? method->GetMethodName() : StripQuotes(nameArgs[0]);

            const List<std::string> helpArgs = method->GetMetadataArgs("Help");
            command.myHelp = helpArgs.IsEmpty() ? std::string() : StripQuotes(helpArgs[0]);

            if (FindCommand(command.myName))
            {
                LOG_WARNING("CVarSystem: duplicate cheat command '%s' ignored.", command.myName.c_str());
                continue;
            }

            myCommands.Add(command);
        }
    }
}

void CVarSystem::RegisterBuiltInCommands()
{
    RegisterCommand("help", "Lists all commands, or 'help <command>' for details on one.",
        [this](const List<std::string>& inArgs)
        {
            if (inArgs.IsEmpty())
            {
                Print("Available commands:");
                for (const ConsoleCommand& command : myCommands)
                    Print("  " + command.myName + (command.myHelp.empty() ? "" : "  - " + command.myHelp));
                return;
            }

            const ConsoleCommand* command = FindCommand(inArgs[0]);
            if (!command)
            {
                Print("Unknown command: " + inArgs[0]);
                return;
            }

            Print(command->myName + ": " + (command->myHelp.empty() ? "(no help)" : command->myHelp));
            if (command->myMethod)
                Print("  usage: " + DescribeSignature(*command));
        });

    RegisterCommand("commands", "Lists the names of all registered commands.",
        [this](const List<std::string>&)
        {
            for (const ConsoleCommand& command : myCommands)
                Print(command.myName);
        });
}

void CVarSystem::RegisterCommand(const std::string& inName, const std::string& inHelp,
    const Delegate<void(const List<std::string>&)>& inCallback)
{
    if (FindCommand(inName))
    {
        LOG_WARNING("CVarSystem: command '%s' is already registered.", inName.c_str());
        return;
    }

    ConsoleCommand command;
    command.myName = inName;
    command.myHelp = inHelp;
    command.myNativeCallback = inCallback;
    myCommands.Add(command);
}

const ConsoleCommand* CVarSystem::FindCommand(const std::string& inName) const
{
    for (const ConsoleCommand& command : myCommands)
    {
        if (String::EqualsIgnoreCase(command.myName, inName))
            return &command;
    }
    return nullptr;
}

void CVarSystem::Execute(const std::string& inCommandLine)
{
    // Split into name + arguments, ignoring empty tokens from repeated spaces.
    const List<std::string> tokens = String::SplitString(inCommandLine, " ");

    std::string name;
    List<std::string> args;
    for (const std::string& token : tokens)
    {
        if (token.empty())
            continue;

        if (name.empty())
            name = token;
        else
            args.Add(token);
    }

    if (name.empty())
        return;

    Print("> " + inCommandLine);

    const ConsoleCommand* command = FindCommand(name);
    if (!command)
    {
        Print("Unknown command: '" + name + "'. Type 'help' to list commands.");
        return;
    }

    if (command->myNativeCallback.IsValid())
    {
        command->myNativeCallback(args);
        return;
    }

    InvokeReflectedCommand(*command, args);
}

bool CVarSystem::InvokeReflectedCommand(const ConsoleCommand& inCommand, const List<std::string>& inArgs)
{
    const Method* method = inCommand.myMethod;
    const List<MethodArgument>& parameters = method->GetArguments();

    if (parameters.size() != inArgs.size())
    {
        Print("'" + inCommand.myName + "' expects " + std::to_string(parameters.size())
            + " argument(s) but got " + std::to_string(inArgs.size()) + ".");
        Print("  usage: " + DescribeSignature(inCommand));
        return false;
    }

    // Decode every string argument into typed storage. The storage must outlive the call, so it
    // lives in a buffer reserved up front (no reallocation) and the invoker receives pointers into it.
    struct ArgStorage
    {
        int32_t asInt = 0;
        uint32_t asUInt = 0;
        float asFloat = 0.f;
        double asDouble = 0.0;
        bool asBool = false;
        std::string asString;
    };

    std::vector<ArgStorage> storage(parameters.size());
    List<void*> rawArgs;

    for (int i = 0; i < parameters.size(); ++i)
    {
        ArgStorage& slot = storage[i];
        const std::string typeName = NormalizeTypeName(parameters[i].GetArgumentType()->GetName());
        const std::string& token = inArgs[i];

        bool parsed = false;
        void* pointer = nullptr;

        if (typeName == "int")
        {
            parsed = ParseNumber(token, slot.asInt);
            pointer = &slot.asInt;
        }
        else if (typeName == "unsignedint")
        {
            parsed = ParseNumber(token, slot.asUInt);
            pointer = &slot.asUInt;
        }
        else if (typeName == "float")
        {
            parsed = ParseNumber(token, slot.asFloat);
            pointer = &slot.asFloat;
        }
        else if (typeName == "double")
        {
            parsed = ParseNumber(token, slot.asDouble);
            pointer = &slot.asDouble;
        }
        else if (typeName == "bool")
        {
            parsed = ParseBool(token, slot.asBool);
            pointer = &slot.asBool;
        }
        else if (IsStdString(typeName))
        {
            slot.asString = token;
            parsed = true;
            pointer = &slot.asString;
        }
        else
        {
            Print("'" + inCommand.myName + "' has unsupported argument type '"
                + parameters[i].GetArgumentType()->GetName() + "'.");
            return false;
        }

        if (!parsed)
        {
            Print("Could not parse '" + token + "' as " + typeName + " for argument '"
                + parameters[i].GetArgumentName() + "'.");
            return false;
        }

        rawArgs.Add(pointer);
    }

    method->InvokeRaw(nullptr, rawArgs);
    return true;
}

std::string CVarSystem::DescribeSignature(const ConsoleCommand& inCommand) const
{
    std::string signature = inCommand.myName;
    if (!inCommand.myMethod)
        return signature;

    signature += " ";
    const List<MethodArgument>& parameters = inCommand.myMethod->GetArguments();
    for (int i = 0; i < parameters.size(); ++i)
    {
        if (i > 0)
            signature += " ";
        signature += "<" + parameters[i].GetArgumentName() + ":"
            + NormalizeTypeName(parameters[i].GetArgumentType()->GetName()) + ">";
    }
    return signature;
}

void CVarSystem::Print(const std::string& inLine)
{
    myOutputLog.Add(inLine);

    constexpr int maxLines = 512;
    while (myOutputLog.size() > maxLines)
        myOutputLog.RemoveIndex(0);

    LOG("%s", inLine.c_str());
}

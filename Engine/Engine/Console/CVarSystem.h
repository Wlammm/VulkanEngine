#pragma once
#include "Engine/System/System.h"

class Type;
class Method;

// A single console command. It is either backed by a reflected static method tagged with
// META(Cheat), or registered natively from code via CVarSystem::RegisterCommand.
struct ConsoleCommand
{
    std::string myName;
    std::string myHelp;

    // Reflected source. myMethod is the static method to invoke; null for native commands.
    const Type* myOwningType = nullptr;
    const Method* myMethod = nullptr;

    // Native source. Unbound for reflected commands.
    Delegate<void(const List<std::string>&)> myNativeCallback;
};

// Drives the console / cheat command system.
//
// On Init it scans the reflection system for every static method tagged with META(Cheat) and
// registers it as a command. Executing the command by name calls the function, marshalling the
// typed arguments from the string tokens of the command line.
//
//   class CheatCommands
//   {
//       META(Cheat, Help("Toggles god mode"))
//       static void GodMode(bool enabled);
//   };
//
//   Engine::GetEngineSystem<CVarSystem>().Execute("GodMode true");
class CVarSystem : public System
{
public:
    void Init() override;

    static CVarSystem& Get();

    // Parses and runs a command line, e.g. "GiveItem sword 3". Output is written to the log.
    void Execute(const std::string& inCommandLine);

    // Registers a native (non-reflected) command.
    void RegisterCommand(const std::string& inName, const std::string& inHelp,
        const Delegate<void(const List<std::string>&)>& inCallback);

    const ConsoleCommand* FindCommand(const std::string& inName) const;
    const List<ConsoleCommand>& GetCommands() const { return myCommands; }

    // Rolling buffer of console output, most recent last. Mirrored to the engine log.
    const List<std::string>& GetOutputLog() const { return myOutputLog; }

private:
    void RegisterReflectedCheats();
    void RegisterBuiltInCommands();

    bool InvokeReflectedCommand(const ConsoleCommand& inCommand, const List<std::string>& inArgs);
    std::string DescribeSignature(const ConsoleCommand& inCommand) const;

    void Print(const std::string& inLine);

    List<ConsoleCommand> myCommands;
    List<std::string> myOutputLog;
};

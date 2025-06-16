#include "ReflectionFileBuilder.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <set>

#include "ReflectionCache.h"

ReflectionFileBuilder::ReflectionFileBuilder(const ReflectionCache& inCache)
{
    std::ofstream file("../Launcher/GeneratedReflectionData.hpp");

    std::string includes;
    BuildClassIncludes(inCache, includes);

    std::string classDeclarations;
    BuildClassDeclarations(inCache, classDeclarations);

    std::string classContentDeclarations;
    BuildClassContentDeclarations(inCache, classContentDeclarations);

    std::string fileContent;
    CreateFileContent(includes, classDeclarations, classContentDeclarations, fileContent);
    
    file.clear();
    file << fileContent;

    file.close();
}

void ReflectionFileBuilder::BuildClassIncludes(const ReflectionCache& inCache, std::string& outString)
{
    for (const std::string& filePath : inCache.GetCachedFiles() | std::views::keys)
    {
        outString += "#include \"" + filePath + "\"\n";
    }
}

void ReflectionFileBuilder::BuildClassDeclarations(const ReflectionCache& inCache, std::string& outString)
{
    for (const CachedFileData& entry : inCache.GetCachedFiles() | std::views::values)
    {
        for (const ReflectedClass& reflectedClass : entry.myClasses)
        {
            outString += "reflectionSystem.AddClass<" + reflectedClass.GetClassName() + ">(\"" + reflectedClass.GetClassName() + "\", typeid(" + reflectedClass.GetClassName() + ").name());\n";
        }
    }
}

void ReflectionFileBuilder::BuildClassContentDeclarations(const ReflectionCache& inCache, std::string& outString)
{
    for (const CachedFileData& entry : inCache.GetCachedFiles() | std::views::values)
    {
        for (const ReflectedClass& reflectedClass : entry.myClasses)
        {
            outString += "{ \n";
            outString += "\tClass* currentClass = reflectionSystem.GetMutableClass<" + reflectedClass.GetClassName() + ">();\n";

            for (const ReflectedField& field : reflectedClass.GetFields())
            {
                outString += "\t{\n";
                outString += "\t\tField& currentField = currentClass->AddField(Field(\"" + field.GetFieldName() + "\", " + std::to_string(field.GetByteOffset()) +", reflectionSystem.GetOrCreateClass<" + field.GetFieldType() + ">(\"" + field.GetFieldType() + "\")));\n";
                for (const std::string& metadata : field.GetFieldMetadata())
                {
                    outString += "\t\tcurrentField.AddMetadata(\"" + metadata + "\");\n";
                }
                outString += "\t}\n";
            }

            for (const std::string& baseClassTypeName : reflectedClass.GetBaseTypeNames())
            {
                outString += "\tcurrentClass->AddBaseClass(reflectionSystem.GetMutableClass<" + baseClassTypeName + ">());\n";
            }

            for (const ReflectedMethod& method : reflectedClass.GetMethods())
            {
                /*
                    *Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void* 
                    {
                    String* instance = static_cast<String*>(inInstance);
                    std::basic_string<char> & arg0 = (std::basic_string<char>&)(inArguments[0]);
                    std::basic_string<char> & result = instance->ToLower(arg0);
                    return (void*)&result;
                    });
                */
                
                // Add method reflection code here.
                outString += "{\n";

                outString += "Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*\n";
                outString += "{\n";
                outString += reflectedClass.GetClassName() + "* instance = static_cast<" + reflectedClass.GetClassName() + "*>(inInstance);\n";

                std::string argList;
                for (int i = 0; i < method.GetArguments().size(); ++i)
                {
                    const ReflectedMethodArgument& arg = method.GetArguments()[i];
                    if (arg.GetIsPointer())
                        outString += arg.GetArgumentType() + " arg" + std::to_string(i) + " = (" + arg.GetUnqualifiedTypeName() + "*)inArguments[" + std::to_string(i) + "];\n";
                    else
                        outString += arg.GetArgumentType() + " arg" + std::to_string(i) + " = *(" + arg.GetUnqualifiedTypeName() + "*)inArguments[" + std::to_string(i) + "];\n";
                    
                    if (argList.empty())
                        argList += "arg" + std::to_string(i);
                    else
                        argList += ", arg" + std::to_string(i);
                }

                if (method.GetReturnTypeName() == "void")
                {
                    outString += "instance->" + method.GetMethodName() + "(" + argList + ");\n";
                    outString += "return nullptr;\n";
                }
                else
                {
                    if (method.IsReturnTypePointer() || method.IsReturnTypeReference())
                        outString += method.GetReturnTypeName() + " result = instance->" + method.GetMethodName() + "(" + argList + ");\n";
                    else
                        outString += "static thread_local " + method.GetReturnTypeName() + " result = instance->" + method.GetMethodName() + "(" + argList + ");\n";
                    
                    if (method.IsReturnTypePointer())
                        outString += "return (void*)result;\n";
                    else
                        outString += "return (void*)&result;\n";
                }
                outString += "});\n";

                // Method& currentMethod = currentClass->AddMethod(Method(inMethodName, GetClass<ClassType>(), invoker, listWithARgumetns));
                outString += "List<MethodArgument> arguments{};\n";

                for (const ReflectedMethodArgument& arg : method.GetArguments())
                {
                    outString += "arguments.Add(MethodArgument(\"" + arg.GetArgumentName() + "\", reflectionSystem.GetOrCreateClass<" + arg.GetArgumentType() + ">(\"" + arg.GetArgumentType() + "\")));\n";
                }
                
                outString += "Method& currentMethod = currentClass->AddMethod(Method(\"" + method.GetMethodName() + "\", reflectionSystem.GetOrCreateClass<" + method.GetReturnTypeName() + ">(\"" + method.GetReturnTypeName() + "\"), invoker, arguments));\n";

                for (const std::string& metadata : method.GetMetadata())
                {
                    outString += "currentMethod.AddMetadata(\"" + metadata + "\");\n";
                }
                
                outString += "}\n";
            }
            
            outString += "}\n";
        }
    }
}

void ReflectionFileBuilder::CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent)
{
    std::stringstream stringStream;

    std::ifstream fileTemplate("../ReflectionGenerator/ReflectionFileTemplate.txt");
    stringStream << fileTemplate.rdbuf();
    fileTemplate.close();

    outContent = stringStream.str();

    outContent = std::regex_replace(outContent, std::regex(R"(\$\(\s*FileIncludes\s*\))"), inIncludes);
    outContent = std::regex_replace(outContent, std::regex(R"(\$\(\s*ClassDeclarations\s*\))"), inClassDeclarations);
    outContent = std::regex_replace(outContent, std::regex(R"(\$\(\s*ClassContentDeclaration\s*\))"), inClassContentDeclarations);
}

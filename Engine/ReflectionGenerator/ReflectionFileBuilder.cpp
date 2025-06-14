#include "ReflectionFileBuilder.h"
#include <fstream>
#include <regex>
#include <sstream>

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
                // TODO: Implement pointer and reference bools here.
                outString += "\tcurrentClass->AddField(Field(\"" + field.GetFieldName() + "\", " + std::to_string(field.GetByteOffset()) +", reflectionSystem.GetClass<" + field.GetFieldType() + ">()));\n";
            }

            for (const std::string& baseClassTypeName : reflectedClass.GetBaseTypeNames())
            {
                outString += "\tcurrentClass->AddBaseClass(reflectionSystem.GetMutableClass<" + baseClassTypeName + ">());\n";
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
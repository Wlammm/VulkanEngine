#include "ReflectionFileBuilder.h"
#include <fstream>
#include <regex>
#include <sstream>

#include "ReflectionJobScheduler.h"

ReflectionFileBuilder::ReflectionFileBuilder(const ReflectionParser* inReflectionParser, const ReflectionJobScheduler& inScheduler)
{
    std::ofstream file("../Launcher/GeneratedReflectionData.hpp");

    std::string includes;
    BuildClassIncludes(inScheduler, includes);

    std::string classDeclarations;
    BuildClassDeclarations(inReflectionParser, classDeclarations);

    std::string classContentDeclarations;
    BuildClassContentDeclarations(inReflectionParser, classContentDeclarations);

    std::string fileContent;
    CreateFileContent(includes, classDeclarations, classContentDeclarations, fileContent);
    
    file.clear();
    file << fileContent;

    file.close();
}

void ReflectionFileBuilder::BuildClassIncludes(const ReflectionJobScheduler& inScheduler, std::string& outString)
{
    for (const std::string& path : inScheduler.GetHeaders())
    {
        outString += "#include \"" + path + "\"\n";
    }
}

void ReflectionFileBuilder::BuildClassDeclarations(const ReflectionParser* inParser, std::string& outString)
{
    for (const Class& reflectedClass : inParser->GetClasses())
    {
        outString += "reflectionSystem.AddClass(\"" + reflectedClass.GetClassName() + "\", typeid(" + reflectedClass.GetClassName() + ").name());\n";
    }
}

void ReflectionFileBuilder::BuildClassContentDeclarations(const ReflectionParser* inParser, std::string& outString)
{
    for (const Class& reflectedClass : inParser->GetClasses())
    {
        outString += "{ \n";
        outString += "\tClass* currentClass = reflectionSystem.GetMutableClass<" + reflectedClass.GetClassName() + ">();\n";

        for (const ReflectedField& field : reflectedClass.GetFields())
        {
            // TODO: Implement pointer and reference bools here.
            outString += "\tcurrentClass->AddField(Field(\"" + field.GetFieldName() + "\", offsetof(" + reflectedClass.GetClassName() + ", " + field.GetFieldName() + "), reflectionSystem.GetClass<" + field.GetFieldType() + ">(), false, false /* Implement this*/));\n";
        }
        // TODO: Implement base class detection and add it here.
        outString += "\t// Base classes are not implemented yet\n";
        outString += "}\n";
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
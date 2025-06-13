#include "ReflectionFileGenerator.h"
#include <fstream>
#include <regex>
#include <sstream>

ReflectionFileGenerator::ReflectionFileGenerator(const std::vector<ReflectionParser>& inReflectionParsers)
{
    std::ofstream file("../Launcher/GeneratedReflectionData.hpp");

    std::string includes;
    BuildClassIncludes(inReflectionParsers, includes);

    std::string classDeclarations;
    BuildClassDeclarations(inReflectionParsers, classDeclarations);

    std::string classContentDeclarations;
    BuildClassContentDeclarations(inReflectionParsers, classContentDeclarations);

    std::string fileContent;
    CreateFileContent(includes, classDeclarations, classContentDeclarations, fileContent);
    
    file.clear();
    file << fileContent;

    file.close();
}

void ReflectionFileGenerator::BuildClassIncludes(const std::vector<ReflectionParser>& inReflectionParsers,
                                                 std::string& outString)
{
    for (const ReflectionParser& parser : inReflectionParsers)
    {
        outString += "#include \"" + parser.GetFile() + "\"\n";
    }
}

void ReflectionFileGenerator::BuildClassDeclarations(const std::vector<ReflectionParser>& inReflectionParsers, std::string& outString)
{
    for (const ReflectionParser& parser : inReflectionParsers)
    {
        for (const Class& reflectedClass : parser.GetClasses())
        {
            outString += "reflectionSystem.AddClass(\"" + reflectedClass.GetClassName() + "\", typeid(" + reflectedClass.GetClassName() + ").name());\n";
        }
    }
}

void ReflectionFileGenerator::BuildClassContentDeclarations(const std::vector<ReflectionParser>& inReflectionParsers, std::string& outString)
{
    for (const ReflectionParser& parser : inReflectionParsers)
    {
        for (const Class& reflectedClass : parser.GetClasses())
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
}

void ReflectionFileGenerator::CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent)
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
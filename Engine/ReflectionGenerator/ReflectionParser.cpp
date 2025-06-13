#include "ReflectionParser.h"

#include <iostream>
#include <__msvc_ostream.hpp>
#include <clang-c/Index.h>

#include "IncludePaths.h"
#include "PathUtils.hpp"
#include "../ImGui/imgui_internal.h"

ReflectionParser::ReflectionParser(const std::string& inFileToReflect, const IncludePaths& inIncludePaths)
    : myIncludePaths(inIncludePaths)
{
    myFileToReflect = inFileToReflect;
    
    Parse();
}

void ReflectionParser::Parse()
{
    clientData = {this, {}};
    CXIndex index = clang_createIndex(0, 0);

    std::vector<const char*> commandLineArgs;
    BuildCommandLineArgs(commandLineArgs);
    
    CXTranslationUnit unit = clang_parseTranslationUnit(index, myFileToReflect.c_str(), commandLineArgs.data(), static_cast<int>(commandLineArgs.size()), nullptr, 0, CXTranslationUnit_SkipFunctionBodies);

    unsigned numDiagnostics = clang_getNumDiagnostics(unit);
    for (unsigned i = 0; i < numDiagnostics; ++i)
    {
        CXDiagnostic diag = clang_getDiagnostic(unit, i);
        CXString diagSpelling = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
        std::cout << clang_getCString(diagSpelling) << std::endl;
        clang_disposeString(diagSpelling);
        clang_disposeDiagnostic(diag);
    }

    if (unit == nullptr)
    {
        myFailed = true;
        myErrorMessages.push_back("Clang failed to parse translation unit.");
        return;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, &ReflectionParser::TraverseAST, &clientData);
}

bool ReflectionParser::Failed() const
{
    return myFailed;
}

const std::unordered_map<std::string, std::list<ReflectedClass>>& ReflectionParser::GetClassData() const
{
    return myClassesInFiles;
}

const std::string& ReflectionParser::GetFile() const
{
    return myFileToReflect;
}

const std::vector<std::string>& ReflectionParser::GetErrorMessages() const
{
    return myErrorMessages;
}

const IncludePaths& ReflectionParser::GetIncludePaths() const
{
    return myIncludePaths;
}

CXChildVisitResult ReflectionParser::TraverseAST(CXCursor inCurrentCursor, CXCursor inParentCursor, CXClientData inClientData)
{
    ClientData& clientData = *static_cast<ClientData*>(inClientData);
    
    CXCursorKind kind = clang_getCursorKind(inCurrentCursor);
    if (kind == CXCursor_StructDecl || kind == CXCursor_ClassDecl || kind == CXCursor_EnumDecl)
    {
        const std::string className = GetDisplayName(inCurrentCursor);

        // If the class is declared in another file and not the one we're searching we can just skip it.
        const CXSourceLocation location = clang_getCursorLocation(inCurrentCursor);
        if (clang_Location_isInSystemHeader(location))
            return CXChildVisit_Continue;

        if (!clang_isCursorDefinition(inCurrentCursor))
            return CXChildVisit_Continue;

        const std::string fileName = GetFileName(location);
        ReflectedClass& currentClass = clientData.self->AddClass(fileName, ReflectedClass(BuildClassNameFromStack(clientData.classStack, className)));
        clientData.classStack.emplace_back(&currentClass);

        clang_visitChildren(inCurrentCursor, &ReflectionParser::TraverseAST, inClientData);

        clientData.classStack.pop_back();
        
        return CXChildVisit_Continue;
    }

    if (kind == CXCursor_FieldDecl)
    {
        const std::string fieldName = GetSpelling(inCurrentCursor);
        const CXType type = clang_getCanonicalType(clang_getCursorType(inCurrentCursor));
        if (type.kind == CXType_Invalid)
        {
            clientData.self->myErrorMessages.push_back("Class: " + clientData.classStack.back()->GetClassName() + " found invalid type with name: " + fieldName);
        }
        
        const std::string typeName = GetSpelling(type);
        clientData.classStack.back()->AddField(ReflectedField(fieldName, typeName));
    }
    
    return CXChildVisit_Continue;
}

void ReflectionParser::BuildCommandLineArgs(std::vector<const char*>& outCommandLineArgs) const
{
    outCommandLineArgs.push_back("-x");
    outCommandLineArgs.push_back("c++");
    outCommandLineArgs.push_back("-std=c++23");
    outCommandLineArgs.push_back("-nostdinc++");
    outCommandLineArgs.push_back("-fms-compatibility");
    outCommandLineArgs.push_back("-fms-extensions");
    outCommandLineArgs.push_back("-fmsc-version=1929"); 
    outCommandLineArgs.push_back("-Wno-pragma-once-outside-header");
    outCommandLineArgs.push_back("-D_MSC_VER=1929");
    outCommandLineArgs.push_back("-D_WIN32");
    outCommandLineArgs.push_back("-D_WINDOWS");
    outCommandLineArgs.push_back("-Wno-everything");
    // outCommandLineArgs.push_back("-DIGNORED_BY_REFLECTION=1");
    outCommandLineArgs.push_back("-D_CRT_USE_BUILTIN_OFFSETOF=1");
    outCommandLineArgs.push_back("-DNDEBUG=1");

    for (const std::string& includeArg : myIncludePaths.GetIncludeArguments())
    {
        outCommandLineArgs.push_back(includeArg.c_str());
    }
}

std::string ReflectionParser::GetSpelling(const CXCursor& inCursor)
{
    const CXString clangSpelling = clang_getCursorSpelling(inCursor);
    const std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

std::string ReflectionParser::GetSpelling(const CXType& inType)
{
    const CXString clangSpelling = clang_getTypeSpelling(inType);
    const std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

std::string ReflectionParser::GetSpelling(const CXCursorKind inCursorKind)
{
    const CXString clangSpelling = clang_getCursorKindSpelling(inCursorKind);
    const std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

std::string ReflectionParser::GetDisplayName(const CXCursor& inCursor)
{
    const CXString clangDisplayName = clang_getCursorDisplayName(inCursor);
    const std::string displayName = clang_getCString(clangDisplayName);
    clang_disposeString(clangDisplayName);
    return displayName;
}



std::string ReflectionParser::GetFileName(const CXSourceLocation& inLocation)
{
    CXFile file;
    unsigned int line, column, offset;

    clang_getExpansionLocation(inLocation, &file, &line, &column, &offset);

    const CXString clangFilename = clang_getFileName(file);
    std::string filename = clang_getCString(clangFilename);
    clang_disposeString(clangFilename);

    filename = PathUtils::NormalizePath(filename);
    return filename;
}

std::string ReflectionParser::BuildClassNameFromStack(const std::vector<ReflectedClass*>& inClassStack, const std::string& inNewClassName)
{
    std::string className;
    for (ReflectedClass* currentClass : inClassStack)
    {
        className += currentClass->GetClassName();
        className += "::";
    }
    className += inNewClassName;
    return className;
}

ReflectedClass& ReflectionParser::AddClass(const std::string& inFile, const ReflectedClass& inClass)
{
    if (myClassesInFiles.find(inFile) == myClassesInFiles.end())
    {
        myClassesInFiles[inFile] = { inClass };
        return myClassesInFiles[inFile].back();
    }
    
    myClassesInFiles[inFile].push_back(inClass);
    return myClassesInFiles[inFile].back();
}

#include "ReflectFile.h"

#include <iostream>
#include <__msvc_ostream.hpp>
#include <clang-c/Index.h>

#include "IncludePaths.h"
#include "../ImGui/imgui_internal.h"

ReflectFile::ReflectFile(const std::string& inFileToReflect, const IncludePaths& inIncludePaths)
    : myIncludePaths(inIncludePaths)
{
    myFileToReflect = inFileToReflect;

    clientData = {this, {}};
}

void ReflectFile::Start()
{
    CXIndex index = clang_createIndex(0, 0);

    std::vector<const char*> commandLineArgs;
    BuildCommandLineArgs(commandLineArgs);
    
    CXTranslationUnit unit = clang_parseTranslationUnit(index, myFileToReflect.c_str(), commandLineArgs.data(), static_cast<int>(commandLineArgs.size()), nullptr, 0, CXTranslationUnit_None);

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
        myIsComplete = true;
        return;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, &ReflectFile::TraverseAST, &clientData);
    
}

bool ReflectFile::Failed() const
{
    return myFailed;
}

const std::atomic_bool& ReflectFile::IsComplete() const
{
    return myIsComplete;
}

const std::vector<ReflectedClass>& ReflectFile::GetClasses() const
{
    return myClasses;
}

const std::string& ReflectFile::GetFile() const
{
    return myFileToReflect;
}

const std::vector<std::string>& ReflectFile::GetErrorMessages() const
{
    return myErrorMessages;
}

CXChildVisitResult ReflectFile::TraverseAST(CXCursor inCurrentCursor, CXCursor inParentCursor, CXClientData inClientData)
{
    ClientData& clientData = *static_cast<ClientData*>(inClientData);
    
    CXCursorKind kind = clang_getCursorKind(inCurrentCursor);
    if (kind == CXCursor_StructDecl || kind == CXCursor_ClassDecl || kind == CXCursor_EnumDecl)
    {
        CXString displayName = clang_getCursorDisplayName(inCurrentCursor);
        std::string className = clang_getCString(displayName);
        clang_disposeString(displayName);

        // If the class is declared in another file and not the one we're searching we can just skip it.
        CXSourceLocation location = clang_getCursorLocation(inCurrentCursor);
        if (!clang_Location_isFromMainFile(location))
            return CXChildVisit_Continue;
        
        ReflectedClass& currentClass = clientData.self->myClasses.emplace_back(className);
        clientData.classStack.emplace(&currentClass);

        clang_visitChildren(inCurrentCursor, &ReflectFile::TraverseAST, inClientData);

        clientData.classStack.pop();
        
        return CXChildVisit_Continue;
    }

    if (kind == CXCursor_FieldDecl)
    {
        std::string fieldName = GetSpelling(inCurrentCursor);
        CXType type = clang_getCanonicalType(clang_getCursorType(inCurrentCursor));
        if (type.kind == CXType_Invalid)
        {
            clientData.self->myErrorMessages.push_back("Class: " + clientData.classStack.top()->GetClassName() + " found invalid type with name: " + fieldName);
        }
        
        std::string typeName = GetSpelling(type);
        clientData.classStack.top()->AddField(ReflectedField(fieldName, typeName));
    }
    
    return CXChildVisit_Continue;
}

void ReflectFile::BuildCommandLineArgs(std::vector<const char*>& outCommandLineArgs)
{
    outCommandLineArgs.push_back("-x");
    outCommandLineArgs.push_back("c++");
    outCommandLineArgs.push_back("-std=c++23");
    outCommandLineArgs.push_back("-nostdinc++");
    outCommandLineArgs.push_back("-DIGNORED_BY_REFLECTION=1");
    outCommandLineArgs.push_back("-DNDEBUG=1");
    outCommandLineArgs.push_back("-include../Engine/EnginePch.h");

    for (const std::string& includeArg : myIncludePaths.GetIncludeArguments())
    {
        outCommandLineArgs.push_back(includeArg.c_str());
    }
}

std::string ReflectFile::GetSpelling(CXCursor inCursor)
{
    CXString clangSpelling = clang_getCursorSpelling(inCursor);
    std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

std::string ReflectFile::GetSpelling(CXType inType)
{
    CXString clangSpelling = clang_getTypeSpelling(inType);
    std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

std::string ReflectFile::GetSpelling(CXCursorKind inCursorKind)
{
    CXString clangSpelling = clang_getCursorKindSpelling(inCursorKind);
    std::string spelling = clang_getCString(clangSpelling);
    clang_disposeString(clangSpelling);
    return spelling;
}

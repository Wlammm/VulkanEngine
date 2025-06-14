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
    
    myTranslationUnit = clang_parseTranslationUnit(index, myFileToReflect.c_str(), commandLineArgs.data(), static_cast<int>(commandLineArgs.size()), nullptr, 0, CXTranslationUnit_SkipFunctionBodies);

    unsigned numDiagnostics = clang_getNumDiagnostics(myTranslationUnit);
    for (unsigned i = 0; i < numDiagnostics; ++i)
    {
        CXDiagnostic diag = clang_getDiagnostic(myTranslationUnit, i);
        CXString diagSpelling = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
        std::cout << clang_getCString(diagSpelling) << std::endl;
        clang_disposeString(diagSpelling);
        clang_disposeDiagnostic(diag);
    }

    if (myTranslationUnit == nullptr)
    {
        myFailed = true;
        myErrorMessages.push_back("Clang failed to parse translation unit.");
        return;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(myTranslationUnit);
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

        // Dont iterate private classes as they wont work with our reflected file.
        CX_CXXAccessSpecifier accessSpecifier = clang_getCXXAccessSpecifier(inCurrentCursor);
        if (accessSpecifier != CX_CXXPublic && accessSpecifier != CX_CXXInvalidAccessSpecifier)
            return CXChildVisit_Continue;

        const std::string fileName = GetFileName(location);
        ReflectedClass& currentClass = clientData.self->AddClass(fileName, ReflectedClass(BuildClassNameFromStack(clientData.classStack, className)));
        clientData.classStack.emplace_back(&currentClass);

        clang_visitChildren(inCurrentCursor, &ReflectionParser::TraverseAST, inClientData);

        clientData.classStack.pop_back();
        
        return CXChildVisit_Continue;
    }

    if (kind == CXCursor_CXXBaseSpecifier)
    {
        CXType baseType = clang_getCanonicalType(clang_getCursorType(inCurrentCursor));
        std::string baseClassName = GetSpelling(baseType);
        clientData.classStack.back()->AddBaseClass(baseClassName);
    }

    if (kind == CXCursor_AnnotateAttr || kind == CXCursor_UnexposedAttr )
    {
        std::string name = GetSpelling(inCurrentCursor);
        int a= 10;
    }

    if (kind == CXCursor_FieldDecl)
    {
        const std::string fieldName = GetSpelling(inCurrentCursor);
        const CXType type = clang_getCanonicalType(clang_getCursorType(inCurrentCursor));
        if (type.kind == CXType_Invalid)
        {
            clientData.self->myErrorMessages.push_back("Class: " + clientData.classStack.back()->GetClassName() + " found invalid type with name: " + fieldName);
        }

        if (!IsTypePublicRecursive(type))
            return CXChildVisit_Continue;
        
        const uint32_t byteOffset = GetByteOffsetOfField(inCurrentCursor, fieldName);

        const std::vector<std::string> fieldMetaData = GetFieldMetaData(inCurrentCursor);
        
        const std::string typeName = GetSpelling(type);
        clientData.classStack.back()->AddField(ReflectedField(fieldName, typeName, byteOffset, fieldMetaData));
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
    outCommandLineArgs.push_back("-DREFLECTION_GENERATION=1");
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

uint32_t ReflectionParser::GetByteOffsetOfField(const CXCursor& inCurrentCursor, const std::string& inFieldName)
{
    CXCursor classCursor = clang_getCursorSemanticParent(inCurrentCursor);
    CXType classType = clang_getCursorType(classCursor);
    long long bitOffset = clang_Type_getOffsetOf(classType, inFieldName.c_str());
    long long byteOffset = 0;
    if (bitOffset < 0)
    {
        std::cout << "Failed to get offset for field: " << inFieldName << std::endl;
    }
    else
    {
        byteOffset = bitOffset / 8;
    }
    return static_cast<uint32_t>(byteOffset);
}

bool ReflectionParser::IsTypePublicRecursive(CXType type)
{
    // Handle pointers, references, arrays
    while (true)
    {
        if (type.kind == CXType_Pointer || type.kind == CXType_LValueReference || type.kind == CXType_RValueReference)
        {
            type = clang_getPointeeType(type);
            continue;
        }

        if (type.kind == CXType_ConstantArray || type.kind == CXType_IncompleteArray)
        {
            type = clang_getArrayElementType(type);
            continue;
        }

        break;
    }

    // Check type declaration
    CXCursor typeDecl = clang_getTypeDeclaration(type);
    if (!clang_Cursor_isNull(typeDecl))
    {
        // Handle template specializations
        CXCursor specialized = clang_getSpecializedCursorTemplate(typeDecl);
        if (!clang_Cursor_isNull(specialized))
            typeDecl = specialized;

        CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(typeDecl);
        if (access != CX_CXXPublic && access != CX_CXXInvalidAccessSpecifier)
            return false;

        // Walk semantic parents for nested private classes
        CXCursor parent = clang_getCursorSemanticParent(typeDecl);
        while (!clang_Cursor_isNull(parent))
        {
            CX_CXXAccessSpecifier parentAccess = clang_getCXXAccessSpecifier(parent);
            if (parentAccess != CX_CXXPublic && parentAccess != CX_CXXInvalidAccessSpecifier)
                return false;

            parent = clang_getCursorSemanticParent(parent);
        }
    }

    // Now handle templates recursively
    int numTemplateArgs = clang_Type_getNumTemplateArguments(type);
    if (numTemplateArgs > 0)
    {
        for (int i = 0; i < numTemplateArgs; ++i)
        {
            CXType templateArg = clang_Type_getTemplateArgumentAsType(type, i);
            if (templateArg.kind == CXType_Invalid)
                continue;

            if (!IsTypePublicRecursive(templateArg))
                return false;
        }
    }

    return true;
}

std::string GetSourceText(CXSourceRange range, CXTranslationUnit translationUnit)
{
    CXSourceLocation startLoc = clang_getRangeStart(range);
    CXSourceLocation endLoc = clang_getRangeEnd(range);

    CXFile file;
    unsigned startOffset;
    clang_getFileLocation(startLoc, &file, nullptr, nullptr, &startOffset);

    unsigned endOffset;
    clang_getFileLocation(endLoc, nullptr, nullptr, nullptr, &endOffset);

    if (file == nullptr || endOffset < startOffset)
        return "";

    // Access the source file buffer from the translation unit
    const char* fileContents = clang_getFileContents(translationUnit, file, nullptr);
    if (!fileContents)
        return "";

    return std::string(fileContents + startOffset, endOffset - startOffset);
}

std::vector<std::string> ReflectionParser::GetFieldMetaData(const CXCursor& fieldCursor)
{
    std::vector<std::string> metaData;
    
    clang_visitChildren(fieldCursor,
        [](CXCursor cursor, CXCursor /*parent*/, CXClientData clientData) -> CXChildVisitResult {
            auto* metaVec = static_cast<std::vector<std::string>*>(clientData);
            
            if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr)
            {
                std::string annotation = GetSpelling(cursor);
                std::istringstream iss(annotation);
                std::string token;
                while (std::getline(iss, token, ','))
                {
                    token.erase(0, token.find_first_not_of(" \t"));
                    token.erase(token.find_last_not_of(" \t") + 1);
                    if (!token.empty())
                        metaVec->push_back(token);
                }
            }
            return CXChildVisit_Recurse;
        },
        &metaData
    );
    
    return metaData;
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

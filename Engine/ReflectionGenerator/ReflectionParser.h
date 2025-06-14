#pragma once
#include <atomic>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "ReflectedClass.h"
#include "IncludePaths.h"

class ReflectionParser
{
public:
    ReflectionParser(const std::string& inFileToReflect, const IncludePaths& inIncludePaths);
    
    void Parse();

    bool Failed() const;

    const std::unordered_map<std::string, std::list<ReflectedClass>> & GetClassData() const;

    const std::string& GetFile() const;
    const std::vector<std::string>& GetErrorMessages() const;

    const IncludePaths& GetIncludePaths() const;

private:
    struct ClientData
    {
        ReflectionParser* self = nullptr;
        std::vector<ReflectedClass*> classStack;
    } clientData;
    
    static CXChildVisitResult TraverseAST(CXCursor inCurrentCursor, CXCursor inParentCursor, CXClientData inClientData);

    void BuildCommandLineArgs(std::vector<const char*>& outCommandLineArgs) const;
    static std::string GetSpelling(const CXCursor& inCursor);
    static std::string GetSpelling(const CXType& inType);
    static std::string GetSpelling(const CXCursorKind inCursorKind);

    static std::string GetDisplayName(const CXCursor& inCursor);
    static std::string GetFileName(const CXSourceLocation& inLocation);

    static std::string BuildClassNameFromStack(const std::vector<ReflectedClass*>& inClassStack, const std::string& inNewClassName);

    static uint32_t GetByteOffsetOfField(const CXCursor& inCurrentCursor, const std::string& inFieldName);

    static bool IsTypePublicRecursive(CXType inType);
    static std::vector<std::string> GetFieldMetaData(const CXCursor& inCursor);

    ReflectedClass& AddClass(const std::string& inFile, const ReflectedClass& inClass);
    
private:
    std::string myFileToReflect;
    IncludePaths myIncludePaths;

    inline static CXTranslationUnit myTranslationUnit;
    
    bool myFailed = false;

    std::unordered_map<std::string, std::list<ReflectedClass>> myClassesInFiles;

    std::vector<std::string> myErrorMessages;
};

#pragma once
#include <atomic>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "Class.h"

class IncludePaths;

class ReflectionParser
{
public:
    ReflectionParser(const std::string& inFileToReflect, const IncludePaths& inIncludePaths);
    
    void ParseInParallel();

    bool Failed() const;

    const std::list<Class>& GetClasses() const;

    const std::string& GetFile() const;
    const std::vector<std::string>& GetErrorMessages() const;

private:
    struct ClientData
    {
        ReflectionParser* self = nullptr;
        std::vector<Class*> classStack;
    } clientData;
    
    static CXChildVisitResult TraverseAST(CXCursor inCurrentCursor, CXCursor inParentCursor, CXClientData inClientData);

    void BuildCommandLineArgs(std::vector<const char*>& outCommandLineArgs);
    static std::string GetSpelling(CXCursor inCursor);
    static std::string GetSpelling(CXType inType);
    static std::string GetSpelling(CXCursorKind inCursorKind);

    static std::string GetDisplayName(CXCursor inCursor);

    static std::string BuildClassNameFromStack(const std::vector<Class*>& inClassStack, const std::string& inNewClassName);

private:
    std::string myFileToReflect;
    const IncludePaths& myIncludePaths;

    bool myFailed = false;

    std::list<Class> myClasses;

    std::vector<std::string> myErrorMessages;
};

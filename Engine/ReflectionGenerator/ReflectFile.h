#pragma once
#include <atomic>
#include <stack>
#include <string>
#include <clang-c/Index.h>

#include "ReflectedClass.h"

class IncludePaths;

class ReflectFile
{
public:
    ReflectFile(const std::string& inFileToReflect, const IncludePaths& inIncludePaths);

    void Start();

    bool Failed() const;

    const std::atomic_bool& IsComplete() const;

    const std::vector<ReflectedClass>& GetClasses() const;

    const std::string& GetFile() const;
    const std::vector<std::string>& GetErrorMessages() const;

private:
    struct ClientData
    {
        ReflectFile* self = nullptr;
        std::stack<ReflectedClass*> classStack;
    } clientData;
    
    static CXChildVisitResult TraverseAST(CXCursor inCurrentCursor, CXCursor inParentCursor, CXClientData inClientData);

    void BuildCommandLineArgs(std::vector<const char*>& outCommandLineArgs);
    static std::string GetSpelling(CXCursor inCursor);
    static std::string GetSpelling(CXType inType);
    static std::string GetSpelling(CXCursorKind inCursorKind);

private:
    std::string myFileToReflect;
    const IncludePaths& myIncludePaths;

    std::atomic_bool myIsComplete = false;

    bool myFailed = false;

    std::vector<ReflectedClass> myClasses;

    std::vector<std::string> myErrorMessages;
};

#include "EnginePch.h"
#include "Console.h"
#include "Engine/Engine.h"

constexpr int COLOR_WHITE = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
constexpr int COLOR_RED = FOREGROUND_RED;
constexpr int COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;

Console::Console(int argc, char** argv)
{
    check(!myInstance && "There can only be one instance of this class active at once.");
    myInstance = this;

    for(int i = 0; i < argc; ++i)
    {
        // Can use Engine::GetEngineProperties().HasStartupArgument("-LogToFile"); but this is constructed too early for that.
        if(String::ToLowerCopy(argv[i]) == "-logtofile")
        {
            myLogToFileEnabled = true;
            break;
        }
    }
    
    myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(myLogToFileEnabled)
    {
        myLogToFileStream.open("Log.txt");
        myCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(myLogToFileStream.rdbuf());
    }
}

Console::~Console()
{
    if(myLogToFileEnabled)
    {
        std::cout.rdbuf(myCoutBuffer);  // Restore original buffer
        myLogToFileStream.close();  // Close the file stream
    }

    myInstance = nullptr;
}

Console& Console::Get()
{
    return *myInstance;
}

void Console::Log(const char* inString, ...)
{
    va_list argptr;
    va_start(argptr, inString);

    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inString, argptr);
    va_end(argptr);

    const std::string msg = std::string("[LOG]: ") + buffer;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);

    std::cout << msg << std::endl;
    OutputDebugStringA(msg.c_str());
}

void Console::Log(const std::string& inString)
{
    const std::string msg = "[LOG]: " + inString;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
    std::cout << msg << std::endl;
    OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const char* inString, ...)
{
    va_list argptr;
    va_start(argptr, inString);

    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inString, argptr);
    va_end(argptr);

    const std::string msg = std::string("[WARNING]: ") + buffer;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_YELLOW);

    std::cout << msg << std::endl;
    OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const std::string& inString)
{
    const std::string msg = "[WARNING]: " + inString;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_YELLOW);
    std::cout << msg << std::endl;
    OutputDebugStringA(msg.c_str());
    SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
}

void Console::LogError(const char* inString, ...)
{
    va_list argptr;
    va_start(argptr, inString);

    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inString, argptr);
    va_end(argptr);

    const std::string msg = std::string("[ERROR]: ") + buffer;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_RED);

    std::cerr << msg << std::endl;
    OutputDebugStringA(msg.c_str());
    SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
}

void Console::LogError(const std::string& inString)
{
    const std::string msg = "[ERROR]: " + inString;
    SetConsoleTextAttribute(myConsoleHandle, COLOR_RED);
    std::cerr << msg << std::endl;
    OutputDebugStringA(msg.c_str());
    SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
}

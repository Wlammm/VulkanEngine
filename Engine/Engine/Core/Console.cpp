#include "EnginePch.h"
#include "Console.h"
#include "Engine.h"

constexpr int COLOR_WHITE = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
constexpr int COLOR_RED = FOREGROUND_RED;
constexpr int COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_BLUE;

Console::Console()
{
	check(!myInstance && "There can only be one instance of this class active at once.");
	myInstance = this;

	myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	myLogToFileEnabled = Engine::GetEngineProperties().HasStartupArgument("-LogToFile");

	if(myLogToFileEnabled)
	{
		myLogToFileStream = std::ofstream{ "Log.txt" };
		myCoutBuffer = std::cout.rdbuf();
		std::cout.rdbuf(myLogToFileStream.rdbuf());
	}
}

Console::~Console()
{
	if(myLogToFileEnabled)
	{
		// This is required as cout has longer lifetime than ofstream to prevent crashes.
		std::cout.rdbuf(myCoutBuffer);
	}

	myInstance = nullptr;
}

Console& Console::Get()
{
	return *myInstance;
}

void Console::Log(const char* inString, ...)
{
	const std::string msg = "[LOG]: " + std::string(inString);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);

	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stdout, msg.c_str(), argptr);
	va_end(argptr);

	std::cout << std::endl;
	OutputDebugStringA(msg.c_str());
}

void Console::Log(const std::string& inString)
{
	const std::string msg = "[LOG]: " + std::string(inString);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
	OutputDebugStringA(msg.c_str());
	std::cout << msg;
	OutputDebugStringA("\n");
}

void Console::LogWarning(const char* inString, ...)
{
	const std::string msg = "[WARNING]: " + std::string(inString);
	std::cout << "[WARNING]: ";
	SetConsoleTextAttribute(myConsoleHandle, COLOR_YELLOW);
	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stdout, msg.c_str(), argptr);
	va_end(argptr);
	SetConsoleTextAttribute(myConsoleHandle, 15);

	std::cout << std::endl;
	OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const std::string& inString, ...)
{
	const std::string msg = "[WARNING]: " + std::string(inString);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_YELLOW);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
	OutputDebugStringA(msg.c_str());
}

void Console::LogError(const char* inString, ...)
{
	const std::string msg = "[ERROR]: " + std::string(inString);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_RED);
	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stderr, msg.c_str(), argptr);
	va_end(argptr);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
	OutputDebugStringA(msg.c_str());
	OutputDebugStringA("\n");
	std::cerr << std::endl;
}

void Console::LogError(const std::string& inString, ...)
{
	const std::string msg = "[ERROR]: " + std::string(inString);
	SetConsoleTextAttribute(myConsoleHandle, COLOR_RED);
	std::cerr << msg;
	SetConsoleTextAttribute(myConsoleHandle, COLOR_WHITE);
	OutputDebugStringA(msg.c_str());
	std::cerr << std::endl;
}

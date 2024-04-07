#include "EnginePch.h"
#include "Console.h"
#include "Engine.h"

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
	const std::string msg = "[LOG]: " + std::string(inString) + "\n";
	std::cout << "[LOG]: ";
	SetConsoleTextAttribute(myConsoleHandle, 15);
	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stderr, inString, argptr);
	va_end(argptr);
	OutputDebugStringA(msg.c_str());
}

void Console::Log(const std::string& inString)
{
	const std::string msg = "[LOG]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 15);
	std::cout << msg;
	OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const char* inString, ...)
{
	const std::string msg = "[WARNING]: " + std::string(inString) + "\n";
	std::cout << "[WARNING]: ";
	SetConsoleTextAttribute(myConsoleHandle, 14);
	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stderr, inString, argptr);
	va_end(argptr);
	SetConsoleTextAttribute(myConsoleHandle, 15);

	OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const std::string& inString, ...)
{
	const std::string msg = "[WARNING]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 14);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}

void Console::LogError(const char* inString, ...)
{
	const std::string msg = "[ERROR]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 12);
	va_list argptr;
	va_start(argptr, inString);
	vfprintf(stderr, inString, argptr);
	va_end(argptr);
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}

void Console::LogError(const std::string& inString, ...)
{
	const std::string msg = "[ERROR]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 12);
	std::cerr << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}

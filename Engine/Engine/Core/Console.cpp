#include "EnginePch.h"
#include "Console.h"

Console::Console()
{
	check(!myInstance && "There can only be one instance of this class active at once.");
	myInstance = this;

	myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console::~Console()
{
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
	std::cout << std::endl;
}

void Console::Log(const std::string& inString)
{
	const std::string msg = "[LOG]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 15);
	std::cout << msg << std::endl;
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
	std::cout << std::endl;
}

void Console::LogWarning(const std::string& inString, ...)
{
	const std::string msg = "[WARNING]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 14);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
	std::cout << std::endl;
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
	std::cout << std::endl;
}

void Console::LogError(const std::string& inString, ...)
{
	const std::string msg = "[ERROR]: " + std::string(inString) + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 12);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
	std::cout << std::endl;
}

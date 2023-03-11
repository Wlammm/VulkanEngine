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

void Console::Log(const std::string& inString)
{
	const std::string msg = "[LOG]: " + inString + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 15);
	std::cout << msg;
	OutputDebugStringA(msg.c_str());
}

void Console::LogWarning(const std::string& inString)
{
	const std::string msg = "[WARNING]: " + inString + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 14);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}

void Console::LogError(const std::string& inString)
{
	const std::string msg = "[ERROR]: " + inString + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 12);
	std::cout <<  msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}
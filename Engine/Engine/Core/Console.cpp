#include "Console.h"
#include "EngineDefines.hpp"
#include <iostream>

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
	SetConsoleTextAttribute(myConsoleHandle, 15);
	std::cout << "[LOG]: " << inString << std::endl;
}

void Console::LogWarning(const std::string& inString)
{
	SetConsoleTextAttribute(myConsoleHandle, 14);
	std::cout << "[WARNING]: " << inString << std::endl;
	SetConsoleTextAttribute(myConsoleHandle, 15);
}

void Console::LogError(const std::string& inString)
{
	const std::string msg = "[ERROR]: " + inString + "\n";
	SetConsoleTextAttribute(myConsoleHandle, 12);
	std::cout << msg;
	SetConsoleTextAttribute(myConsoleHandle, 15);
	OutputDebugStringA(msg.c_str());
}
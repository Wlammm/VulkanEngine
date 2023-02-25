module;
#include "Utils/String.hpp"
#include "Math/Defines.hpp"
#include <unordered_set>

export module EngineProperties;

export struct EngineProperties
{
	std::wstring Title = L"Default Engine";

	// Start size of the window.
	uint WindowWidth = 1920;
	uint WindowHeight = 1080;

	// Start position of window.
	uint WindowX = 100;
	uint WindowY = 100;

	void AddStartupArguments(int inArgC, char** inArgv)
	{
		for(int i = 0; i < inArgC; ++i)
		{
			StartupArguments.insert(String::ToLowerCopy(inArgv[i]));
		}
	}

	void AddStartupArgument(const std::string& inArg)
	{
		StartupArguments.insert(String::ToLowerCopy(inArg));
	}

	bool HasStartupArgument(const std::string& inArg) const
	{
		return StartupArguments.count(String::ToLowerCopy(inArg));
	}

private:
	std::unordered_set<std::string> StartupArguments;
};
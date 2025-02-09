#pragma once

#include "Engine/EnginePch.h"
#include "Utils/String.hpp"

struct EngineProperties
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

	// Expects argument as -ArgumentName. Not case sensitive.
	bool HasStartupArgument(const std::string& inArg) const
	{
		return StartupArguments.count(String::ToLowerCopy(inArg));
	}

private:
	std::unordered_set<std::string> StartupArguments;
};
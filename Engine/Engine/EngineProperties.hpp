#pragma once

#include "Engine/EnginePch.h"

struct EngineProperties
{
	std::wstring Title = L"Default Engine";

	// Start size of the window.
	uint WindowWidth = 1920;
	uint WindowHeight = 1080;

	// Start position of window.
	uint WindowX = 100;
	uint WindowY = 100;
};
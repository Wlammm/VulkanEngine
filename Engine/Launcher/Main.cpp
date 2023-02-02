#include <iostream>
#include "Engine/Engine.h"

int main()
{
	EngineProperties properties{};
	properties.Title = L"Engine";

	Engine engine{ properties };

	bool isRunning = true;
	while(isRunning)
	{
		engine.Tick();

		if (!engine.ShouldRun())
			isRunning = false;
	}

	return 0;
}
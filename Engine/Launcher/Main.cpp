#include <iostream>
#include "Engine/Engine.h"

int main()
{
	Engine engine{};

	bool isRunning = true;
	while(isRunning)
	{
		engine.Tick();

		if (!engine.ShouldRun())
			isRunning = false;
	}

	return 0;
}
#include <iostream>
#include "Engine/Engine.h"

int main()
{

	try
	{
		EngineProperties properties{};
		properties.Title = L"Engine";

		Engine engine{ properties };

		bool isRunning = true;
		while (isRunning)
		{
			engine.Tick();

			if (!engine.ShouldRun())
				isRunning = false;
		}

		return 0;
	}
	catch(const std::exception& e)
	{
		std::string s = "CRASH due to: ";
		s += e.what();
		LOG_ERROR(s);
		return 1;
	}
}
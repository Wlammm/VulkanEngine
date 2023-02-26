#include <iostream>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

import Engine;
#include "Engine/Core/Console.h"

int main(int argc, char** argv)
{
	try
	{
		EngineProperties properties{};
		properties.Title = L"Engine";
		properties.AddStartupArguments(argc, argv);

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
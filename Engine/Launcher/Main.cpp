#include <iostream>
#include "Engine/Engine.h"
#include "Engine/Core/Console.h"

#if EDITOR
#include <Editor/Editor.h>
#endif

int main(int argc, char** argv)
{
	Console console{argc, argv};
	try
	{
		EngineProperties properties{};
		properties.Title = L"Engine";
		properties.AddStartupArguments(argc, argv);

		Engine engine{ properties };

#if EDITOR
		Editor editor{};
		Engine::SetEditorTickFunction(Editor::StaticTick);
#endif

		bool isRunning = true;
		while (isRunning)
		{
			engine.Tick();

			if (!engine.ShouldRun())
				isRunning = false;
		}

		return 0;
	}
	catch(...)
	{
		std::exception_ptr eptr = std::current_exception();
		try
		{
			if (eptr)
				std::rethrow_exception(eptr);
		}
		catch(const std::exception& e)
		{
			LOG("Caught exception: %s", e.what());
			return 1;
		}
	}
}
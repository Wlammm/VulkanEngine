// This always needs to be at the top
#include "GeneratedReflectionData.hpp"

#include <iostream>
#include "Engine/Engine.h"
#include "Engine/Core/Console.h"
#include "Game/Game.h"

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
		GeneratedReflectionData::RegisterReflectionData(engine);
		
		Game game{};
		
#if EDITOR
		Editor editor{};
		editor.SetGameTickFunction(Game::StaticTick);
		Engine::SetExternalTickFunction(Editor::StaticTick);
#else
		Engine::SetExternalTickFunction(Game::StaticTick);
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
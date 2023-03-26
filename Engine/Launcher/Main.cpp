#include <iostream>
#include "Engine/Engine.h"

#if EDITOR
#include <Editor/Editor.h>
#endif

int main(int argc, char** argv)
{
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
	catch(const std::exception& e)
	{
		std::string s = "CRASH due to: ";
		s += e.what();
		LOG_ERROR(s);
		return 1;
	}
}
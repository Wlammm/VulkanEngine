#include "EnginePch.h"
#include "Engine.h"

#include "Utils/Time.h"
#include <iostream>
#include "Windows/WindowHandler.h"

Engine::Engine(const EngineProperties inEngineProperties)
{
	myEngineProperties = inEngineProperties;
	instance = this;
	WindowHandler::Create();
}

Engine::~Engine()
{
	WindowHandler::Destroy();
	instance = nullptr;
}

void Engine::Tick()
{
	Time::Tick();
	WindowHandler::Tick();

	std::cout << Time::GetDeltaTime() << std::endl;
}

bool Engine::ShouldRun() const
{
	return myIsRunning;
}

void Engine::SetIsRunning(const bool inIsRunning)
{
	instance->myIsRunning = inIsRunning;
}

const EngineProperties& Engine::GetEngineProperties()
{
	return instance->myEngineProperties;
}

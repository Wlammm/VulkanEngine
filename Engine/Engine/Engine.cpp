#include "Engine.h"
#include "Time.h"
#include <iostream>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::Tick()
{
	Time::Tick();

	std::cout << Time::GetDeltaTime() << std::endl;
}

bool Engine::ShouldRun() const
{
	return myIsRunning;
}
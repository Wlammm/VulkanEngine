#include "EnginePch.h"
#include "Engine.h"

#include "Utils/Time.h"
#include "Windows/WindowHandler.h"
#include "Rendering/VulkanContext.h"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties { inEngineProperties }
{
	myInstance = this;

	myConsole = new Console();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
}

Engine::~Engine()
{
	del(myVulkanContext);
	del(myWindowHandler);
	del(myConsole);

	myInstance = nullptr;
}

void Engine::Tick()
{
	Time::Tick();
	myWindowHandler->Tick();
}

bool Engine::ShouldRun() const
{
	return myIsRunning;
}

void Engine::SetIsRunning(const bool inIsRunning)
{
	myInstance->myIsRunning = inIsRunning;
}

const EngineProperties& Engine::GetEngineProperties()
{
	return myInstance->myEngineProperties;
}

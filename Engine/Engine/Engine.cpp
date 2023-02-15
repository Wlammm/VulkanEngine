#include "EnginePch.h"
#include "Engine.h"

#include "Utils/Time.h"
#include "Windows/WindowHandler.h"
#include "Rendering/VulkanContext.h"
#include "Rendering/VulkanRenderer.h"
#include "ECS/SystemDispatcher.h"
#include "ECS/TEstSystem.h"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties { inEngineProperties }
{
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	myConsole = new Console();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	myRenderer = new VulkanRenderer();
	mySystemDispatcher = new SystemDispatcher();

	auto testsystem = new TestSystem(3);
	int a = 10;
}

Engine::~Engine()
{
	del(mySystemDispatcher);
	del(myRenderer);
	del(myVulkanContext);
	del(myWindowHandler);
	del(myConsole);

	myInstance = nullptr;
}

void Engine::Tick()
{
	Time::Tick();
	myWindowHandler->Tick();

	VulkanContext::BeginFrame();

	myRenderer->Tick();

	VulkanContext::EndFrame();
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

const SystemDispatcher& Engine::GetSystemDispatcher()
{
	return *myInstance->mySystemDispatcher;
}

const World& Engine::GetWorld()
{
	return *myInstance->myWorld;
}

void Engine::SetWorld(World* inWorld)
{
	myInstance->myWorld = inWorld;
}

module;
#include "Core/Time.h"
#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"
#include "ECS/SystemDispatcher.h"
#include "ECS/Systems/RenderSystem.h"
#include "Core\Input.h"

#include "Containers/List.hpp"
#include "Core/EngineDefines.hpp"
#include "Core/Console.h"

module Engine;

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties{ inEngineProperties }
{
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	myConsole = new Console();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	mySystemDispatcher = new SystemDispatcher();

	CreateSystems();
}

Engine::~Engine()
{
	del(mySystemDispatcher);
	del(myVulkanContext);
	del(myWindowHandler);
	del(myConsole);

	myInstance = nullptr;
}

void Engine::Tick()
{
	Time::Tick();
	myWindowHandler->Tick();

	if (Input::IsKeyDown(KeyCode::Escape))
		PostQuitMessage(0);

	VulkanContext::BeginFrame();

	mySystemDispatcher->DispatchSystems();

	VulkanContext::EndFrame();
	Input::EndFrame();
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

void Engine::CreateSystems()
{
	mySystemDispatcher->AddSystem<RenderSystem>();
}

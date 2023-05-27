#include "EnginePch.h"
#include "Engine.h"

#include "Core/Time.h"
#include "Core/Input.h"
#include "Containers/List.hpp"

#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"

#include "ECS/SystemDispatcher.h"
#include "ECS/Systems/RenderSystem.h"

#include "World/World.h"
#include "ECS/Systems/CameraSystem.h"
#include "Vulkan/VulkanImGui.h"

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

	VulkanImGui::Start();

	SetWorld(new World());
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

#if EDITOR
	myEditorTick();
#endif

	mySystemDispatcher->DispatchSystems();

	VulkanContext::EndFrame();
	Input::EndFrame();

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
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

const WindowHandler& Engine::GetWindowHandler()
{
	return *myInstance->myWindowHandler;
}

World& Engine::GetWorld()
{
	return *myInstance->myWorld;
}

void Engine::SetWorld(World* inWorld)
{
	myInstance->myWorld = inWorld;
}

Vec2ui Engine::GetRenderResolution()
{
	return { myInstance->myVulkanContext->GetSwapChain().GetWidth(), myInstance->myVulkanContext->GetSwapChain().GetHeight() };
}

#if EDITOR
void Engine::SetEditorTickFunction(const std::function<void()> inEditorTickFunction)
{
	myInstance->myEditorTick = inEditorTickFunction;
}
#endif

void Engine::CreateSystems()
{
	mySystemDispatcher->AddSystem<RenderSystem>();
	mySystemDispatcher->AddSystem<CameraSystem>();
}

#include "EnginePch.h"
#include "Engine.h"

#include "Core/Time.h"
#include "Core/Input.h"
#include "Containers/List.hpp"

#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"

#include "ECS/SystemDispatcher.h"
#include "ECS/Systems/RenderSystem.h"

#include "World/World.h"
#include "ECS/Systems/CameraSystem.h"
#include "Events/EventHandler.h"
#include "Assets/AssetRegistry.h"
#include "Core/ThreadPool.h"
#include "Core/Filewatcher.h"
#include "Utils/ThreadUtils.hpp"

#include "Tracy/tracy/Tracy.hpp"
#include "ECS/Systems/PointLightSystem.h"
#include "Core/AutoInitManager.h"
#include "Delegates/Delegate.hpp"
#include "ECS/Systems/DirectionalLightSystem.h"
#include "ECS/Systems/StaticMeshSystem.h"
#include "Rendering/BindlessResourceSystem.h"
#include "Utils/Debug.h"
//#include "Delegates/MulticastDelegate.hpp"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties{ inEngineProperties }
{
	ThreadUtils::NameThread(GetCurrentThread(), L"Main thread");
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	myPostMaster = new EventHandler();
	myConsole = new Console();
	myThreadPool = new ThreadPool();
	myFilewatcher = new Filewatcher();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	myAssetRegistry = new AssetRegistry();
	mySystemDispatcher = new SystemDispatcher();

	// This might cause issues in the future.
	CreateSystems();

	SetWorld(new World());
}

Engine::~Engine()
{
	del(myWorld);
	del(mySystemDispatcher);
	del(myAssetRegistry);
	del(myVulkanContext);
	del(myWindowHandler);
	del(myFilewatcher);
	del(myThreadPool);
	del(myConsole);
	del(myPostMaster);

	myInstance = nullptr;
}

void Engine::Tick()
{
	FrameMark;
	ZoneScoped;
	Time::Tick();
	AutoInitManager::Tick();
	
#if !TRACY_ENABLE // Disable fps limit when we're profiling.
	constexpr double targetFPS = 144.f;
	long long sleepTimeMicroseconds = static_cast<long long>((1.0 / targetFPS - Time::GetDeltaTime()) * 1000000);
	std::this_thread::sleep_for(std::chrono::microseconds(sleepTimeMicroseconds));
#endif

	myWindowHandler->Tick();

	if (Input::IsKeyDown(KeyCode::Escape))
		PostQuitMessage(0);

	myVulkanContext->BeginFrame();
	myFilewatcher->FlushChanges();
#if EDITOR
	myEditorTick();
#endif

	mySystemDispatcher->DispatchSystems();

	myVulkanContext->EndFrame();
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

EventHandler& Engine::GetEventHandler()
{
	return *myInstance->myPostMaster;
}

AssetRegistry& Engine::GetAssetRegistry()
{
	return *myInstance->myAssetRegistry;
}

ThreadPool& Engine::GetThreadPool()
{
	return *myInstance->myThreadPool;
}

Filewatcher& Engine::GetFilewatcher()
{
	return *myInstance->myFilewatcher;
}

World& Engine::GetWorld()
{
	return *myInstance->myWorld;
}

void Engine::SetWorld(World* inWorld)
{
	myInstance->myWorld = inWorld;
}

glm::vec2 Engine::GetRenderResolution()
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
	mySystemDispatcher->AddSystem<CameraSystem>();
	mySystemDispatcher->AddSystem<PointLightSystem>();
	mySystemDispatcher->AddSystem<DirectionalLightSystem>();
	mySystemDispatcher->AddSystem<RenderSystem>();
	mySystemDispatcher->AddSystem<StaticMeshSystem>();
	mySystemDispatcher->AddSystem<BindlessResourceSystem>();
}

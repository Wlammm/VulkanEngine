#include "EnginePch.h"
#include "Engine.h"

#include "Core/Time.h"
#include "Core/Input.h"

#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"

#include "World/World.h"
#include "Events/EventHandler.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Core/ThreadPool.h"
#include "Core/Filewatcher.h"
#include "Utils/ThreadUtils.hpp"

#include "Tracy/tracy/Tracy.hpp"
#include "Core/AutoInitManager.h"
#include "Coroutines/CoroutineManager.h"
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/MeshSystem.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/TextureSystem.h"
#include "Rendering/VertexBufferSystem.h"
#include "Subsystem/SystemManager.h"
#include "Vulkan/ObjectSystem.h"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties{ inEngineProperties }
{
	
	
	ThreadUtils::NameThread(GetCurrentThread(), L"Main thread");
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	myPostMaster = new EventHandler();
	myConsole = new Console();
	myThreadPool = new ThreadPool();
	CoroutineManager coroutine{};
	myFilewatcher = new Filewatcher();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	myAssetRegistry = new AssetRegistry();
	mySystemManager = new SystemManager();
	CreateSystems();

	SetWorld(new World());
}

Engine::~Engine()
{
	del(myWorld);
	del(mySystemManager);
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

	myWorld->Update();
	
	TickNextFrame.Invoke();
	TickNextFrame.Clear();
	
	GetEngineSystem<VertexBufferSystem>().Tick();
	GetEngineSystem<IndexBufferSystem>().Tick();
	GetEngineSystem<TextureSystem>().Tick();
	GetEngineSystem<MeshSystem>().Tick();
	GetEngineSystem<RenderSystem>().Tick();
	
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
	mySystemManager->AddSystem<RenderSystem>();
	mySystemManager->AddSystem<TextureSystem>();
	mySystemManager->AddSystem<MeshSystem>();
	mySystemManager->AddSystem<ObjectSystem>();
	mySystemManager->AddSystem<IndexBufferSystem>();
	mySystemManager->AddSystem<VertexBufferSystem>();

	mySystemManager->InitAllSystems();
}
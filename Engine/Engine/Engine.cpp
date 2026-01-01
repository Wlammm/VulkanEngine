#include "EnginePch.h"
#include "Engine.h"

#include "Core/Time.h"
#include "Core/Input.h"

#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"

#include "World/World.h"
#include "AssetRegistry/AssetRegistry.h"
#include "AssetRegistry/AssetUtils.h"
#include "Core/ThreadPool.h"
#include "Core/Filewatcher.h"
#include "Utils/ThreadUtils.hpp"

#include "Tracy/tracy/Tracy.hpp"
#include "Core/AutoInitManager.h"
#include "Physics/PhysicsSystem.h"
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/MeshSystem.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/TextureSystem.h"
#include "Rendering/VertexBufferSystem.h"
#include "Serialization/TypeSerializers/TypeSerializer.h"
#include "System/SystemManager.hpp"
#include "Systems/PointLightSystem.h"
#include "Utils/Debug.h"
#include "Vulkan/GPUSceneSystem.h"
#include "Vulkan/Staging/StagingSystem.h"
#include "World/GameWorld.h"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties{ inEngineProperties }
{
	ThreadUtils::NameThread(GetCurrentThread(), L"Main thread");
	ThreadUtils::RegisterThisThreadAsMainThread();
	
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	if(GetEngineProperties().HasStartupArgument("-waitfordebugger"))
	{
		while(!IsDebuggerPresent())
		{
			Sleep(10);		
		}
		__debugbreak();
	}
	
	TypeSerializer::RegisterSerializers();

	AssetUtils::Start();
	
	PhysicsSystem::CreateStaticObjects();

	std::unique_ptr<int> somePtr;

	myThreadPool = MakeUnique<ThreadPool>();
	myFilewatcher = MakeUnique<Filewatcher>();
	myWindowHandler = MakeUnique<WindowHandler>();
	myVulkanContext = MakeUnique<VulkanContext>();
	mySystemManager = MakeUnique<SystemManager<System>>();
	CreateSystems();

#if !EDITOR
	SetWorld(AssetRegistry::Get()->GetAssetSynchronous<GameWorld>("Assets/Worlds/Sponza.world"));
#endif
}

Engine::~Engine()
{
	myWorld.reset();
	mySystemManager.Reset();
	myVulkanContext.Reset();
	myWindowHandler.Reset();
	myFilewatcher.Reset();
	myThreadPool.Reset();

	PhysicsSystem::DestoryStaticObjects();

	myInstance = nullptr;
}

void Engine::Tick()
{
	myFrameIndex++;
	FrameMark;
	ZoneScoped;
	Time::Tick();
	
#if !TRACY_ENABLE // Disable fps limit when we're profiling.
	constexpr double targetFPS = 144.f;
	long long sleepTimeMicroseconds = static_cast<long long>((1.0 / targetFPS - Time::GetDeltaTime()) * 1000000);
	std::this_thread::sleep_for(std::chrono::microseconds(sleepTimeMicroseconds));
#endif

	myWindowHandler->Tick();

	if (Input::IsKeyDown(KeyCode::Escape))
		PostQuitMessage(0);

	{
		ZoneScopedN("TicksNextFrame");
		// Move the data so that we can bind the same function for the next frame inside this invocation.
		MulticastDelegate<void()> ticksThisFrame = std::move(TickNextFrame);
		TickNextFrame.Clear();
		ticksThisFrame.Invoke();
	}
	
	Debug::DrawSphere(glm::vec3(0, 0, 0), 100);
	
	AutoInitManager::Tick();
	
	myVulkanContext->BeginFrame();
	myFilewatcher->FlushChanges();
	
	GetEngineSystem<StagingSystem>().Tick();
	GetEngineSystem<GPUSceneSystem>().Tick();
	
	myWorld->DoTick();

	myExternalTickFunction();
	
	GetEngineSystem<TextureSystem>().Tick();
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

uint Engine::GetFrameIndex()
{
	return myInstance->myFrameIndex;
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

ThreadPool& Engine::GetThreadPool()
{
	return *myInstance->myThreadPool;
}

Filewatcher& Engine::GetFilewatcher()
{
	return *myInstance->myFilewatcher;
}

SharedPtr<World> Engine::GetWorld()
{
	return myInstance->myWorld;
}

WeakPtr<World> Engine::GetWeakWorld()
{
	return myInstance->myWorld;
}

void Engine::SetWorld(SharedPtr<World> inWorld)
{
	SharedPtr<World> previousWorld = myInstance->myWorld;
	myInstance->myWorld = inWorld;
	OnWorldChangedDelegate.Invoke(previousWorld);
}

glm::vec2 Engine::GetRenderResolution()
{
	return { myInstance->myVulkanContext->GetSwapChain().GetWidth(), myInstance->myVulkanContext->GetSwapChain().GetHeight() };
}

void Engine::SetExternalTickFunction(const std::function<void()> inExternalTickFunction)
{
	myInstance->myExternalTickFunction = inExternalTickFunction;
}

void Engine::CreateSystems()
{
	mySystemManager->AddSystem<AssetRegistry>();
	mySystemManager->AddSystem<StagingSystem>();
	mySystemManager->AddSystem<TextureSystem>();
	mySystemManager->AddSystem<MeshSystem>();
	mySystemManager->AddSystem<GPUSceneSystem>();
	mySystemManager->AddSystem<IndexBufferSystem>();
	mySystemManager->AddSystem<VertexBufferSystem>();
	mySystemManager->AddSystem<RenderSystem>();
	mySystemManager->AddSystem<PointLightSystem>();

	mySystemManager->InitAllSystems();
}

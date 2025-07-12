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
#include "Reflection/ReflectionSystem.h"
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/MeshSystem.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/TextureSystem.h"
#include "Rendering/VertexBufferSystem.h"
#include "System/SystemManager.hpp"
#include "Systems/PointLightSystem.h"
#include "Vulkan/GPUSceneSystem.h"
#include "Vulkan/Staging/StagingSystem.h"

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

	AssetRegistry::ScanAssetsFolder();
	
	myPostMaster = new EventHandler();
	myThreadPool = new ThreadPool();
	myFilewatcher = new Filewatcher();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	myAssetRegistry = new AssetRegistry();
	mySystemManager = new SystemManager<System>();
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
	del(myPostMaster);

	myInstance = nullptr;
}

void Engine::Tick()
{
#if 0 // Serialization testing.
	BinarySerializer serializer("SomeFile.format", BinarySerializer::Mode::Write);
	TestClass testClass;
	testClass.SomeField = 321534534;
	testClass.SomeFloat = 321321321.232154f;
	serializer.SerializeClass(testClass);
	serializer.Close();

	TestClass readClass;
	BinarySerializer reader("SomeFile.format", BinarySerializer::Mode::Read);
	reader.SerializeClass(readClass);
	reader.Close();
	exit(0);
#endif	

	myFrameIndex++;
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

	{
		ZoneScopedN("TicksNextFrame");
		// Move the data so that we can bind the same function for the next frame inside this invocation.
		MulticastDelegate<void()> ticksThisFrame = std::move(TickNextFrame);
		TickNextFrame.Clear();
		ticksThisFrame.Invoke();
	}
	
	myVulkanContext->BeginFrame();
	myFilewatcher->FlushChanges();
	
	GetEngineSystem<StagingSystem>().Tick();
	GetEngineSystem<GPUSceneSystem>().Tick();
	
	myWorld->Update();

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

void Engine::SetExternalTickFunction(const std::function<void()> inExternalTickFunction)
{
	myInstance->myExternalTickFunction = inExternalTickFunction;
}

void Engine::CreateSystems()
{
	mySystemManager->AddSystem<StagingSystem>();
	mySystemManager->AddSystem<RenderSystem>();
	mySystemManager->AddSystem<TextureSystem>();
	mySystemManager->AddSystem<MeshSystem>();
	mySystemManager->AddSystem<GPUSceneSystem>();
	mySystemManager->AddSystem<IndexBufferSystem>();
	mySystemManager->AddSystem<VertexBufferSystem>();
	mySystemManager->AddSystem<PointLightSystem>();

	mySystemManager->InitAllSystems();
}

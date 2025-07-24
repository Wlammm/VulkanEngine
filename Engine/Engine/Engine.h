#pragma once

#include "Delegates/MulticastDelegate.hpp"
#include "Engine/EngineProperties.hpp"
#include "System/SystemManager.hpp"

class World;
class ReflectionSystem;


class TestClass
{
public:
	META(SerializeField)
	float SomeFloat = 12.345f;
	
	META(SerializeField)
	int SomeField = 321;
};

class Engine
{
public:
	Engine(const EngineProperties inEngineProperties);
	~Engine();

	void Tick();

	bool ShouldRun() const;

	static uint GetFrameIndex();
	
	static void SetIsRunning(const bool inIsRunning);

	static const EngineProperties& GetEngineProperties();
	static const class WindowHandler& GetWindowHandler();
	static class AssetRegistry& GetAssetRegistry();
	static class ThreadPool& GetThreadPool();
	static class Filewatcher& GetFilewatcher();

	static SharedPtr<World> GetWorld();
	static WeakPtr<World> GetWeakWorld();
	static void SetWorld(SharedPtr<World> inWorld);

	template<typename SystemType>
	static SystemType& GetEngineSystem()
	{
		ZoneScoped;
		check(myInstance);
		check(myInstance->mySystemManager);
		
		return myInstance->mySystemManager->GetSystem<SystemType>();
	}

	static glm::vec2 GetRenderResolution();

	// Ticks right before rendering.
	inline static MulticastDelegate<void()> TickNextFrame;

	static void SetExternalTickFunction(const std::function<void()> inExternalTickFunction);

private:
	void CreateSystems();

private:
	uint myFrameIndex = 0;

	bool myIsRunning = true;

	EngineProperties myEngineProperties;

	UniquePtr<SystemManager<System>> mySystemManager;
	UniquePtr<ThreadPool> myThreadPool = nullptr;
	UniquePtr<Filewatcher> myFilewatcher = nullptr;
	UniquePtr<WindowHandler> myWindowHandler = nullptr;
	UniquePtr<class VulkanContext> myVulkanContext = nullptr;

	// This asset registry holds engine related data. If you need a game resource, use the worlds asset registry instead.
	UniquePtr<AssetRegistry> myAssetRegistry = nullptr;

	SharedPtr<World> myWorld = nullptr;

	std::function<void()> myExternalTickFunction = nullptr;

private:
	inline static Engine* myInstance = nullptr;
};

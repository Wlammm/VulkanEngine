#pragma once

#include "Delegates/MulticastDelegate.hpp"
#include "Engine/EngineProperties.hpp"
#include "System/SystemManager.hpp"

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
	static class EventHandler& GetEventHandler();
	static class AssetRegistry& GetAssetRegistry();
	static class ThreadPool& GetThreadPool();
	static class Filewatcher& GetFilewatcher();

	static class World& GetWorld();
	static void SetWorld(World* inWorld);

	template<typename SystemType>
	static SystemType& GetEngineSystem()
	{
		ZoneScoped;
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

	class SystemManager<System>* mySystemManager = nullptr;
	class EventHandler* myPostMaster = nullptr;
	class ThreadPool* myThreadPool = nullptr;
	class Filewatcher* myFilewatcher = nullptr;
	class WindowHandler* myWindowHandler = nullptr;
	class VulkanContext* myVulkanContext = nullptr;

	// This asset registry holds engine related data. If you need a game resource, use the worlds asset registry instead.
	class AssetRegistry* myAssetRegistry = nullptr;

	class World* myWorld = nullptr;

	std::function<void()> myExternalTickFunction = nullptr;

private:
	inline static Engine* myInstance = nullptr;
};
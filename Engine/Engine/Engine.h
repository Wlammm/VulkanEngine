#pragma once

#include "Engine/EngineProperties.hpp"
#include "Subsystem/SystemManager.h"

class Engine
{
public:
	Engine(const EngineProperties inEngineProperties);
	~Engine();

	void Tick();

	bool ShouldRun() const;
	
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
		return myInstance->mySystemManager->GetSystem<SystemType>();
	}

	static glm::vec2 GetRenderResolution();

#if EDITOR
	static void SetEditorTickFunction(const std::function<void()> inEditorTickFunction);
#endif

private:
	void CreateSystems();

private:
	bool myIsRunning = true;

	EngineProperties myEngineProperties;

	class SystemManager* mySystemManager = nullptr;
	class EventHandler* myPostMaster = nullptr;
	class Console* myConsole = nullptr;
	class ThreadPool* myThreadPool = nullptr;
	class Filewatcher* myFilewatcher = nullptr;
	class WindowHandler* myWindowHandler = nullptr;
	class VulkanContext* myVulkanContext = nullptr;

	// This asset registry holds engine related data. If you need a game resource, use the worlds asset registry instead.
	class AssetRegistry* myAssetRegistry = nullptr;

	class World* myWorld = nullptr;

#if EDITOR
	std::function<void()> myEditorTick = nullptr;
#endif

private:
	inline static Engine* myInstance = nullptr;
};
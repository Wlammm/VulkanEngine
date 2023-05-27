#pragma once

#include "Engine/EngineProperties.hpp"
#include "ECS/SystemDispatcher.h"

class Engine
{
public:
	Engine(const EngineProperties inEngineProperties);
	~Engine();

	void Tick();

	bool ShouldRun() const;
	
	static void SetIsRunning(const bool inIsRunning);
	static const EngineProperties& GetEngineProperties();

	static const class SystemDispatcher& GetSystemDispatcher();

	static const class WindowHandler& GetWindowHandler();

	static class World& GetWorld();
	static void SetWorld(World* inWorld);

	static Vec2ui GetRenderResolution();

	template<typename T>
	static T* GetSystem()
	{
		return myInstance->mySystemDispatcher->GetSystem<T>();
	}

#if EDITOR
	static void SetEditorTickFunction(const std::function<void()> inEditorTickFunction);
#endif

private:
	void CreateSystems();

private:
	bool myIsRunning = true;

	EngineProperties myEngineProperties;

	class Console* myConsole = nullptr;
	class WindowHandler* myWindowHandler = nullptr;
	class VulkanContext* myVulkanContext = nullptr;
	class SystemDispatcher* mySystemDispatcher = nullptr;

	class World* myWorld = nullptr;

#if EDITOR
	std::function<void()> myEditorTick = nullptr;
#endif

private:
	inline static Engine* myInstance = nullptr;
};
#pragma once

#include "Engine/EngineProperties.hpp"

class Engine
{
public:
	Engine(const EngineProperties inEngineProperties);
	~Engine();

	void Tick();

	bool ShouldRun() const;
	
	static void SetIsRunning(const bool inIsRunning);
	static const EngineProperties& GetEngineProperties();

private:
	bool myIsRunning = true;

	EngineProperties myEngineProperties;

	class Console* myConsole = nullptr;
	class WindowHandler* myWindowHandler = nullptr;
	class VulkanContext* myVulkanContext = nullptr;

private:
	inline static Engine* myInstance = nullptr;
};
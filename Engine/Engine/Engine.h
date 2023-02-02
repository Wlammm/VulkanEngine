#pragma once

#include "EngineProperties.hpp"

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

private:
	inline static Engine* instance = nullptr;
};
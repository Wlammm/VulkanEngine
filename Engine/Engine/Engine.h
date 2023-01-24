#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void Tick();

	bool ShouldRun() const;
	
	static void SetIsRunning(const bool inIsRunning);

private:
	bool myIsRunning = true;

private:
	inline static Engine* instance = nullptr;
};
#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void Tick();

	bool ShouldRun() const;

private:
	bool myIsRunning = true;
};
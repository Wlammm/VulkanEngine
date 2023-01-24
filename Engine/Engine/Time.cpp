#include "EnginePch.h"
#include "Time.h"
#include <chrono>

void Time::Tick()
{
	static std::chrono::high_resolution_clock timer;
	static std::chrono::high_resolution_clock::time_point lastTime;
	std::chrono::high_resolution_clock::time_point now = timer.now();
	myDeltaTime = (now - lastTime).count() / 1000000000.0f * myTimeScale;
	lastTime = now;
	
	myDeltaTime = std::clamp(myDeltaTime, 0.0f, 0.5f);
}

float Time::GetDeltaTime()
{
	return myDeltaTime;
}

void Time::SetTimeScale(const float inTimeScale)
{
	myTimeScale = inTimeScale;
}
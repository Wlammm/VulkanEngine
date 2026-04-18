#include "EnginePch.h"
#include "Time.h"
#include <chrono>

void Time::Tick()
{
	static std::chrono::high_resolution_clock timer;
	static std::chrono::high_resolution_clock::time_point lastTime;
	std::chrono::high_resolution_clock::time_point now = timer.now();
	myDeltaTime = (now - lastTime).count() / 1000000000.0f;
	lastTime = now;

	// TODO: Should this clamp be here? 
	myDeltaTime = std::clamp(myDeltaTime, 0.0f, 0.5f);
	myTimeSinceStart += myDeltaTime;
}

float Time::GetDeltaTime()
{
	return myDeltaTime * myTimeScale;
}

float Time::GetUnscaledDeltaTime()
{
	return myDeltaTime;
}

void Time::SetTimeScale(const float inTimeScale)
{
	myTimeScale = inTimeScale;
}

double Time::GetSeconds()
{
	static std::chrono::high_resolution_clock timer;
	return timer.now().time_since_epoch().count() / 1000000000.0;
}

float Time::GetTimeSinceStart()
{
	return myTimeSinceStart;
}

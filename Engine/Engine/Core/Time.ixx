export module Time;

export class Time
{
public:
	static void Tick();

	static float GetDeltaTime();
	static void SetTimeScale(const float inTimeScale);

	static double GetSeconds();

private:
	inline static float myDeltaTime = 0.f;
	inline static float myTimeScale = 1.f;
};
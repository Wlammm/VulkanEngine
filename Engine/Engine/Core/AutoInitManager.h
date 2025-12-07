#pragma once

class AutoInit;

class AutoInitManager
{
public:

	static void AddInitObject(AutoInit* inObject);
	static void RemoveInitObject(AutoInit* inObject);

	static void Tick();

private:
	inline static List<AutoInit*> myObjectsToInit;
};
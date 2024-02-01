#include "EnginePch.h"
#include "AutoInitManager.h"
#include "AutoInit.h"

void AutoInitManager::AddInitObject(AutoInit* inObject)
{
	myObjectsToInit.Add(inObject);
}

void AutoInitManager::Tick()
{
	for(AutoInit* init : myObjectsToInit)
	{
		init->Init();
	}
	myObjectsToInit.Clear();
}

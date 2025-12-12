#include "EnginePch.h"
#include "AutoInitManager.h"
#include "AutoInit.h"

void AutoInitManager::AddInitObject(AutoInit* inObject)
{
	myObjectsToInit.Add(inObject);
}

void AutoInitManager::RemoveInitObject(AutoInit* inObject)
{
	myObjectsToInit.Remove(inObject);
}

void AutoInitManager::Tick()
{
	myObjectsToInit.Lock();
	
	for(AutoInit* init : myObjectsToInit)
	{
		init->Init();
		init->myHasInited = true;
	}
	myObjectsToInit.Clear();
	
	myObjectsToInit.Unlock();
}

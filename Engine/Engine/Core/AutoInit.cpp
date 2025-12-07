#include "EnginePch.h"
#include "AutoInit.h"
#include "AutoInitManager.h"

AutoInit::AutoInit()
{
	AutoInitManager::AddInitObject(this);
}

AutoInit::~AutoInit()
{
	if (!myHasInited)
		AutoInitManager::RemoveInitObject(this);
}

#include "EnginePch.h"
#include "AutoInit.h"
#include "AutoInitManager.h"

AutoInit::AutoInit()
{
	AutoInitManager::AddInitObject(this);
}

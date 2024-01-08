#include "EnginePch.h"
#include "OnScopeExit.h"

OnScopeExit::OnScopeExit(std::function<void()> inOnScopeExit)
{
	myOnScopeExit = inOnScopeExit;
}

OnScopeExit::~OnScopeExit()
{
	if (myOnScopeExit != nullptr)
		myOnScopeExit();
}
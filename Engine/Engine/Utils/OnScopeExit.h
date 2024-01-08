#pragma once

class OnScopeExit
{
public:
	OnScopeExit() = delete;
	OnScopeExit(std::function<void()> inOnScopeExit);
	~OnScopeExit();
	

private:
	std::function<void()> myOnScopeExit = nullptr;
};
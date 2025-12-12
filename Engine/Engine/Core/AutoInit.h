#pragma once

/*
 * Be careful when using this as its always running on main thread as soon as the object is constructed. This is not necessarily what you want for assets as an example.
 */ 
class AutoInit
{
public:
	AutoInit();
	virtual ~AutoInit();

	virtual void Init() = 0;

private:
	friend class AutoInitManager;
	bool myHasInited = false;
};
#pragma once

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
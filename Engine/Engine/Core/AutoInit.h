#pragma once

class AutoInit
{
public:
	AutoInit();
	virtual ~AutoInit() = default;

	virtual void Init() = 0;
};
#pragma once

class Editor
{
public:
	Editor();
	~Editor();

private:
	inline static Editor* myInstance = nullptr;
};
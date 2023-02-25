#pragma once

#include <windows.h>

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	void Tick();
	
	static HWND GetHWND();
	static HINSTANCE GetHInstance();

private:
	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND myHWND;
	HINSTANCE myHInstance;

	inline static WindowHandler* myInstance = nullptr;
};
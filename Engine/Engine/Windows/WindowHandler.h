#pragma once

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	void Tick();
	
	static HWND GetHWND();
	static HINSTANCE GetHInstance();

	// Returns true and outputs the new scale if a DPI change is pending (consumes the flag).
	static bool ConsumeDpiChange(float& outDpiScale);

private:
	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND myHWND;
	HINSTANCE myHInstance;

	inline static WindowHandler* myInstance = nullptr;
	inline static bool myDpiChanged = false;
	inline static float myPendingDpiScale = 1.0f;
};
#pragma once

class WindowHandler
{
public:
	static void Create();
	static void Destroy();

	static void Tick();
	
private:
	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	inline static HWND myHWND;

};
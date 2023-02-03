#pragma once

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	void Tick();
	
private:
	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND myHWND;

	inline static WindowHandler* myInstance = nullptr;

};
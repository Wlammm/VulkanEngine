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

	// --- Custom (borderless) title bar support ---
	// Switches the OS window to borderless so the app can draw its own title bar.
	// Left/right/bottom edges stay resizable and the OS keeps snapping + min/max.
	static void EnableCustomTitleBar(bool inEnable);
	// Pixel height of the app-drawn title bar; used for caption hit-testing.
	static void SetTitleBarHeight(int inHeightPx);
	// Set each frame: true when the cursor is over an empty (draggable) part of the bar.
	static void SetTitleBarDragHovered(bool inHovered);

	static bool IsWindowMaximized();
	static void Minimize();
	static void ToggleMaximize();
	static void RequestClose();

private:
	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool IsMaximized(HWND hWnd);

private:
	HWND myHWND;
	HINSTANCE myHInstance;

	inline static WindowHandler* myInstance = nullptr;
	inline static bool myDpiChanged = false;
	inline static float myPendingDpiScale = 1.0f;

	inline static bool myUseCustomTitleBar = false;
	inline static int myTitleBarHeight = 32;
	inline static bool myTitleBarDragHovered = false;
};
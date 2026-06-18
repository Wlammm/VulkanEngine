#include "EnginePch.h"
#include "Engine/Windows/WindowHandler.h"
#include "Engine/Engine.h"
#include "Engine/Core/Input.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowHandler::WindowHandler()
{
	check(!myInstance && "Only one instance of this class can exist at once.");
	myInstance = this;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	WNDCLASS wndClass{};
	wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WindowHandler::WndProc;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.lpszClassName = Engine::GetEngineProperties().Title.c_str();
	wndClass.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(10, 10, 10));
	RegisterClass(&wndClass);

	// -1920 here as I want the window to start on my left display. TODO: Change to zero for packaged games.
	myHWND = CreateWindow(Engine::GetEngineProperties().Title.c_str(), Engine::GetEngineProperties().Title.c_str(), WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE, 100, 100, 1600, 900, nullptr, nullptr, nullptr, nullptr);

	myHInstance = GetModuleHandle(NULL);
}

WindowHandler::~WindowHandler()
{
	DestroyWindow(myHWND);
	myInstance = nullptr;
}

void WindowHandler::Tick()
{
	MSG windowsMessage{};
	while (PeekMessage(&windowsMessage, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&windowsMessage);
		DispatchMessage(&windowsMessage);

		if (windowsMessage.message == WM_QUIT)
		{
			Engine::SetIsRunning(false);
		}
	}
}

HWND WindowHandler::GetHWND()
{
	return myInstance->myHWND;
}

bool WindowHandler::ConsumeDpiChange(float& outDpiScale)
{
	if (!myDpiChanged)
		return false;
	outDpiScale = myPendingDpiScale;
	myDpiChanged = false;
	return true;
}

HINSTANCE WindowHandler::GetHInstance()
{
	return myInstance->myHInstance;
}

bool WindowHandler::IsMaximized(HWND hWnd)
{
	WINDOWPLACEMENT placement{};
	placement.length = sizeof(WINDOWPLACEMENT);
	if (GetWindowPlacement(hWnd, &placement))
		return placement.showCmd == SW_SHOWMAXIMIZED;
	return false;
}

void WindowHandler::EnableCustomTitleBar(bool inEnable)
{
	myUseCustomTitleBar = inEnable;
	if (myInstance && myInstance->myHWND)
	{
		// Force the non-client area to be recalculated so WM_NCCALCSIZE takes effect.
		SetWindowPos(myInstance->myHWND, nullptr, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void WindowHandler::SetTitleBarHeight(int inHeightPx)
{
	myTitleBarHeight = inHeightPx;
}

void WindowHandler::SetTitleBarDragHovered(bool inHovered)
{
	myTitleBarDragHovered = inHovered;
}

bool WindowHandler::IsWindowMaximized()
{
	return (myInstance && myInstance->myHWND) ? IsMaximized(myInstance->myHWND) : false;
}

void WindowHandler::Minimize()
{
	if (myInstance && myInstance->myHWND)
		ShowWindow(myInstance->myHWND, SW_MINIMIZE);
}

void WindowHandler::ToggleMaximize()
{
	if (!myInstance || !myInstance->myHWND)
		return;
	ShowWindow(myInstance->myHWND, IsMaximized(myInstance->myHWND) ? SW_RESTORE : SW_MAXIMIZE);
}

void WindowHandler::RequestClose()
{
	if (myInstance && myInstance->myHWND)
		PostMessage(myInstance->myHWND, WM_CLOSE, 0, 0);
}

LRESULT WindowHandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	// Borderless title bar: take over the non-client area so the app draws its own bar.
	if (myUseCustomTitleBar)
	{
		if (uMsg == WM_NCCALCSIZE && wParam == TRUE)
		{
			// When maximized, Windows inflates the window by the frame size on every
			// edge; inset the client back so nothing is clipped off-screen. When
			// restored, the client spans the entire window (no caption, no frame).
			if (IsMaximized(hWnd))
			{
				NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				RECT& rect = params->rgrc[0];

				const UINT dpi = GetDpiForWindow(hWnd);
				const int frameX = GetSystemMetricsForDpi(SM_CXFRAME, dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
				const int frameY = GetSystemMetricsForDpi(SM_CYFRAME, dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

				rect.left += frameX;
				rect.right -= frameX;
				rect.top += frameY;
				rect.bottom -= frameY;
			}
			return 0;
		}

		if (uMsg == WM_NCHITTEST)
		{
			const int x = static_cast<short>(LOWORD(lParam));
			const int y = static_cast<short>(HIWORD(lParam));

			RECT rc{};
			GetWindowRect(hWnd, &rc);

			const UINT dpi = GetDpiForWindow(hWnd);
			const int borderX = GetSystemMetricsForDpi(SM_CXFRAME, dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
			const int borderY = GetSystemMetricsForDpi(SM_CYFRAME, dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

			// Resize edges (disabled while maximized).
			if (!IsMaximized(hWnd))
			{
				const bool left = x < rc.left + borderX;
				const bool right = x >= rc.right - borderX;
				const bool top = y < rc.top + borderY;
				const bool bottom = y >= rc.bottom - borderY;

				if (top && left)     return HTTOPLEFT;
				if (top && right)    return HTTOPRIGHT;
				if (bottom && left)  return HTBOTTOMLEFT;
				if (bottom && right) return HTBOTTOMRIGHT;
				if (left)            return HTLEFT;
				if (right)           return HTRIGHT;
				if (top)             return HTTOP;
				if (bottom)          return HTBOTTOM;
			}

			// Title bar: draggable empty space acts as the caption; over a widget it
			// stays client so ImGui (menus / window buttons) handles the click.
			if (y < rc.top + myTitleBarHeight)
				return myTitleBarDragHovered ? HTCAPTION : HTCLIENT;

			return HTCLIENT;
		}
	}

	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (uMsg == WM_DPICHANGED)
	{
		myPendingDpiScale = LOWORD(wParam) / 96.0f;
		myDpiChanged = true;

		const RECT* suggestedRect = reinterpret_cast<const RECT*>(lParam);
		SetWindowPos(hWnd, nullptr,
			suggestedRect->left, suggestedRect->top,
			suggestedRect->right - suggestedRect->left,
			suggestedRect->bottom - suggestedRect->top,
			SWP_NOZORDER | SWP_NOACTIVATE);
		return 0;
	}

	if (Input::UpdateEvents(uMsg, wParam, lParam))
		return 0;

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

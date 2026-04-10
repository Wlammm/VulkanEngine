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

LRESULT WindowHandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

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

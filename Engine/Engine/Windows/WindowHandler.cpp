#include "Windows/WindowHandler.h"
import Engine;
#include "Core\Input.h"
#include "Core/EngineDefines.hpp"

WindowHandler::WindowHandler()
{
	check(!myInstance && "Only one instance of this class can exist at once.");
	myInstance = this;

	WNDCLASS wndClass{};
	wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WindowHandler::WndProc;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.lpszClassName = Engine::GetEngineProperties().Title.c_str();
	RegisterClass(&wndClass);

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

HINSTANCE WindowHandler::GetHInstance()
{
	return myInstance->myHInstance;
}

LRESULT WindowHandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (Input::UpdateEvents(uMsg, wParam, lParam))
		return 0;

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

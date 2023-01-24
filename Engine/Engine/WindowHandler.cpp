#include "EnginePch.h"
#include "WindowHandler.h"
#include "Engine.h"

void WindowHandler::Create()
{
	std::wstring title = L"Default Title";

	WNDCLASS wndClass{};
	wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WindowHandler::WndProc;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.lpszClassName = title.c_str();
	RegisterClass(&wndClass);

	myHWND = CreateWindow(title.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE, 100, 100, 1600, 900, nullptr, nullptr, nullptr, nullptr);
}

void WindowHandler::Destroy()
{

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

LRESULT WindowHandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

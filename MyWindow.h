#include "stdafx.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

struct MyWindow 
{
	HWND hWnd;

	MyWindow(HINSTANCE hInstance) 
	{
		WNDCLASSEX wc;

		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WindowProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"WindowClass";

		RegisterClassEx(&wc);

		RECT desktop;
		HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop); // Get the size of screen to the variable desktop

		// calculate extra space needed for window border and header
		RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		hWnd = CreateWindowEx(NULL,
							  wc.lpszClassName,
							  L"DirectX 11",
							  WS_OVERLAPPEDWINDOW,
							  ((desktop.right - desktop.left) - (wr.right - wr.left)) / 2, // center window
							  ((desktop.bottom - desktop.top) - (wr.bottom - wr.top)) / 2, // center window
							  (wr.right - wr.left), // window width including borders
							  (wr.bottom - wr.top), // window height including borders
							  NULL,
							  NULL,
							  hInstance,
							  NULL);
	}
};

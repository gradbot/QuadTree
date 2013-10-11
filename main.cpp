#include "stdafx.h"

#include "MyWindow.h"
#include "D3DSetup.h"
#include "MyShaders.h"
#include "MyScene.h"

// include the Direct3D Library files
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3dcompiler.lib")

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
		case WM_KEYDOWN: 
            switch (wParam) 
            { 
			case VK_ESCAPE: 
	            PostQuitMessage(0);
                return 0; 
			}
	}

    return DefWindowProc (hWnd, message, wParam, lParam);
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_CRT_DF;
    tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
	*/

	MyWindow window(hInstance);
	D3DSetup d3d(window.hWnd);
	MyShaders shaders(d3d.dev, d3d.devcon);
	MyScene scene;

    ShowWindow(window.hWnd, nCmdShow);

	// deep blue
	float backgroundColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    MSG msg;
	int frame = 1;

    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

		// clear the back buffer to a deep blue
		d3d.devcon->ClearRenderTargetView(d3d.backbuffer, backgroundColor);

		scene.update(d3d.dev, d3d.devcon, frame);
		scene.render(d3d.devcon);

		// switch the back buffer and the front buffer, aka show what we just rendered
		d3d.swapchain->Present(1, 0);
		++frame;
    }

    return msg.wParam;
}

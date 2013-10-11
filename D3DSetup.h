#include "stdafx.h"

struct D3DSetup 
{
	//ComPtr<>
	IDXGISwapChain *swapchain;             // swap chain interface
	ID3D11Device *dev;                     // Direct3D device interface
	ID3D11DeviceContext *devcon;           // Direct3D device context
	ID3D11RenderTargetView *backbuffer;    // back buffer

	D3DSetup(HWND hWnd) 
	{
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1;                                   // one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
		scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
		scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
		scd.OutputWindow = hWnd;                               // the window to be used
		scd.SampleDesc.Count = 1;                              // how many multisamples
		scd.Windowed = TRUE;                                   // windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

		// create a device, device context and swap chain using the information in the scd struct
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);

		// get the address of the back buffer
		ID3D11Texture2D *pBackBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
		pBackBuffer->Release();

		// set the render target as the back buffer
		devcon->OMSetRenderTargets(1, &backbuffer, NULL);

		// Set the viewport
		D3D11_VIEWPORT viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0};

		devcon->RSSetViewports(1, &viewport);

		//// Set wireframe mode
		//{
		//	D3D11_RASTERIZER_DESC rasterDesc;
		//	ID3D11RasterizerState* m_rasterState;

		//	// Setup the raster description which will determine how and what polygons will be drawn.
		//	rasterDesc.AntialiasedLineEnable = false;
		//	rasterDesc.CullMode = D3D11_CULL_BACK;
		//	rasterDesc.DepthBias = 0;
		//	rasterDesc.DepthBiasClamp = 0.0f;
		//	rasterDesc.DepthClipEnable = true;
		//	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		//	rasterDesc.FrontCounterClockwise = false;
		//	rasterDesc.MultisampleEnable = false;
		//	rasterDesc.ScissorEnable = false;
		//	rasterDesc.SlopeScaledDepthBias = 0.0f;

		//	// Create the rasterizer state from the description we just filled out.
		//	dev->CreateRasterizerState(&rasterDesc, &m_rasterState);
		//	// Now set the rasterizer state.
		//	devcon->RSSetState(m_rasterState);
		//}
	}

	~D3DSetup()
	{
		// close and release all existing COM objects
		swapchain->Release();
		backbuffer->Release();
		dev->Release();
		devcon->Release();
	}
};
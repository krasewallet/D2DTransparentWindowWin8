#include "stdafx.h"
#include "CD3DWindow.h"
#include <Windows.h>
#include <dwmapi.h>

CD3DWindow::CD3DWindow(
	_In_opt_ HWND hWndParent,
	_In_ _U_RECT rect,
	_In_opt_z_ LPCTSTR szWindowName,
	_In_ DWORD dwStyle,
	_In_ DWORD dwExStyle,
	_In_ _U_MENUorID MenuOrID,
	_In_opt_ LPVOID lpCreateParam)
{
	m_hWnd = __super::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle | WS_EX_NOREDIRECTIONBITMAP, MenuOrID, lpCreateParam);

	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd);

	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HRESULT hr = S_OK;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_DRIVER_TYPE driverTypes[] =
	{
	  D3D_DRIVER_TYPE_HARDWARE,
	  D3D_DRIVER_TYPE_WARP,
	  D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
	  D3D_FEATURE_LEVEL_11_1,
	  D3D_FEATURE_LEVEL_11_0,
	  D3D_FEATURE_LEVEL_10_1,
	  D3D_FEATURE_LEVEL_10_0,
	  D3D_FEATURE_LEVEL_9_3,
	  D3D_FEATURE_LEVEL_9_2,
	  D3D_FEATURE_LEVEL_9_1
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		D3D_DRIVER_TYPE m_d3dDriverType = driverTypes[driverTypeIndex];
		
		hr = D3D11CreateDevice(
			nullptr,
			m_d3dDriverType,
			nullptr,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			&m_d3dFeatureLevel,
			&m_d3dContext
		);

		if (SUCCEEDED(hr))
			break;
	}
	m_d3dDevice->QueryInterface(__uuidof(m_dxgiDevice), reinterpret_cast<void**>(&m_dxgiDevice));
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(m_dxgifactory), reinterpret_cast<void **>(&m_dxgifactory));
	m_dxgifactory->CreateSwapChainForComposition(m_d3dDevice, &swapChainDesc, nullptr, &m_dxgiSwapChain);

	/*ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_d3dRenderTargetView);
	pBackBuffer->Release();*/

	// Create depth stencil texture
	//D3D11_TEXTURE2D_DESC depthDesc;
	//ZeroMemory(&depthDesc, sizeof(depthDesc));
	//depthDesc.Width = width;
	//depthDesc.Height = height;
	//depthDesc.MipLevels = 1;
	//depthDesc.ArraySize = 1;
	//depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthDesc.SampleDesc.Count = 1;
	//depthDesc.SampleDesc.Quality = 0;
	//depthDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//depthDesc.CPUAccessFlags = 0;
	//depthDesc.MiscFlags = 0;

	//hr = m_d3dDevice->CreateTexture2D(&depthDesc, nullptr, &m_d3dDepthStencil);
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	//ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	//depthStencilViewDesc.Format = depthDesc.Format;
	//depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0;
	//hr = m_d3dDevice->CreateDepthStencilView(m_d3dDepthStencil, &depthStencilViewDesc, &m_d3dDepthStencilView);

	//m_d3dContext->OMSetRenderTargets(1, &m_d3dRenderTargetView, m_d3dDepthStencilView);

	//// Setup the viewport
	//D3D11_VIEWPORT vp;
	//vp.Width = (FLOAT)width;
	//vp.Height = (FLOAT)height;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;
	//m_d3dContext->RSSetViewports(1, &vp);

	m_dxgiSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&m_surface);
	
	DCompositionCreateDevice(m_dxgiDevice,
		__uuidof(m_dcompDevice),
		reinterpret_cast<void **>(&m_dcompDevice));
	m_dcompDevice->CreateTargetForHwnd(m_hWnd,
		true, // Top most
		&m_target);
	m_dcompDevice->CreateVisual(&m_visual);
}


CD3DWindow::~CD3DWindow()
{
}

RECT CD3DWindow::GetRect()
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	return rect;
}

HWND CD3DWindow::GetHwnd()
{
	return m_hWnd;
}

IDXGISurface * CD3DWindow::GetSurface()
{
	return m_surface;
}

IDXGIDevice * CD3DWindow::GetDxgiDevice()
{
	return m_dxgiDevice;
}


void CD3DWindow::Update()
{
	//RECT rect;
	//::GetClientRect(m_hWnd, &rect);
	//InvalidateRect(&rect, FALSE);
	m_dxgiSwapChain->Present(1, 0);
	m_visual->SetContent(m_dxgiSwapChain);
	m_target->SetRoot(m_visual);
	m_dcompDevice->Commit();
}

LRESULT CD3DWindow::OnMouseEvent(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	int type = nMsg;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	if (m_mouseEventHandle) {
		m_mouseEventHandle(type, xPos, yPos);
	}
	return S_OK;
}

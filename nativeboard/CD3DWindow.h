#pragma once
#include "CWindowBase.h"
#include "typedef.h"
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <d3d11_1.h>
#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <dcomp.h>

class CD3DWindow:public CWindowBase
{
public:
	BEGIN_MSG_MAP(CD3DWindow)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseEvent)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseEvent)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseEvent)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CD3DWindow(
		_In_opt_ HWND hWndParent = NULL,
		_In_ _U_RECT rect = NULL,
		_In_opt_z_ LPCTSTR szWindowName = NULL,
		_In_ DWORD dwStyle = 0,
		_In_ DWORD dwExStyle = 0,
		_In_ _U_MENUorID MenuOrID = 0U,
		_In_opt_ LPVOID lpCreateParam = NULL
	);
	~CD3DWindow();

	HDC GetMemoryDC();
	RECT GetRect();
	HWND GetHwnd();
	IDXGISurface* GetSurface();
	IDXGIDevice * GetDxgiDevice();
	void Update();

	LRESULT OnMouseEvent(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
public:
	MOUSEHANDLE m_mouseEventHandle;
private:
	HWND m_hWnd;

	IDXGISwapChain1 *m_dxgiSwapChain;
	ID3D11Device *m_d3dDevice;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
	ID3D11DeviceContext *m_d3dContext;
	ID3D11RenderTargetView *m_d3dRenderTargetView;
	ID3D11Texture2D *m_d3dDepthStencil;
	ID3D11DepthStencilView *m_d3dDepthStencilView;
	IDXGISurface* m_surface;
	IDXGIFactory2 *m_dxgifactory;
	IDCompositionDevice *m_dcompDevice;
	IDXGIDevice *m_dxgiDevice;
	IDCompositionTarget *m_target;
	IDCompositionVisual *m_visual;
};


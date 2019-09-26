#include "stdafx.h"
#include "ClsD2DDraw.h"

ClsD2DDraw::ClsD2DDraw():
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pBrush(NULL)
{
}

ClsD2DDraw::~ClsD2DDraw()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBrush);
}

HRESULT ClsD2DDraw::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	// Create a Direct2D factory.
	D2D1_FACTORY_OPTIONS const options = { D2D1_DEBUG_LEVEL_NONE };
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &m_pDirect2dFactory);
	return hr;
}

HRESULT ClsD2DDraw::CreateDeviceResources(IDXGISurface * surface, IDXGIDevice *dxgiDevice)
{
	/*D2D1_SIZE_U size = D2D1::SizeU(
		rect.right - rect.left,
		rect.bottom - rect.top
	);
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);*/
	//D2D1_HWND_RENDER_TARGET_PROPERTIES hWndProps = D2D1::HwndRenderTargetProperties(hWnd, size);
	//HRESULT hr = m_pDirect2dFactory->CreateHwndRenderTarget(&props,&hWndProps, &m_pRenderTarget);

	//HRESULT hr = m_pDirect2dFactory->CreateDCRenderTarget(&props, &m_pRenderTarget);

	/*HRESULT hr = m_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(surface, &props, &m_pRenderTarget);*/

	HRESULT hr = m_pDirect2dFactory->CreateDevice(dxgiDevice, &m_pDirect2dDevice);
	m_pDirect2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pRenderTarget);

	D2D1_BITMAP_PROPERTIES1 properties = {};
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	m_pRenderTarget->CreateBitmapFromDxgiSurface(surface, properties, &m_bitmap);
	m_pRenderTarget->SetTarget(m_bitmap);

	if (SUCCEEDED(hr)) {
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red, 0.5F),
			&m_pBrush
		);
	}
	//hr = m_pRenderTarget->BindDC(gDC, &rect);
	return hr;
}

HRESULT ClsD2DDraw::Render()
{
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black,0.0F));

	D2D1_RECT_F rect = { 10.0F,10.0F,500.0F,300.0F };
	m_pRenderTarget->FillRectangle(rect, m_pBrush);
	m_pRenderTarget->EndDraw();

	return S_OK;
}

void ClsD2DDraw::onMouseEvent(int type, int x, int y)
{
	switch (type) {
	case WM_LBUTTONDOWN:
		DrawEnable(TRUE, x, y);
	break;
	case WM_LBUTTONUP:
		DrawEnable(FALSE, x, y);
	break;
	case WM_MOUSEMOVE:
		DrawLineTo(x, y);
	break;
	}
}

void ClsD2DDraw::DrawEnable(BOOL isStar, int x, int y)
{
	m_bDraw = isStar;
	if (m_bDraw) {
		m_pointCur = D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
	}
}

void ClsD2DDraw::DrawLineTo(int x, int y)
{
	if (m_bDraw) {
		m_pRenderTarget->BeginDraw();
		D2D1_POINT_2F nextPoint = D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
		m_pRenderTarget->DrawLine(m_pointCur, nextPoint, m_pBrush, 5.0f);
		m_pointCur = nextPoint;
		m_pRenderTarget->EndDraw();
		Update();
	}
}

void ClsD2DDraw::Update()
{
	if (m_updateHandle) {
		m_updateHandle();
	}
}

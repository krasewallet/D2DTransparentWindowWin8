#pragma once
#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include "typedef.h"

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


class ClsD2DDraw
{
public:
	ClsD2DDraw();
	~ClsD2DDraw();

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(IDXGISurface * surface, IDXGIDevice *dxgiDevice);

	HRESULT Render();
	void onMouseEvent(int type, int x, int y);
	void Update();
public:
	UPDATEHANDLE m_updateHandle;
private:
	void DrawEnable(BOOL isStart, int x = 0, int y = 0);
	void DrawLineTo(int x, int y);

	ID2D1Factory2* m_pDirect2dFactory;
	ID2D1Device1* m_pDirect2dDevice;
	ID2D1DeviceContext* m_pRenderTarget;
	//ID2D1RenderTarget* m_pRenderTarget;
	// ID2D1DCRenderTarget *m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBrush;
	ID2D1Bitmap1 *m_bitmap;

	bool m_bDraw;
	D2D1_POINT_2F m_pointCur;
};


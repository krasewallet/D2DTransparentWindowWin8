// nativeboard.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "nativeboard.h"
#include "CD3DWindow.h"
#include "ClsD2DDraw.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	ClsD2DDraw d2d;
	d2d.CreateDeviceIndependentResources();

	CD3DWindow win(NULL, CWindow::rcDefault, _T("D2DWindow"), WS_OVERLAPPEDWINDOW);
	//HDC gDC = win.GetMemoryDC();
	RECT rcDraw = win.GetRect();
	HWND hWnd = win.GetHwnd();

	d2d.CreateDeviceResources(win.GetSurface(), win.GetDxgiDevice());
	win.m_mouseEventHandle = bind(&ClsD2DDraw::onMouseEvent, &d2d, placeholders::_1, placeholders::_2, placeholders::_3);
	d2d.m_updateHandle = bind(&CD3DWindow::Update, &win);
	d2d.Render();
	//
	d2d.Update();
    // 主消息循环:
	MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

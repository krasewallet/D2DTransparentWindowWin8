#pragma once
#include "CWindowBase.h"
#include "CLayeredInfo.h"
#include "typedef.h"

class CLayeredWindow:public CWindowBase
{
public:
	BEGIN_MSG_MAP(CLayeredWindow)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseEvent)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseEvent)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseEvent)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CLayeredWindow(
		_In_opt_ HWND hWndParent = NULL,
		_In_ _U_RECT rect = NULL,
		_In_opt_z_ LPCTSTR szWindowName = NULL,
		_In_ DWORD dwStyle = 0,
		_In_ DWORD dwExStyle = 0,
		_In_ _U_MENUorID MenuOrID = 0U,
		_In_opt_ LPVOID lpCreateParam = NULL
	);
	~CLayeredWindow();

	HDC GetMemoryDC();
	RECT GetRect();
	HWND GetHwnd();
	void Update();

	LRESULT OnMouseEvent(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
public:
	MOUSEHANDLE m_mouseEventHandle;
private:
	CLayeredInfo m_info;
	HWND m_hWnd;
	HDC m_memDC;
	HBITMAP m_DIBSelection,m_OldBmp;
};


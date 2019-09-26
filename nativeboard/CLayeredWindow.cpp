#include "stdafx.h"
#include "CLayeredWindow.h"
#include <Windows.h>

CLayeredWindow::CLayeredWindow(
	_In_opt_ HWND hWndParent,
	_In_ _U_RECT rect,
	_In_opt_z_ LPCTSTR szWindowName,
	_In_ DWORD dwStyle,
	_In_ DWORD dwExStyle,
	_In_ _U_MENUorID MenuOrID,
	_In_opt_ LPVOID lpCreateParam)
{
	m_hWnd = __super::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle | WS_EX_LAYERED, MenuOrID, lpCreateParam);
	m_info.Init(m_hWnd);

	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd);
}


CLayeredWindow::~CLayeredWindow()
{
	if (m_memDC && m_OldBmp)
		SelectObject(m_memDC, m_OldBmp);
	if (m_DIBSelection)
		DeleteObject(m_DIBSelection);
	if (m_memDC)
		DeleteDC(m_memDC);
	m_memDC = 0;
	m_OldBmp = 0;
	m_DIBSelection = 0;
}

HDC CLayeredWindow::GetMemoryDC()
{
	HDC hDC = ::GetDC(m_hWnd);
	m_memDC = CreateCompatibleDC(hDC);
	::ReleaseDC(m_hWnd, hDC);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = m_info.m_size.cx;
	bmi.bmiHeader.biHeight = m_info.m_size.cy;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	m_DIBSelection = CreateDIBSection(m_memDC, &bmi, DIB_RGB_COLORS, 0, 0, 0);
	m_OldBmp = (HBITMAP)SelectObject(m_memDC, m_DIBSelection);
	return m_memDC;
}

RECT CLayeredWindow::GetRect()
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	return rect;
}

HWND CLayeredWindow::GetHwnd()
{
	return m_hWnd;
}

void CLayeredWindow::Update()
{
	m_info.Update(m_memDC);
	UpdateLayeredWindowIndirect(m_hWnd, &m_info);
}

LRESULT CLayeredWindow::OnMouseEvent(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	int type = nMsg;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	if (m_mouseEventHandle) {
		m_mouseEventHandle(type, xPos, yPos);
	}
	return S_OK;
}

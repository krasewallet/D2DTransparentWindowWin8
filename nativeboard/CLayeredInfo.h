#pragma once
#include <Windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>

class CLayeredInfo
{
public:
	CLayeredInfo();
	~CLayeredInfo();

	void Init(HWND hWnd);
	void Update(HDC source);

	UPDATELAYEREDWINDOWINFO* operator &();

	POINT m_sourcePosition;
	POINT m_windowPosition;
	CSize m_size;
	BLENDFUNCTION m_blend;
	UPDATELAYEREDWINDOWINFO m_info;
};


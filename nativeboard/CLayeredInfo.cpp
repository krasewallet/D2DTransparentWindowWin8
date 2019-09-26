#include "stdafx.h"
#include "CLayeredInfo.h"

CLayeredInfo::CLayeredInfo():
	m_sourcePosition(),
	m_windowPosition(),
	m_size(),
	m_blend(),
	m_info()
{
	m_info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
	m_info.pptSrc = &m_sourcePosition;
	m_info.pptDst = &m_windowPosition;
	m_info.psize = &m_size;
	m_info.pblend = &m_blend;
	m_info.dwFlags = ULW_ALPHA;

	m_blend.SourceConstantAlpha = 255;
	m_blend.AlphaFormat = AC_SRC_ALPHA;
}


CLayeredInfo::~CLayeredInfo()
{
}

void CLayeredInfo::Init(HWND hWnd)
{
	RECT rcClient;
	::GetClientRect(hWnd, &rcClient);
	m_size = { rcClient.right - rcClient.left,rcClient.bottom - rcClient.top };
	m_info.hdcDst = ::GetDC(hWnd);
}

void CLayeredInfo::Update(HDC source)
{
	m_info.hdcSrc = source;
}

UPDATELAYEREDWINDOWINFO * CLayeredInfo::operator&()
{
	return &m_info;
}

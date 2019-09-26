#include "stdafx.h"
#include "CWindowBase.h"


CWindowBase::CWindowBase()
{
}


CWindowBase::~CWindowBase()
{
}

LRESULT CWindowBase::OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	PostQuitMessage(1);
	return S_OK;
}

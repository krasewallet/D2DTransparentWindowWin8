#pragma once
#include "stdafx.h"
#include <atlbase.h>
#include <atlwin.h>

class CWindowBase:public CWindowImpl<CWindowBase>
{
public:
	BEGIN_MSG_MAP(CWindowBase)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CWindowBase();
	~CWindowBase();
protected:
	LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
};


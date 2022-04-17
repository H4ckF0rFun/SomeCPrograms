#pragma once
#include "MFC.h"
class CMyWinApp :public CWinApp
{
public:
	DECLARE_DYNAMIC(CMyWinApp)
	CMyWinApp();
	~CMyWinApp();
	virtual BOOL InitInstance();
};
class CMyFrameWnd :public CFrameWnd
{
public:
	DECLARE_DYNAMIC(CMyFrameWnd)
	CMyFrameWnd();
	~CMyFrameWnd();

};

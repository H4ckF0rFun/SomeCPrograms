#include "My.h"

CMyWinApp theApp;

BOOL CMyWinApp::InitInstance()
{
	
	cout << "CMyWinApp::InitInstance" << endl;
	m_pMainWnd = new CMyFrameWnd;
	return TRUE;
}
CMyFrameWnd::CMyFrameWnd()
{
	cout << "CMyFrameWnd::CMyFrameWnd" << endl;
	Create();
}

CMyWinApp::CMyWinApp()
{
	cout << "CMyWinApp::CMyWinApp" << endl;
}
CMyWinApp::~CMyWinApp()
{
	cout << "CMyWinApp::~CMyWinApp" << endl;
}
CMyFrameWnd::~CMyFrameWnd()
{
	//cout << "CMyFrameWnd::~CMyFrameWnd" << endl;
}

IMPLEMENT_NYNAMIC(CMyWinApp, CWinApp)
IMPLEMENT_NYNAMIC(CMyFrameWnd, CFrameWnd)
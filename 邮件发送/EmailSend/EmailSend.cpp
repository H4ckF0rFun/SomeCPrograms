// EmailSend.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EmailSend.h"
#include "EmailSendDlg.h"
#include "LogonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmailSendApp

BEGIN_MESSAGE_MAP(CEmailSendApp, CWinApp)
	//{{AFX_MSG_MAP(CEmailSendApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmailSendApp construction

CEmailSendApp::CEmailSendApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEmailSendApp object

CEmailSendApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEmailSendApp initialization

BOOL CEmailSendApp::InitInstance()
{
	//初始化套接字库
	WSADATA wsadata;
	if(0 != WSAStartup(MAKEWORD(2,0),&wsadata))
	{
		AfxMessageBox("初始化套接字库失败!");
		return FALSE;
	}
	//new socket对象，s传给两个dlg对象处理
	CSocket *s = new CSocket;
	
	//创建登录窗口
	CLogonDlg LogonDlg(s);
	if(IDCANCEL/*点击了退出*/ == LogonDlg.DoModal())
	{
		delete s;
		return FALSE;
	}
	//主界面窗口
	CEmailSendDlg EmailSendDlg(s,LogonDlg.m_User);
	EmailSendDlg.DoModal();

	return FALSE;
}

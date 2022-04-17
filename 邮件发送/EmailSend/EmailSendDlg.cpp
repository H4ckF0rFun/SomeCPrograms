// EmailSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EmailSend.h"
#include "EmailSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmailSendDlg dialog

CEmailSendDlg::CEmailSendDlg(CSocket*s,CString User,CWnd* pParent /*=NULL*/)
	: CDialog(CEmailSendDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmailSendDlg)
	m_Receiver = _T("");
	m_Subject = _T("");
	m_ChaoSong = _T("");
	m_Body = _T("");
	m_OutMsg = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_socket = s;
	m_User = User;
}

void CEmailSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmailSendDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Receiver);
	DDX_Text(pDX, IDC_EDIT2, m_Subject);
	DDX_Text(pDX, IDC_EDIT3, m_ChaoSong);
	DDX_Text(pDX, IDC_EDIT4, m_Body);
	DDX_Text(pDX, IDC_EDIT5, m_OutMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEmailSendDlg, CDialog)
	//{{AFX_MSG_MAP(CEmailSendDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmailSendDlg message handlers

BOOL CEmailSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowText(m_User);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEmailSendDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEmailSendDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEmailSendDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_Receiver.IsEmpty())
	{
		MessageBox("请填写收件人地址!","提示");
		return;
	}

	char RecvBuff[256];
	CString SendMsg;
	m_OutMsg = "";
	//开始发送
	
	//发件人
	SendMsg = "MAIL FROM:<";
	SendMsg +=m_User;
	SendMsg +=">\r\n";

	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//接收响应
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//输出接收的响应
	m_OutMsg +=RecvBuff;
	UpdateData(FALSE);
	

	if(strcmp(RecvBuff,"250 Mail OK\r\n"))
		return;
	

	//收件人
	SendMsg = "RCPT TO:<";
	SendMsg +=m_Receiver;
	SendMsg +=">\r\n";

	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//接收响应
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//输出接收的响应
	m_OutMsg +=RecvBuff;
	UpdateData(FALSE);
	
	if(strcmp(RecvBuff,"250 Mail OK\r\n"))
		return;

	//这里是后来添加的，好像是只要是要收到的邮箱都要RCPT TO
	if(!m_ChaoSong.IsEmpty())
	{
		SendMsg = "RCPT TO:<";
		SendMsg +=m_ChaoSong;
		SendMsg +=">\r\n";

		m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
		//接收响应
		memset(RecvBuff,0,sizeof(RecvBuff));
		m_socket->Receive(RecvBuff,sizeof(RecvBuff));
		//输出接收的响应
		m_OutMsg +=RecvBuff;
		UpdateData(FALSE);
		if(strcmp(RecvBuff,"250 Mail OK\r\n"))
			return;
	}
	//发送邮件开始标记
	SendMsg ="DATA\r\n";

	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//接收响应
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//输出接收的响应
	m_OutMsg +=RecvBuff;
	UpdateData(FALSE);
	
	if(strcmp(RecvBuff,"354 End data with <CR><LF>.<CR><LF>\r\n"))
		return;
	SendMsg="";
	//发送邮件

	//判断主题是否为空
	if(!m_Subject.IsEmpty())
	{
		SendMsg+="Subject:";
		SendMsg+=m_Subject;
		SendMsg+="\r\n";
	}
	//
	SendMsg+="From:<";
	SendMsg+=m_User;
	SendMsg+=">\r\n";

	SendMsg+="To:<";
	SendMsg+=m_Receiver;
	SendMsg+=">\r\n";

	//判断抄送是否为空
	if(!m_ChaoSong.IsEmpty())
	{
		SendMsg+="Cc:<";
		SendMsg+=m_ChaoSong;
		SendMsg+=">\r\n";
	}

	SendMsg+="\r\n";
	SendMsg+=m_Body;
	SendMsg+="\r\n.\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//接收响应
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//输出接收的响应
	m_OutMsg +=RecvBuff;
	UpdateData(FALSE);
}

void CEmailSendDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString SendMsg = "quit\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	char RecvBuff[256] = {0};
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	AfxMessageBox(RecvBuff);
	CDialog::OnClose();
}


//QQ邮箱好像发不了
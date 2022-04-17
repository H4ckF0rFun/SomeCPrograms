
// LogonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EmailSend.h"
#include "LogonDlg.h"
#include "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog


CLogonDlg::CLogonDlg(CSocket*s,CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{
	m_socket = s;
	//{{AFX_DATA_INIT(CLogonDlg)
	m_SmtpServer = _T("smtp.163.com");
	m_User = _T("qaz721566701@163.com");
	m_Password = _T("qaz721566701");
	//}}AFX_DATA_INIT
}


void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Text(pDX, IDC_EDIT1, m_SmtpServer);
	DDX_Text(pDX, IDC_EDIT2, m_User);
	DDX_Text(pDX, IDC_EDIT3, m_Password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg message handlers

void CLogonDlg::OnOK() 
{
	// TODO: Add extra validation here
	//�ı����ؼ�--->����(TRUE)		����--->�ؼ�(FASLE)
	UpdateData(TRUE);
	if(m_SmtpServer.IsEmpty() || m_User.IsEmpty() || m_Password.IsEmpty())
	{
		MessageBox("�뽫��Ϣ��д����!","��ʾ");
		return;
	}
	//����socket
	if(FALSE == m_socket->Create())
	{
		AfxMessageBox("����socketʧ��!");
		return;
	}
	//���ӷ�����
	if(FALSE == m_socket->Connect(m_SmtpServer,25))
	{
		MessageBox("�޷����ӵ�������!","error");
		m_socket->Close();
		return;
	}
	char RecvBuff[200];
	CString SendMsg;
	//���շ���������Ϣ
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//AfxMessageBox(RecvBuff);
	//����HELO
	SendMsg = "HELO smtp\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//������Ӧ
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//AfxMessageBox(RecvBuff);
	//�ж���Ӧ
	if(strcmp(RecvBuff,"250 OK\r\n"))
	{
		MessageBox("��¼ʧ��!","error");
		m_socket->Close();
		return;
	}
	//���߷�����Ҫ��¼��
	SendMsg = "auth login\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//������Ӧ
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//�ж���Ӧ
	if(strcmp(RecvBuff,"334 dXNlcm5hbWU6\r\n"))
	{
		MessageBox("��¼ʧ��!","error");
		m_socket->Close();
		return;
	}
	
	CBase64 Encoder;
	//����base64����������
	SendMsg = Encoder.Encode(m_User,m_User.GetLength());
	SendMsg+="\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//������Ӧ
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//�ж���Ӧ
	if(strcmp(RecvBuff,"334 UGFzc3dvcmQ6\r\n"))
	{
		MessageBox("��¼ʧ��!","error");
		m_socket->Close();
		return;
	}

	//����base64����������
	SendMsg = Encoder.Encode(m_Password,m_Password.GetLength());
	SendMsg+="\r\n";
	m_socket->Send(SendMsg.GetBuffer(0),SendMsg.GetLength());
	//������Ӧ
	memset(RecvBuff,0,sizeof(RecvBuff));
	m_socket->Receive(RecvBuff,sizeof(RecvBuff));
	//�ж���Ӧ
	if(strcmp(RecvBuff,"235 Authentication successful\r\n"))
	{
		MessageBox("��¼ʧ��!","error");
		m_socket->Close();
		return;
	}
	//MessageBox("��¼�ɹ�!","error");
	CDialog::OnOK();
}

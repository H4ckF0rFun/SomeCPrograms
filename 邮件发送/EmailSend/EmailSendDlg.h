// EmailSendDlg.h : header file
//

#if !defined(AFX_EMAILSENDDLG_H__3C7F8EBD_7422_49C6_B744_11860B12CE50__INCLUDED_)
#define AFX_EMAILSENDDLG_H__3C7F8EBD_7422_49C6_B744_11860B12CE50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEmailSendDlg dialog

class CEmailSendDlg : public CDialog
{
// Construction
public:
	CSocket *m_socket;
	CString m_User;
	CEmailSendDlg(CSocket*s,CString User,CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEmailSendDlg)
	enum { IDD = IDD_EMAILSEND_DIALOG };
	CString	m_Receiver;
	CString	m_Subject;
	CString	m_ChaoSong;
	CString	m_Body;
	CString	m_OutMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmailSendDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEmailSendDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILSENDDLG_H__3C7F8EBD_7422_49C6_B744_11860B12CE50__INCLUDED_)

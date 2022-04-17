#if !defined(AFX_LOGONDLG_H__7AAD3B46_7B12_4879_A464_92D0EBCBACA1__INCLUDED_)
#define AFX_LOGONDLG_H__7AAD3B46_7B12_4879_A464_92D0EBCBACA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog

class CLogonDlg : public CDialog
{
// Construction
public:
	CSocket*m_socket;
	CLogonDlg(CSocket*s,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogonDlg)
	enum { IDD = IDD_LOGON_DLG };
	CString	m_SmtpServer;
	CString	m_User;
	CString	m_Password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogonDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGONDLG_H__7AAD3B46_7B12_4879_A464_92D0EBCBACA1__INCLUDED_)

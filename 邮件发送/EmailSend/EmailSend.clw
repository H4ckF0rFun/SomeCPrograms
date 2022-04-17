; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEmailSendDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EmailSend.h"

ClassCount=3
Class1=CEmailSendApp
Class2=CEmailSendDlg

ResourceCount=3
Resource2=IDD_EMAILSEND_DIALOG
Resource1=IDR_MAINFRAME
Class3=CLogonDlg
Resource3=IDD_LOGON_DLG

[CLS:CEmailSendApp]
Type=0
HeaderFile=EmailSend.h
ImplementationFile=EmailSend.cpp
Filter=N

[CLS:CEmailSendDlg]
Type=0
HeaderFile=EmailSendDlg.h
ImplementationFile=EmailSendDlg.cpp
Filter=D
LastObject=CEmailSendDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_EMAILSEND_DIALOG]
Type=1
Class=CEmailSendDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT2,edit,1350631552
Control10=IDC_EDIT3,edit,1350631552
Control11=IDC_EDIT4,edit,1352732676
Control12=IDC_STATIC,button,1342177287
Control13=IDC_EDIT5,edit,1352730756

[CLS:CLogonDlg]
Type=0
HeaderFile=LogonDlg.h
ImplementationFile=LogonDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLogonDlg
VirtualFilter=dWC

[DLG:IDD_LOGON_DLG]
Type=1
Class=CLogonDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT1,edit,1350631552
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_EDIT3,edit,1350631584


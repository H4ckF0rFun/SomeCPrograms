#pragma once
#include <iostream>
using namespace std;

typedef char* LPSTR;
typedef int BOOL;
#define TRUE 1
#define FALSE 0

//////////////////////////////////////////////////////////////////
//RTTI
class CObject;

struct CRuntimeClass
{
	LPSTR m_lpszClassName;
	int m_nObjectSzie;
	int m_nSchema;
	CObject* (*pfnCreateObject)();
	CObject* CreateObject();
	static void Load();
	static CRuntimeClass *pFirstClass;
	CRuntimeClass *pBaseClass;
	CRuntimeClass *pNextClass;
};

struct  AFX_CLASSINIT
{
	AFX_CLASSINIT(CRuntimeClass*pClass)
	{
		pClass->pNextClass = CRuntimeClass::pFirstClass;
		CRuntimeClass::pFirstClass = pClass;
	}
};

#define RUNTIMECLASS(class_name) \
	(&class_name::_class##class_name) \

#define DECLARE_DYNAMIC(class_name) \
	public:\
	static CRuntimeClass _class##class_name; \
	virtual CRuntimeClass*GetRuntimeClass(); \

#define IMPLEMENT_NYNAMIC(class_name,baseclass_name) \
	IMPLEMENT_RUNTIMECLASS(class_name, baseclass_name, 0xffff, NULL) \

#define IMPLEMENT_RUNTIMECLASS(class_name,baseclass_name,nSchema,pfnCreateObject) \
	static char lpsz##class_name[] = #class_name; \
	CRuntimeClass class_name::_class##class_name = \
{\
	lpsz##class_name, sizeof(class_name), nSchema, pfnCreateObject, RUNTIMECLASS(baseclass_name), NULL \
}; \
	AFX_CLASSINIT init##class_name(RUNTIMECLASS(class_name)); \
	CRuntimeClass* class_name::GetRuntimeClass() \
{\
	return RUNTIMECLASS(class_name); \
}\

//////////////////////////////////////////////////////////////////
//Dynamic Create
#define DECLARE_DYCREATE(class_name) \
	DECLARE_DYNAMIC(class_name) \
	static CObject*CreateObject(); \

#define IMPLEMENT_DYCREATE(class_name,baseclass_name) \
	CObject* class_name::CreateObject() \
{\
	return new class_name; \
}\
	IMPLEMENT_RUNTIMECLASS(class_name,baseclass_name,0xffff,&class_name::CreateObject) \

//////////////////////////////////////////////////////////////////
class CObject
{
public:
	static CRuntimeClass _classCObject;
	virtual CRuntimeClass*GetRuntimeClass();
	virtual BOOL IsKindOf(CRuntimeClass*);
	virtual void Hello() = 0;
	CObject()
	{
		cout << "CObject::CObject" << endl;
	}
	~CObject()
	{
		cout << "CObject::~CObject" << endl;
	}
};

class CCmdTarget :public CObject
{
public:
	DECLARE_DYCREATE(CCmdTarget)
	//DECLARE_DYNAMIC(CCmdTarget)
	void Hello()
	{
		printf("Hello ,this is a %s\n",_classCCmdTarget.m_lpszClassName);
	}
	CCmdTarget()
	{
		cout << "CCmdTarget::CCmdTarget" << endl;
	}
	~CCmdTarget()
	{
		cout << "CCmdTarget::~CCmdTarget" << endl;
	}
};
class CWinThread :public CCmdTarget
{
public:
	DECLARE_DYCREATE(CWinThread)
	//DECLARE_DYNAMIC(CWinThread)
	void Hello()
	{
		printf("Hello ,this is a %s\n", _classCWinThread.m_lpszClassName);
	}
	CWinThread()
	{
		cout << "CWinThread::CWinThread" << endl;
	}
	~CWinThread()
	{
		cout << "CWinThread::~CWinThread" << endl;
	}
	virtual int Run()
	{
		cout << "CWnThread::Run" << endl;
		return 1;
	}
	virtual BOOL InitInstance()
	{
		cout << "CWinThread::InitInstacne" << endl;
		return TRUE;
	}

};
class CWnd;
class CWinApp :public CWinThread
{
public:
	DECLARE_DYCREATE(CWinApp)
	//DECLARE_DYNAMIC(CWinApp)
	CWinApp * m_pCurrentWinApp;
	CWnd * m_pMainWnd;
	CWinApp()
	{
		m_pCurrentWinApp = this;
		cout << "CWinApp::CWinApp" << endl;
	}
	~CWinApp()
	{
		cout << "CWinApp::~CWinApp" << endl;
	}
	void Hello()
	{
		printf("Hello ,this is a %s\n", _classCWinApp.m_lpszClassName);
	}
	virtual BOOL InitApplication()
	{
		cout << "CWinApp::InitApplication" << endl;
		return TRUE;
	}
	virtual BOOL InitInstance()
	{
		cout << "CWinApp::InitInstance" << endl;
		return TRUE;
	}
	virtual int Run()
	{
		cout << "CWinApp::Run" << endl;
		return CWinThread::Run();
	}
};

class CWnd :public CCmdTarget
{
public:
	DECLARE_DYCREATE(CWnd)
	//DECLARE_DYNAMIC(CWnd)
	CWnd()
	{
		cout << "CWnd::CWnd" << endl;
	}
	~CWnd()
	{
		cout << "CWnd::~CWnd" << endl;
	}
	void Hello()
	{
		printf("Hello ,this is a %s\n", _classCWnd.m_lpszClassName);
	}
	virtual BOOL PreCreateWindow()
	{
		cout << "CWnd::PreCreateWindow" << endl;
		return TRUE;
	}
	virtual BOOL Create()
	{
		cout << "CWnd::Create" << endl;
		return TRUE;
	}
	 BOOL CreateEx()
	{
		 cout << "CWnd::CreateEx" << endl;
		 PreCreateWindow();
		 return TRUE;
	}
	

};

class CFrameWnd :public CWnd
{
public:
	DECLARE_DYCREATE(CFrameWnd)
	//DECLARE_DYNAMIC(CFrameWnd)
	CFrameWnd()
	{
		cout << "CFrameWnd::CFrameWnd" << endl;
	}
	~CFrameWnd()
	{
		cout << "CFrameWnd::~CFrameWnd" << endl;
	}
	void Hello()
	{
		printf("Hello ,this is a %s\n", _classCFrameWnd.m_lpszClassName);
	}
	BOOL Create()
	{
		cout << "CFrameWnd::Create" << endl;
		CreateEx();
		return TRUE;
	}
	BOOL PreCreateWindow()
	{
		cout << "CFrameWnd::PreCreateWindow" << endl;
		return TRUE;
	}
};


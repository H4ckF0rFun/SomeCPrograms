
#include "My.h"
extern CMyWinApp theApp;

CObject* CRuntimeClass::CreateObject()
{
	CObject *pObj = (*pfnCreateObject)();
	if (pObj == NULL)
		return NULL;
	return pObj;
}

void CRuntimeClass::Load()
{
	cout << "[Begine Dynamic Create] " << endl;
	while (1)
	{
		char ClassName[64] = { 0 };
		
		cout << "Please input a class name" << endl;
		cin >> ClassName;
		CRuntimeClass*pclass;
		for (pclass = CRuntimeClass::pFirstClass; pclass != NULL; pclass = pclass->pNextClass)
		{
			if (0 == strcmp(pclass->m_lpszClassName, ClassName))
			{
				if (0 == strcmp(pclass->m_lpszClassName, "CObject"))
				{
					cout << "Sorry ,Couldn't create the object" << endl;
				}
				else
				{
					cout << "Ok ,Creating the object" << endl;
					CObject * pObj = pclass->CreateObject();
					if (pObj != NULL)
					{
						pObj->Hello();
						delete pObj;
					}

				}
				break;
			}
		}
		if (pclass == NULL)
		{
			cout << "Sorry ,not found the class name!" << endl;
		}
	}
}


















BOOL CObject::IsKindOf(CRuntimeClass*pClass)
{
	
	for (CRuntimeClass *pCurrrentClass = GetRuntimeClass(); pCurrrentClass != NULL; pCurrrentClass = pCurrrentClass->pBaseClass)
	{
		if (pCurrrentClass == pClass)
		{
			 printf("Yes ,the %s object belong to the %s class!\n",GetRuntimeClass()->m_lpszClassName,pClass->m_lpszClassName);
			return TRUE;
		}
	}
	printf("No ,the %s object don't belong to the %s class!\n", GetRuntimeClass()->m_lpszClassName, pClass->m_lpszClassName);
	return FALSE;
}

//CObject特殊处理,实际就是多了把pFirstClass置为0
static char lpszCObject[] = "CObject";

CRuntimeClass CObject::_classCObject =
{
	lpszCObject, sizeof(CObject), 0xffff, NULL, NULL, NULL
};

CRuntimeClass * CRuntimeClass::pFirstClass = NULL;

AFX_CLASSINIT initCObject(RUNTIMECLASS(CObject));
CRuntimeClass *CObject::GetRuntimeClass()
{
	return &CObject::_classCObject;
}
//

//IMPLEMENT_NYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYCREATE(CCmdTarget, CObject)
//IMPLEMENT_NYNAMIC(CWinThread, CCmdTarget)
IMPLEMENT_DYCREATE(CWinThread, CCmdTarget)
//IMPLEMENT_NYNAMIC(CWinApp, CWinThread)
IMPLEMENT_DYCREATE(CWinApp, CWinThread)
//IMPLEMENT_NYNAMIC(CWnd, CCmdTarget)
IMPLEMENT_DYCREATE(CWnd, CCmdTarget)
//IMPLEMENT_NYNAMIC(CFrameWnd, CWnd)
IMPLEMENT_DYCREATE(CFrameWnd, CWnd)

CWinApp * AfxGetApp()
{
	return theApp.m_pCurrentWinApp;
}
int main()
{
	cout << "**************************************************************" << endl << endl;
	CWinApp *pApp = AfxGetApp();
	pApp->InitApplication();
	pApp->InitInstance();
	pApp->Run();
	cout << "**************************************************************" << endl << endl;
	//输出类型链表
	for (CRuntimeClass*pClass = CRuntimeClass::pFirstClass; pClass != NULL; pClass = pClass->pNextClass)
	{
		cout << pClass->m_lpszClassName << "\t";
		if (pClass->pBaseClass != NULL)
		{
			cout << pClass->pBaseClass->m_lpszClassName << endl;
		}
	}
	cout << "**************************************************************" << endl << endl;
	CWinApp* pCWinAppObj= new CWinApp;
	pCWinAppObj->IsKindOf(RUNTIMECLASS(CObject));
	pCWinAppObj->IsKindOf(RUNTIMECLASS(CWnd));
	pCWinAppObj->IsKindOf(RUNTIMECLASS(CCmdTarget));

	CFrameWnd*CFrameWndObj = new CFrameWnd;
	CFrameWndObj->IsKindOf(RUNTIMECLASS(CObject));
	CFrameWndObj->IsKindOf(RUNTIMECLASS(CWinApp));
	CFrameWndObj->IsKindOf(RUNTIMECLASS(CWnd));
	CFrameWndObj->IsKindOf(RUNTIMECLASS(CCmdTarget));
	cout << "**************************************************************" << endl << endl;
	CRuntimeClass::Load();
	return 0;
}
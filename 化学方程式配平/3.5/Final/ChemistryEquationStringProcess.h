#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <Windows.h>
#include <stdio.h>

char* convertUTF8ToGB2312(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

char* convertGB2312ToUTF8(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
#include "EquationSetSolve.h"
typedef int BOOL;
#define TRUE 1
#define FALSE 0

BOOL bShowProcess = FALSE;

typedef struct _ELEMENT
{
	char Name[128];		//Ԫ������;
	int Num;			//����;
	struct _ELEMENT *next;
}ELEMENT;

typedef struct _SUBSTANCE
{
	char kName[64];		//��ϵ������Ԫ;
	char Name[128];		//��������;
	char UsedName[128];	//Used Name;
	ELEMENT *elements;	//������ʵ�Ԫ��;
	struct _SUBSTANCE *next;
}SUBSTANCE;

typedef struct _SUBSTANCEPOINTER
{
	SUBSTANCE*pAddress;
	struct _SUBSTANCEPOINTER*next;
}SUBSTANCEPOINTER;


char kName = 'A';		//ÿ������ǰ��Ԫ;

SUBSTANCE *LeftSubsHead = 0;	//������ߵ�����;
SUBSTANCE *LeftSubsTail = 0;

SUBSTANCE *RightSubsHead = 0;	//�����ұߵ�����;
SUBSTANCE *RightSubsTail = 0;	//�����ұߵ�����;

ELEMENT*AllEleCLas = 0;			//�������е�Ԫ��;
int AllEleClasCount = 0;		//��¼Ԫ�صĸ���;

SUBSTANCEPOINTER *AllSubsAddrsHead = NULL;		//��¼����Ԫ�صĵ�ַ;
SUBSTANCEPOINTER *AllSubsAddrsTail = NULL;

char szEquationSet[128];		//�������ɵķ�����;
char szEquation[2048];			//���淽��ʽ;
char szRetEquation[2048];		//���淽��ʽ;
char szLeftEqua[1024];			//�󲿷ַ���ʽ;
char szRightEqua[1024];			//�Ҳ��ַ���ʽ;
//�Ƿ���Ԫ��������;
ELEMENT* InElementList(char*ElementName, ELEMENT*ListHead)
{
	ELEMENT*pEle;
	for (pEle = ListHead; pEle != NULL; pEle = pEle->next)
	{
		if (!strcmp(pEle->Name, ElementName))
			break;
	}
	return pEle;
}

//����ַ�����;
void Init()
{
	memset(szEquation, 0, sizeof(szEquation));
	memset(szEquation, 0, sizeof(szRetEquation));
	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));
}
//�ָ��ʽ,ʧ�ܷ���FALSE,�ɹ�����TRUE;
BOOL DevideEquation()
{
	char *p;
	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));

	p = szEquation;
	//�Ⱥ����û�ж���;
	if (*p == '=')
		return FALSE;
	while (*p != '='&& *p != NULL) p++;
	//û�е��ں�;
	if (*p == NULL)
		return FALSE;
	//���淽��ʽ�󲿷�;
	memcpy(szLeftEqua, szEquation, p - szEquation);
	//�Ƶ�����ʽ�Ҳ��ֿ�ʼ��;
	while (*p == '='&& *p != NULL) p++;
	//�Ⱥ��ұ�û�ж���;
	if (*p == NULL)
		return FALSE;
	//�����ұߵķ���ʽ;
	strcpy(szRightEqua, p);
	//�������ָ��������;
	strcat(szLeftEqua, "+");
	strcat(szRightEqua, "+");
	return TRUE;
}
//�ָ��������;
BOOL CallDevideSubstance(char*szPartEquation, SUBSTANCE**SubsListHead, SUBSTANCE**SubsListTail)
{
	char*pSubsStart, *pSubsEnd;
	pSubsStart = pSubsEnd = szPartEquation;
	while (*pSubsEnd != NULL)
	{
		//��һ��Ԫ�ص�β��;
		if (*pSubsEnd == '+')
		{
			//��ӵ�SubsList��;
			//βΪ��,ͷ������;
			if (*SubsListTail == NULL)
			{
				*SubsListHead = *SubsListTail = (SUBSTANCE*)malloc(sizeof(SUBSTANCE));
			}
			//β��Ϊ��,��β�ĺ������;
			else
			{
				(*SubsListTail)->next = (SUBSTANCE*)malloc(sizeof(SUBSTANCE));
				*SubsListTail = (*SubsListTail)->next;
			}
			/*--------------------------------��ӵ�AllSubsAddrsList��----------------------------*/
			if (AllSubsAddrsTail == NULL)
			{
				AllSubsAddrsHead = AllSubsAddrsTail = (SUBSTANCEPOINTER*)malloc(sizeof(SUBSTANCEPOINTER));
			}
			else
			{
				AllSubsAddrsTail->next = (SUBSTANCEPOINTER*)malloc(sizeof(SUBSTANCEPOINTER));
				AllSubsAddrsTail = AllSubsAddrsTail->next;
			}
			AllSubsAddrsTail->next = NULL;
			AllSubsAddrsTail->pAddress = *SubsListTail;
			/*-----------------------------------------------------------------------------------*/

			//������SUBSTANCE��ֵ;
			memset((*SubsListTail)->kName, 0, sizeof((*SubsListTail)->kName));
			memset((*SubsListTail)->Name, 0, sizeof((*SubsListTail)->Name));
			memset((*SubsListTail)->UsedName, 0, sizeof((*SubsListTail)->UsedName));

			(*SubsListTail)->elements = NULL;
			(*SubsListTail)->next = NULL;
			*(*SubsListTail)->kName = kName++;

			//�����Ѿ���д��ϵ��;
			while (*pSubsStart >= '0' && *pSubsStart <= '9')
				pSubsStart++;
			//������,û��Ԫ������;
			if (pSubsEnd == pSubsStart)
				return FALSE;
			memcpy((*SubsListTail)->Name, pSubsStart, pSubsEnd - pSubsStart);
			memcpy((*SubsListTail)->UsedName, pSubsStart, pSubsEnd - pSubsStart);
			pSubsStart = pSubsEnd + 1;
		}
		pSubsEnd++;
	}
	return TRUE;
}
BOOL DevideSubstance()
{
	//�ָ���ߵ�����;
	if (!CallDevideSubstance(szLeftEqua, &LeftSubsHead, &LeftSubsTail))
		return FALSE;
	//�ָ��ұߵ�����;
	if (!CallDevideSubstance(szRightEqua, &RightSubsHead, &RightSubsTail))
		return FALSE;
	return TRUE;
}
//����ת��Ϊ����;
void CallChangePoint(SUBSTANCE*pSubs)
{
	char *pPointStart, *pPointEnd;
	int value = 0, len = 0;
	char buff[128] = { 0 };
	while (1)
	{
		pPointEnd = pSubs->UsedName;
		//���ҿ�ʼ��;
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;
		pPointStart = pPointEnd;
		//�����ڵ�;
		if (*pPointStart == NULL)
			break;
		pPointEnd++;
		//���ҽ�����;
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;

		//��ʼת����;
		//��������Ϊ����;
		if (*(pPointStart + 1) >= '0' && *(pPointStart + 1) <= '9')
		{
			//��������󣨰����㣩���Ƶ�buff��;
			memcpy(buff, pPointEnd, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			//��������󣨰����㣩��Ϊ0
			memset(pPointEnd, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			strcat(pSubs->UsedName, ")");
			len = StrToNum(pPointStart + 1, &value);
			//������ֵ�����;
			NumToStr(pPointEnd + 1/*����')',��1*/, value);
			strcat(pSubs->UsedName, buff);
			/*-------------------------------*/
			memset(buff, 0, sizeof(buff));
			memcpy(buff, pPointStart + len + 1, pSubs->UsedName + strlen(pSubs->UsedName) - (pPointStart + len + 1));
			memset(pPointStart, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pPointStart);
			strcat(pSubs->UsedName, "(");
			strcat(pSubs->UsedName, buff);
		}
		else
		{
			strcpy(buff, pPointStart + 1);
			memset(pPointStart, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pPointStart);
			strcat(pSubs->UsedName, buff);
		}

	}
}
//�ڸ���Ϊ1��Ԫ�غ����'1';
void CallUpdateSubsName(SUBSTANCE*pSubs)
{
	char str[128];
	int j = 0,i;
	memset(str, 0, sizeof(str));
	for (i = 0; i < strlen(pSubs->UsedName); i++)
	{
		str[j] = pSubs->UsedName[i];
		j++;
		if ((pSubs->UsedName[i] >= 'a'&&
			pSubs->UsedName[i] <= 'z') ||
			(pSubs->UsedName[i] >= 'A'&&
			pSubs->UsedName[i] <= 'Z'))
		{
			//��ǰԪ�ؽ�βû������;
			if ((pSubs->UsedName[i + 1] >= 'A'&&
				pSubs->UsedName[i + 1] <= 'Z') ||
				pSubs->UsedName[i + 1] == 0 ||
				pSubs->UsedName[i + 1] == '(' ||
				pSubs->UsedName[i + 1] == ')')
			{
				str[j] = '1';
				j++;
			}
		}
	}
	memset(pSubs->UsedName, 0, sizeof(pSubs->UsedName));
	strcpy(pSubs->UsedName, str);
}
//ȥ�������е�����;
BOOL RemoveParentheses(SUBSTANCE*pSubs)
{
	char buff[128];
	char*pParenthesesLeft, *pParenthesesRight;
	while (1)
	{

		pParenthesesLeft = pParenthesesRight = pSubs->UsedName;
		//ת�Ƶ����ڲ����Ŵ�;
		while (*pParenthesesRight != ')'&&*pParenthesesRight != NULL)
		{
			if (*pParenthesesRight == '(')
				pParenthesesLeft = pParenthesesRight;
			pParenthesesRight++;
		}
		//û���ұ�����;
		if (*pParenthesesRight == NULL)
		{
			//���ǻ����������;
			if (*pParenthesesLeft == '(')
				//��ƥ��;
				return FALSE;
			//������û�����ţ��˳�ѭ��;
			break;
		}
		//�ҵ������ţ������������ţ�����;
		if (*pParenthesesLeft != '(')
			return FALSE;
		//�����ұ�Ϊ����;
		if (*(pParenthesesRight + 1) >= '0'&&*(pParenthesesRight + 1) <= '9')
		{
			char *p;
			int Value, Len;
			Len = StrToNum(pParenthesesRight + 1, &Value);

			memset(buff, 0, sizeof(buff));
			//�������ұ������ұ߲��ָ���;
			strcpy(buff, pParenthesesRight + Len + 1);
			//��������ұ�;
			memset(pParenthesesRight + 1, 0, pSubs->UsedName + strlen(pSubs->UsedName) - (pParenthesesRight + 1));
			//����buff(ʵ����ȥ�����ֲ���);
			strcat(pSubs->UsedName, buff);
			/*-----------------------------------------------------*/
			p = pParenthesesLeft + 1;
			//����������,�����ֳ������ź������;
			while (p < pParenthesesRight)
			{
				if (*p >= '0'&&*p <= '9')
				{
					char szNewNum[128] = { 0 };
					int nNow, lenNow, nNew, lenNew;
					//�仯�ĳ���;
					int dtLen;
					//��ȡ��ǰ���ֺͳ���;
					lenNow = StrToNum(p, &nNow);
					//�����µ�����;
					nNew = Value*nNow;
					//���µ�����תΪ�ַ���,���泤��;
					lenNew = NumToStr(szNewNum, nNew);
					//����仯�ĳ���;
					dtLen = lenNew - lenNow;
					memset(buff, 0, sizeof(buff));
					//����ǰ�����ұ߸��Ƶ�buff;
					strcpy(buff, p + lenNow);
					//�����ֺ�������Ϊ0;
					memset(p, 0, pSubs->UsedName + strlen(pSubs->UsedName) - p);
					//�����³���;
					strcat(pSubs->UsedName, szNewNum);
					//����buff;
					strcat(pSubs->UsedName, buff);
					//�����������ƶ�dtLen����;
					pParenthesesRight += dtLen;
					//p�ƶ��������ֺ���;
					p += lenNew;
				}
				else
					p++;
			}
		}
		//ȥ����������;
		memset(buff, 0, sizeof(buff));
		strcpy(buff, pParenthesesRight + 1);
		memset(pParenthesesRight, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pParenthesesRight);
		strcat(pSubs->UsedName, buff);
		/*--------------------------------------------------------------------------------------------*/
		memset(buff, 0, sizeof(buff));
		strcpy(buff, pParenthesesLeft + 1);
		memset(pParenthesesLeft, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pParenthesesLeft);
		strcat(pSubs->UsedName, buff);
	}
	return TRUE;
}
//�ָ��������Ԫ��;
void DevideElement(SUBSTANCE*pSubs)
{
	char *pEleStart, *pEleEnd;
	pEleStart = pEleEnd = pSubs->UsedName;
	while (*pEleStart != NULL)
	{
		//�����ַָ�Ԫ��;
		if (*pEleEnd >= '0'&&*pEleEnd <= '9')
		{
			int Value, Len;
			ELEMENT*pEle;
			char EleName[64] = { 0 };
			memcpy(EleName, pEleStart, pEleEnd - pEleStart);
			pEle = InElementList(EleName, pSubs->elements);
			if (pEle == NULL)
			{
				ELEMENT*pEle = NULL;
				pEle = (ELEMENT*)malloc(sizeof(ELEMENT));
				pEle->next = pSubs->elements;
				pSubs->elements = pEle;
				memset(pEle->Name, 0, sizeof(pEle->Name));
				memcpy(pEle->Name, pEleStart, pEleEnd - pEleStart);
				Len = StrToNum(pEleEnd, &pEle->Num);
			}
			else
			{
				Len = StrToNum(pEleEnd, &Value);
				pEle->Num += Value;
			}
			pEleEnd += Len;
			pEleStart = pEleEnd;
		}
		else
			pEleEnd++;
	}
}
//�������Ԫ�������Ƿ��غ�;
BOOL IsCovered()
{
	//������ʽ��ߵ�Ԫ����ӵ�AllEleClaslist��;
	SUBSTANCE*pSubs;
	int RightEleClasCount = 0;
	ELEMENT*pEle;
	for (pSubs = LeftSubsHead; pSubs != NULL; pSubs = pSubs->next)
	{
		for (pEle = pSubs->elements; pEle != NULL; pEle = pEle->next)
		{
			if (NULL == InElementList(pEle->Name, AllEleCLas))
			{
				ELEMENT*pNewEle = (ELEMENT*)malloc(sizeof(ELEMENT));
				memset(pNewEle, 0, sizeof(ELEMENT));
				strcpy(pNewEle->Name, pEle->Name);
				pNewEle->Num = 1;
				pNewEle->next = AllEleCLas;
				AllEleCLas = pNewEle;
				AllEleClasCount++;
			}
		}
	}
	//�뷽��ʽ�ұߵ�Ԫ�رȽ�;

	for (pSubs = RightSubsHead; pSubs != NULL; pSubs = pSubs->next)
	{
		for (pEle = pSubs->elements; pEle != NULL; pEle = pEle->next)
		{
			ELEMENT*ele = InElementList(pEle->Name, AllEleCLas);
			if (NULL == ele)
			{
				
				strcpy(szRetEquation , "Error: the types of elements on the left and right sides of the equation are different!");
				char *pTemp = convertGB2312ToUTF8(szRetEquation);
				memset(szRetEquation,0,sizeof(szRetEquation));
				strcpy(szRetEquation,pTemp);
				delete [] pTemp;
				return FALSE;
			}
			else
				ele->Num++;
		}
	}
	for (pEle = AllEleCLas; pEle != NULL; pEle = pEle->next)
	{
		if (pEle->Num == 1)
		{
			strcpy(szRetEquation, "Error: the types of elements on the left and right sides of the equation are different!");
			char *pTemp = convertGB2312ToUTF8(szRetEquation);
			memset(szRetEquation,0,sizeof(szRetEquation));
			strcpy(szRetEquation,pTemp);
			delete [] pTemp;
			return FALSE;
		}
	}
	return TRUE;
}
//����ÿһ������;
BOOL BeginProcessStr()
{
	SUBSTANCEPOINTER*pSubsAddr;
	for (pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
	{
		//����ת��Ϊ����;
		CallChangePoint(pSubsAddr->pAddress);
		//�ڸ���Ϊ1��Ԫ�غ����'1'
		CallUpdateSubsName(pSubsAddr->pAddress);
		//ȥ�������е�����;
		if (!RemoveParentheses(pSubsAddr->pAddress))
		{
			strcpy(szRetEquation, "Error: brackets do not match!");
			char *pTemp = convertGB2312ToUTF8(szRetEquation);
			memset(szRetEquation,0,sizeof(szRetEquation));
			strcpy(szRetEquation,pTemp);
			delete [] pTemp;
			return FALSE;
		}
		//�ָ��������Ԫ��;
		DevideElement(pSubsAddr->pAddress);
	}
	return TRUE;
}
//���ɷ�����;
void MakeEquationSet()
{
	char buff[128] = { 0 };
	ELEMENT*pCurEle;
	for (pCurEle = AllEleCLas; pCurEle != NULL; pCurEle = pCurEle->next)
	{
		SUBSTANCEPOINTER*pSubsAddr;
		for (pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
		{
			ELEMENT*pEle;
			//���������Ƿ����ұߵĵ�һ������;
			if (pSubsAddr->pAddress == RightSubsHead && strlen(szEquationSet) != 0)
			{
				strcpy(szEquationSet + strlen(szEquationSet) - 1, "==");
			}
			for (pEle = pSubsAddr->pAddress->elements; pEle != NULL; pEle = pEle->next)
			{
				if (!strcmp(pCurEle->Name, pEle->Name))
				{
					sprintf(buff, "%d%s+", pEle->Num, pSubsAddr->pAddress->kName);
					strcat(szEquationSet, buff);
				}
			}
			//���������Ƿ��������һ������;
			if (pSubsAddr == AllSubsAddrsTail && strlen(szEquationSet) != 0)
			{
				strcpy(szEquationSet + strlen(szEquationSet) - 1, ";");
			}
		}
	}
}
//�ͷ��ڴ�;
void FreeMemory()
{
	SUBSTANCEPOINTER*pSubsAddr;
	for (pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL;)
	{
		SUBSTANCEPOINTER*tp;
		ELEMENT*pEle;
		for (pEle = pSubsAddr->pAddress->elements; pEle != NULL;)
		{
			ELEMENT *tp = pEle;
			pEle = pEle->next;
			free(tp);
		}
		free(pSubsAddr->pAddress);
		tp = pSubsAddr;
		pSubsAddr = pSubsAddr->next;
		free(tp);
	}
}
//��ʾ�������;
void ShowProcess()
{
	SUBSTANCEPOINTER*pSubsAddr;
	for (pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
	{
		ELEMENT*pEle;
		if (pSubsAddr->pAddress == LeftSubsHead)
			printf("-----------------------------Left----------------------------\n\n");
		else if (pSubsAddr->pAddress == RightSubsHead)
			printf("\n----------------------------Right----------------------------\n\n");
		printf("::K:%s\tName:%s\tUsedName:%s\n", pSubsAddr->pAddress->kName, pSubsAddr->pAddress->Name, pSubsAddr->pAddress->UsedName);
		printf("---------------------------elements--------------------------\n");
		for (pEle = pSubsAddr->pAddress->elements; pEle != NULL; pEle = pEle->next)
		{
			printf("::::element:%s\tnum:%d\n", pEle->Name, pEle->Num);
		}
		printf("-------------------------------------------------------------\n");
	}
	printf("-------------------------EquationSet-------------------------\n");
	printf(szEquationSet);
	printf("\n-------------------------------------------------------------\n");
}
//����
void EndChemistryEquationStringProcess()
{
	//�ͷ��ڴ�;
	FreeMemory();
}

//����ѧ�����ַ���;
BOOL ChemistryEquationStringProcess(char*szChemistryEquation)
{
	//����ַ�����;
	Init();
	//--------------------------------------------------------------------------
	//char szChemistryEquation[] = "Al2(SO4)3+NH3.H2O====Al(OH)3+(NH4)2SO4";
	strcpy(szEquation, szChemistryEquation);
	//--------------------------------------------------------------------------
	//�ָ��ʽ,ʧ�ܷ���FALSE,�ɹ�����TRUE;
	if (!DevideEquation())
		return FALSE;
	//�ָ��������;
	if (!DevideSubstance())
		return FALSE;
	//����ÿһ������;
	if (!BeginProcessStr())
		return FALSE;
	//�������Ԫ�������Ƿ��غ�;
	if (!IsCovered())
	{
		strcat(szRetEquation,"Element species are not conserved!");
		return FALSE;
	}
	//���ɷ�����;
	MakeEquationSet();
	//��ʾ�������;
	if(bShowProcess)
		ShowProcess();
	return TRUE;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
typedef int BOOL;
#define TRUE 1
#define FALSE 0

typedef struct _ELEMENT
{
	char Name[128];		//Ԫ������
	int Num;		//����
	struct _ELEMENT *next;
}ELEMENT;

typedef struct _SUBSTANCE
{
	char kName[64];		//��ϵ������Ԫ
	char Name[128];		//��������
	char UsedName[128];	//Used Name
	ELEMENT *elements;	//������ʵ�Ԫ��
	struct _SUBSTANCE *next;
}SUBSTANCE;

typedef struct _SUBSTANCEPOINTER
{
	SUBSTANCE*pAddress;
	struct _SUBSTANCEPOINTER*next;
}SUBSTANCEPOINTER;


char kName = 'A';		//ÿ������ǰ��Ԫ

SUBSTANCE *LeftSubsHead = 0;	//������ߵ�����
SUBSTANCE *LeftSubsTail = 0;

SUBSTANCE *RightSubsHead = 0;	//�����ұߵ�����
SUBSTANCE *RightSubsTail = 0;	//�����ұߵ�����

ELEMENT*AllEleCLas = 0;			//�������е�Ԫ��
int AllEleClasCount = 0;		//��¼Ԫ�صĸ���

SUBSTANCEPOINTER *AllSubsAddrsHead = NULL;		//��¼����Ԫ�صĵ�ַ
SUBSTANCEPOINTER *AllSubsAddrsTail = NULL;

char szEquationSet[128];		//�������ɵķ�����
char szEquation[2048];			//���淽��ʽ
char szRetEquation[2048];		//���淽��ʽ
char szLeftEqua[1024];			//�󲿷ַ���ʽ
char szRightEqua[1024];			//�Ҳ��ַ���ʽ
//�Ƿ���Ԫ��������
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
//���ַ�����ʼ���ֵ������ַ�תΪ����,���س���
int StrToNum(char *str, int *n)
{
	char *p = str;
	int len = 0;
	*n = 0;
	while (*p >= '0'&&*p <= '9')
		p++, len++;
	p = str;
	for (int i = 0; i < len; i++, p++)
	{
		*n += (int)(*p - '0')*pow(10.0, (double)len - 1 - i);
	}
	return len;
}
//������תΪ�ַ���,���س���
int NumToStr(char *buff, int n)
{
	sprintf(buff, "%d", n);
	return strlen(buff);
}
//����ַ�����
void Init()
{
	memset(szEquation, 0, sizeof(szEquation));
	memset(szEquation, 0, sizeof(szRetEquation));
	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));
}
//�ָ��ʽ,ʧ�ܷ���FALSE,�ɹ�����TRUE
BOOL DevideEquation()
{

	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));

	char *p = szEquation;
	//�Ⱥ����û�ж���
	if (*p == '=')
		return FALSE;
	while (*p != '='&& *p != NULL) p++;
	//û�е��ں�
	if (*p == NULL)
		return FALSE;
	//���淽��ʽ�󲿷�
	memcpy(szLeftEqua, szEquation, p - szEquation);
	//�Ƶ�����ʽ�Ҳ��ֿ�ʼ��
	while (*p == '='&& *p != NULL) p++;
	//�Ⱥ��ұ�û�ж���
	if (*p == NULL)
		return FALSE;
	//�����ұߵķ���ʽ
	strcpy(szRightEqua, p);
	//�������ָ��������
	strcat(szLeftEqua, "+");
	strcat(szRightEqua, "+");
	return TRUE;
}
//�ָ��������
BOOL CallDevideSubstance(char*szPartEquation, SUBSTANCE**SubsListHead, SUBSTANCE**SubsListTail)
{
	char*pSubsStart, *pSubsEnd;
	pSubsStart = pSubsEnd = szPartEquation;
	while (*pSubsEnd != NULL)
	{
		//��һ��Ԫ�ص�β��
		if (*pSubsEnd == '+')
		{
			//��ӵ�SubsList��
			//βΪ��,ͷ������
			if (*SubsListTail == NULL)
			{
				*SubsListHead = *SubsListTail = (SUBSTANCE*)malloc(sizeof(SUBSTANCE));
			}
			//β��Ϊ��,��β�ĺ������
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

			//������SUBSTANCE��ֵ
			memset((*SubsListTail)->kName, 0, sizeof((*SubsListTail)->kName));
			memset((*SubsListTail)->Name, 0, sizeof((*SubsListTail)->Name));
			memset((*SubsListTail)->UsedName, 0, sizeof((*SubsListTail)->UsedName));

			(*SubsListTail)->elements = NULL;
			(*SubsListTail)->next = NULL;
			*(*SubsListTail)->kName = kName++;

			//�����Ѿ���д��ϵ��
			while (*pSubsStart >= '0' && *pSubsStart <= '9')
				pSubsStart++;
			//������,û��Ԫ������
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
	//�ָ���ߵ�����
	if (!CallDevideSubstance(szLeftEqua, &LeftSubsHead, &LeftSubsTail))
		return FALSE;
	//�ָ��ұߵ�����
	if (!CallDevideSubstance(szRightEqua, &RightSubsHead, &RightSubsTail))
		return FALSE;
	return TRUE;
}
//����ת��Ϊ����
void CallChangePoint(SUBSTANCE*pSubs)
{
	char *pPointStart, *pPointEnd;
	while (1)
	{
		pPointEnd = pSubs->UsedName;
		//���ҿ�ʼ��
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;
		pPointStart = pPointEnd;
		//�����ڵ�
		if (*pPointStart == NULL)
			break;
		pPointEnd++;
		//���ҽ�����
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;

		//��ʼת����
		int value = 0, len = 0;
		char buff[128] = { 0 };
		//��������Ϊ����
		if (*(pPointStart + 1) >= '0' && *(pPointStart + 1) <= '9')
		{
			//��������󣨰����㣩���Ƶ�buff��
			memcpy(buff, pPointEnd, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			//��������󣨰����㣩��Ϊ0
			memset(pPointEnd, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			strcat(pSubs->UsedName, ")");
			len = StrToNum(pPointStart + 1, &value);
			//������ֵ�����
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
//�ڸ���Ϊ1��Ԫ�غ����'1'
void CallUpdateSubsName(SUBSTANCE*pSubs)
{
	char str[128];
	memset(str, 0, sizeof(str));
	int j = 0;
	for (int i = 0; i < strlen(pSubs->UsedName); i++)
	{
		str[j] = pSubs->UsedName[i];
		j++;
		if ((pSubs->UsedName[i] >= 'a'&&
			pSubs->UsedName[i] <= 'z') ||
			(pSubs->UsedName[i] >= 'A'&&
			pSubs->UsedName[i] <= 'Z'))
		{
			//��ǰԪ�ؽ�βû������
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
//ȥ�������е�����
BOOL RemoveParentheses(SUBSTANCE*pSubs)
{
	char buff[128];
	char*pParenthesesLeft, *pParenthesesRight;
	while (1)
	{

		pParenthesesLeft = pParenthesesRight = pSubs->UsedName;
		//ת�Ƶ����ڲ����Ŵ�
		while (*pParenthesesRight != ')'&&*pParenthesesRight != NULL)
		{
			if (*pParenthesesRight == '(')
				pParenthesesLeft = pParenthesesRight;
			pParenthesesRight++;
		}
		//û���ұ�����
		if (*pParenthesesRight == NULL)
		{
			//���ǻ����������
			if (*pParenthesesLeft == '(')
				//��ƥ��
				return FALSE;
			//������û�����ţ��˳�ѭ��
			break;
		}
		//�ҵ������ţ������������ţ�����
		if (*pParenthesesLeft != '(')
			return FALSE;
		//�����ұ�Ϊ����
		if (*(pParenthesesRight + 1) >= '0'&&*(pParenthesesRight + 1) <= '9')
		{
			int Value, Len;
			Len = StrToNum(pParenthesesRight + 1, &Value);

			memset(buff, 0, sizeof(buff));
			//�������ұ������ұ߲��ָ���
			strcpy(buff, pParenthesesRight + Len + 1);
			//��������ұ�
			memset(pParenthesesRight + 1, 0, pSubs->UsedName + strlen(pSubs->UsedName) - (pParenthesesRight + 1));
			//����buff(ʵ����ȥ�����ֲ���)
			strcat(pSubs->UsedName, buff);
			/*-----------------------------------------------------*/
			char *p = pParenthesesLeft + 1;
			//����������,�����ֳ������ź������
			while (p < pParenthesesRight)
			{
				if (*p >= '0'&&*p <= '9')
				{
					int nNow, lenNow, nNew, lenNew;
					//�仯�ĳ���
					int dtLen;
					//��ȡ��ǰ���ֺͳ���
					lenNow = StrToNum(p, &nNow);
					//�����µ�����
					nNew = Value*nNow;
					char szNewNum[128] = { 0 };
					//���µ�����תΪ�ַ���,���泤��
					lenNew = NumToStr(szNewNum, nNew);
					//����仯�ĳ���
					dtLen = lenNew - lenNow;
					memset(buff, 0, sizeof(buff));
					//����ǰ�����ұ߸��Ƶ�buff
					strcpy(buff, p + lenNow);
					//�����ֺ�������Ϊ0
					memset(p, 0, pSubs->UsedName + strlen(pSubs->UsedName) - p);
					//�����³���
					strcat(pSubs->UsedName, szNewNum);
					//����buff
					strcat(pSubs->UsedName, buff);
					//�����������ƶ�dtLen����
					pParenthesesRight += dtLen;
					//p�ƶ��������ֺ���
					p += lenNew;
				}
				else
					p++;
			}
		}
		//ȥ����������
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
//�ָ��������Ԫ��
void DevideElement(SUBSTANCE*pSubs)
{
	char *pEleStart, *pEleEnd;
	pEleStart = pEleEnd = pSubs->UsedName;
	while (*pEleStart != NULL)
	{
		//�����ַָ�Ԫ��
		if (*pEleEnd >= '0'&&*pEleEnd <= '9')
		{
			int Value, Len;

			char EleName[64] = { 0 };
			memcpy(EleName, pEleStart, pEleEnd - pEleStart);
			ELEMENT*pEle = InElementList(EleName, pSubs->elements);
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
//�������Ԫ�������Ƿ��غ�
BOOL IsCovered()
{
	//������ʽ��ߵ�Ԫ����ӵ�AllEleClaslist��
	for (SUBSTANCE*pSubs = LeftSubsHead; pSubs != NULL; pSubs = pSubs->next)
	{
		for (ELEMENT*pEle = pSubs->elements; pEle != NULL; pEle = pEle->next)
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
	//�뷽��ʽ�ұߵ�Ԫ�رȽ�
	int RightEleClasCount = 0;
	for (SUBSTANCE*pSubs = RightSubsHead; pSubs != NULL; pSubs = pSubs->next)
	{
		for (ELEMENT*pEle = pSubs->elements; pEle != NULL; pEle = pEle->next)
		{
			ELEMENT*ele = InElementList(pEle->Name, AllEleCLas);
			if (NULL == ele)
			{
				strcpy(szRetEquation, "ERROR:����ʽ��������Ԫ�����಻��ͬ!");
				return FALSE;
			}
			else
				ele->Num++;
		}
	}
	for (ELEMENT*pEle = AllEleCLas; pEle != NULL; pEle = pEle->next)
	{
		if (pEle->Num == 1)
		{
			strcpy(szRetEquation, "ERROR:����ʽ��������Ԫ�����಻��ͬ!");
			return FALSE;
		}
	}
	return TRUE;
}
//����ÿһ������
BOOL BeginProcessStr()
{
	for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
	{
		//����ת��Ϊ����
		CallChangePoint(pSubsAddr->pAddress);
		//�ڸ���Ϊ1��Ԫ�غ����'1'
		CallUpdateSubsName(pSubsAddr->pAddress);
		//ȥ�������е�����
		if (!RemoveParentheses(pSubsAddr->pAddress))
		{
			strcpy(szRetEquation, "ERROR:���Ų�ƥ��!");
			return FALSE;
		}
		//�ָ��������Ԫ��
		DevideElement(pSubsAddr->pAddress);
	}
	return TRUE;
}
//���ɷ�����
void MakeEquationSet()
{
	char buff[128] = { 0 };
	for (ELEMENT*pCurEle = AllEleCLas; pCurEle != NULL; pCurEle = pCurEle->next)
	{
		for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
		{
			//���������Ƿ����ұߵĵ�һ������
			if (pSubsAddr->pAddress == RightSubsHead && strlen(szEquationSet) != 0)
			{
				strcpy(szEquationSet + strlen(szEquationSet) - 1, "==");
			}

			for (ELEMENT*pEle = pSubsAddr->pAddress->elements; pEle != NULL; pEle = pEle->next)
			{
				if (!strcmp(pCurEle->Name, pEle->Name))
				{
					sprintf(buff, "%d%s+", pEle->Num, pSubsAddr->pAddress->kName);
					strcat(szEquationSet, buff);
				}
			}
			//���������Ƿ��������һ������
			if (pSubsAddr == AllSubsAddrsTail && strlen(szEquationSet) != 0)
			{
				strcpy(szEquationSet + strlen(szEquationSet) - 1, ";");
			}
		}
	}
}

//�ͷ��ڴ�
void FreeMemory()
{
	for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL;)
	{

		for (ELEMENT*pEle = pSubsAddr->pAddress->elements; pEle != NULL;)
		{
			ELEMENT *tp = pEle;
			pEle = pEle->next;
			free(tp);
		}
		free(pSubsAddr->pAddress);
		SUBSTANCEPOINTER*tp = pSubsAddr;
		pSubsAddr = pSubsAddr->next;
		free(tp);
	}
}
//��ʾ�������
void ShowProcess()
{
	for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
	{
		if (pSubsAddr->pAddress == LeftSubsHead)
			printf("-----------------------------Left----------------------------\n\n");
		else if (pSubsAddr->pAddress == RightSubsHead)
			printf("\n----------------------------Right----------------------------\n\n");
		printf("::K:%s\tName:%s\tUsedName:%s\n", pSubsAddr->pAddress->kName, pSubsAddr->pAddress->Name, pSubsAddr->pAddress->UsedName);
		printf("---------------------------elements--------------------------\n");
		for (ELEMENT*pEle = pSubsAddr->pAddress->elements; pEle != NULL; pEle = pEle->next)
		{
			printf("::::element:%s\tnum:%d\n", pEle->Name, pEle->Num);
		}
		printf("-------------------------------------------------------------\n");
	}
	printf("-------------------------EquationSet-------------------------\n");
	printf(szEquationSet);
	printf("\n-------------------------------------------------------------\n");
}
BOOL main()
{
	BOOL IsSuccess = TRUE;
	//����ַ�����
	Init();
	//--------------------------------------------------------------------------
	char szChemistryEquation[] = "Al2(SO4)3+NH3.H2O====Al(OH)3+(NH4)2SO4";
	strcpy(szEquation, szChemistryEquation);
	//--------------------------------------------------------------------------
	//�ָ��ʽ,ʧ�ܷ���FALSE,�ɹ�����TRUE
	if (!DevideEquation())
	{
		IsSuccess = FALSE;
		goto failure;
	}
		
	//�ָ��������
	if (!DevideSubstance())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//����ÿһ������
	if (!BeginProcessStr())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//�������Ԫ�������Ƿ��غ�
	if (!IsCovered())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//���ɷ�����
	MakeEquationSet();
	//��ʾ�������
	ShowProcess();
failure:
	//�ͷ��ڴ�
	FreeMemory();
	return IsSuccess;
}
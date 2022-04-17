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
	char Name[128];		//元素名称
	int Num;		//数量
	struct _ELEMENT *next;
}ELEMENT;

typedef struct _SUBSTANCE
{
	char kName[64];		//给系数设置元
	char Name[128];		//物质名称
	char UsedName[128];	//Used Name
	ELEMENT *elements;	//组成物质的元素
	struct _SUBSTANCE *next;
}SUBSTANCE;

typedef struct _SUBSTANCEPOINTER
{
	SUBSTANCE*pAddress;
	struct _SUBSTANCEPOINTER*next;
}SUBSTANCEPOINTER;


char kName = 'A';		//每种物质前的元

SUBSTANCE *LeftSubsHead = 0;	//储存左边的物质
SUBSTANCE *LeftSubsTail = 0;

SUBSTANCE *RightSubsHead = 0;	//储存右边的物质
SUBSTANCE *RightSubsTail = 0;	//储存右边的物质

ELEMENT*AllEleCLas = 0;			//储存所有的元素
int AllEleClasCount = 0;		//记录元素的个数

SUBSTANCEPOINTER *AllSubsAddrsHead = NULL;		//记录所有元素的地址
SUBSTANCEPOINTER *AllSubsAddrsTail = NULL;

char szEquationSet[128];		//储存生成的方程组
char szEquation[2048];			//储存方程式
char szRetEquation[2048];		//储存方程式
char szLeftEqua[1024];			//左部分方程式
char szRightEqua[1024];			//右部分方程式
//是否在元素链表中
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
//将字符串起始部分的数字字符转为整数,返回长度
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
//将整数转为字符串,返回长度
int NumToStr(char *buff, int n)
{
	sprintf(buff, "%d", n);
	return strlen(buff);
}
//清空字符数组
void Init()
{
	memset(szEquation, 0, sizeof(szEquation));
	memset(szEquation, 0, sizeof(szRetEquation));
	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));
}
//分割方程式,失败反回FALSE,成功返回TRUE
BOOL DevideEquation()
{

	memset(szLeftEqua, 0, sizeof(szLeftEqua));
	memset(szRightEqua, 0, sizeof(szRightEqua));

	char *p = szEquation;
	//等号左边没有东西
	if (*p == '=')
		return FALSE;
	while (*p != '='&& *p != NULL) p++;
	//没有等于号
	if (*p == NULL)
		return FALSE;
	//储存方程式左部分
	memcpy(szLeftEqua, szEquation, p - szEquation);
	//移到方程式右部分开始处
	while (*p == '='&& *p != NULL) p++;
	//等号右边没有东西
	if (*p == NULL)
		return FALSE;
	//储存右边的方程式
	strcpy(szRightEqua, p);
	//方便后面分割各种物质
	strcat(szLeftEqua, "+");
	strcat(szRightEqua, "+");
	return TRUE;
}
//分割各种物质
BOOL CallDevideSubstance(char*szPartEquation, SUBSTANCE**SubsListHead, SUBSTANCE**SubsListTail)
{
	char*pSubsStart, *pSubsEnd;
	pSubsStart = pSubsEnd = szPartEquation;
	while (*pSubsEnd != NULL)
	{
		//是一种元素的尾端
		if (*pSubsEnd == '+')
		{
			//添加到SubsList中
			//尾为空,头不存在
			if (*SubsListTail == NULL)
			{
				*SubsListHead = *SubsListTail = (SUBSTANCE*)malloc(sizeof(SUBSTANCE));
			}
			//尾不为空,在尾的后面添加
			else
			{
				(*SubsListTail)->next = (SUBSTANCE*)malloc(sizeof(SUBSTANCE));
				*SubsListTail = (*SubsListTail)->next;
			}
			/*--------------------------------添加到AllSubsAddrsList中----------------------------*/
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

			//设置新SUBSTANCE的值
			memset((*SubsListTail)->kName, 0, sizeof((*SubsListTail)->kName));
			memset((*SubsListTail)->Name, 0, sizeof((*SubsListTail)->Name));
			memset((*SubsListTail)->UsedName, 0, sizeof((*SubsListTail)->UsedName));

			(*SubsListTail)->elements = NULL;
			(*SubsListTail)->next = NULL;
			*(*SubsListTail)->kName = kName++;

			//忽略已经填写的系数
			while (*pSubsStart >= '0' && *pSubsStart <= '9')
				pSubsStart++;
			//常数项,没有元素名称
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
	//分割左边的物质
	if (!CallDevideSubstance(szLeftEqua, &LeftSubsHead, &LeftSubsTail))
		return FALSE;
	//分割右边的物质
	if (!CallDevideSubstance(szRightEqua, &RightSubsHead, &RightSubsTail))
		return FALSE;
	return TRUE;
}
//将点转换为括号
void CallChangePoint(SUBSTANCE*pSubs)
{
	char *pPointStart, *pPointEnd;
	while (1)
	{
		pPointEnd = pSubs->UsedName;
		//查找开始点
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;
		pPointStart = pPointEnd;
		//不存在点
		if (*pPointStart == NULL)
			break;
		pPointEnd++;
		//查找结束点
		while (*pPointEnd != '.' && *pPointEnd != NULL)
			pPointEnd++;

		//开始转换点
		int value = 0, len = 0;
		char buff[128] = { 0 };
		//如果点后面为数字
		if (*(pPointStart + 1) >= '0' && *(pPointStart + 1) <= '9')
		{
			//将结束点后（包括点）复制到buff中
			memcpy(buff, pPointEnd, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			//将结束点后（包括点）置为0
			memset(pPointEnd, 0, pSubs->UsedName + strlen(pSubs->UsedName) - pPointEnd);
			strcat(pSubs->UsedName, ")");
			len = StrToNum(pPointStart + 1, &value);
			//添加数字到后面
			NumToStr(pPointEnd + 1/*多了')',加1*/, value);
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
//在个数为1的元素后面加'1'
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
			//当前元素结尾没有数字
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
//去除物质中的括号
BOOL RemoveParentheses(SUBSTANCE*pSubs)
{
	char buff[128];
	char*pParenthesesLeft, *pParenthesesRight;
	while (1)
	{

		pParenthesesLeft = pParenthesesRight = pSubs->UsedName;
		//转移到最内层括号处
		while (*pParenthesesRight != ')'&&*pParenthesesRight != NULL)
		{
			if (*pParenthesesRight == '(')
				pParenthesesLeft = pParenthesesRight;
			pParenthesesRight++;
		}
		//没有右边括号
		if (*pParenthesesRight == NULL)
		{
			//但是还有左边括号
			if (*pParenthesesLeft == '(')
				//不匹配
				return FALSE;
			//正常，没有括号，退出循环
			break;
		}
		//找到右括号，但左面无括号，错误
		if (*pParenthesesLeft != '(')
			return FALSE;
		//括号右边为数字
		if (*(pParenthesesRight + 1) >= '0'&&*(pParenthesesRight + 1) <= '9')
		{
			int Value, Len;
			Len = StrToNum(pParenthesesRight + 1, &Value);

			memset(buff, 0, sizeof(buff));
			//将括号右边数字右边部分复制
			strcpy(buff, pParenthesesRight + Len + 1);
			//清除括号右边
			memset(pParenthesesRight + 1, 0, pSubs->UsedName + strlen(pSubs->UsedName) - (pParenthesesRight + 1));
			//加上buff(实际是去了数字部分)
			strcat(pSubs->UsedName, buff);
			/*-----------------------------------------------------*/
			char *p = pParenthesesLeft + 1;
			//遍历括号内,将数字乘上括号后的数字
			while (p < pParenthesesRight)
			{
				if (*p >= '0'&&*p <= '9')
				{
					int nNow, lenNow, nNew, lenNew;
					//变化的长度
					int dtLen;
					//获取当前数字和长度
					lenNow = StrToNum(p, &nNow);
					//计算新的数字
					nNew = Value*nNow;
					char szNewNum[128] = { 0 };
					//将新的数字转为字符串,保存长度
					lenNew = NumToStr(szNewNum, nNew);
					//计算变化的长度
					dtLen = lenNew - lenNow;
					memset(buff, 0, sizeof(buff));
					//将当前数字右边复制到buff
					strcpy(buff, p + lenNow);
					//将数字和右面置为0
					memset(p, 0, pSubs->UsedName + strlen(pSubs->UsedName) - p);
					//加上新长度
					strcat(pSubs->UsedName, szNewNum);
					//加上buff
					strcat(pSubs->UsedName, buff);
					//右括号向右移动dtLen长度
					pParenthesesRight += dtLen;
					//p移动到新数字后面
					p += lenNew;
				}
				else
					p++;
			}
		}
		//去掉两边括号
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
//分割物质组成元素
void DevideElement(SUBSTANCE*pSubs)
{
	char *pEleStart, *pEleEnd;
	pEleStart = pEleEnd = pSubs->UsedName;
	while (*pEleStart != NULL)
	{
		//用数字分割元素
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
//检测左右元素种类是否守恒
BOOL IsCovered()
{
	//将方程式左边的元素添加到AllEleClaslist中
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
	//与方程式右边的元素比较
	int RightEleClasCount = 0;
	for (SUBSTANCE*pSubs = RightSubsHead; pSubs != NULL; pSubs = pSubs->next)
	{
		for (ELEMENT*pEle = pSubs->elements; pEle != NULL; pEle = pEle->next)
		{
			ELEMENT*ele = InElementList(pEle->Name, AllEleCLas);
			if (NULL == ele)
			{
				strcpy(szRetEquation, "ERROR:方程式左右两边元素种类不相同!");
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
			strcpy(szRetEquation, "ERROR:方程式左右两边元素种类不相同!");
			return FALSE;
		}
	}
	return TRUE;
}
//处理每一种物质
BOOL BeginProcessStr()
{
	for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
	{
		//将点转换为括号
		CallChangePoint(pSubsAddr->pAddress);
		//在个数为1的元素后面加'1'
		CallUpdateSubsName(pSubsAddr->pAddress);
		//去除物质中的括号
		if (!RemoveParentheses(pSubsAddr->pAddress))
		{
			strcpy(szRetEquation, "ERROR:括号不匹配!");
			return FALSE;
		}
		//分割物质组成元素
		DevideElement(pSubsAddr->pAddress);
	}
	return TRUE;
}
//生成方程组
void MakeEquationSet()
{
	char buff[128] = { 0 };
	for (ELEMENT*pCurEle = AllEleCLas; pCurEle != NULL; pCurEle = pCurEle->next)
	{
		for (SUBSTANCEPOINTER*pSubsAddr = AllSubsAddrsHead; pSubsAddr != NULL; pSubsAddr = pSubsAddr->next)
		{
			//这种物质是方程右边的第一种物质
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
			//这种物质是方程中最后一种物质
			if (pSubsAddr == AllSubsAddrsTail && strlen(szEquationSet) != 0)
			{
				strcpy(szEquationSet + strlen(szEquationSet) - 1, ";");
			}
		}
	}
}

//释放内存
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
//显示处理过程
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
	//清空字符数组
	Init();
	//--------------------------------------------------------------------------
	char szChemistryEquation[] = "Al2(SO4)3+NH3.H2O====Al(OH)3+(NH4)2SO4";
	strcpy(szEquation, szChemistryEquation);
	//--------------------------------------------------------------------------
	//分割方程式,失败反回FALSE,成功返回TRUE
	if (!DevideEquation())
	{
		IsSuccess = FALSE;
		goto failure;
	}
		
	//分割各种物质
	if (!DevideSubstance())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//处理每一种物质
	if (!BeginProcessStr())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//检测左右元素种类是否守恒
	if (!IsCovered())
	{
		IsSuccess = FALSE;
		goto failure;
	}
	//生成方程组
	MakeEquationSet();
	//显示处理过程
	ShowProcess();
failure:
	//释放内存
	FreeMemory();
	return IsSuccess;
}
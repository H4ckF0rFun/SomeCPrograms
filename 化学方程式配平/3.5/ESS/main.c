#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define TYPE_UNKNOWN	1
#define TYPE_CONSTANT	0
#define SUCCESS 0
#define NEXT	1
#define FAIL	-1
typedef int BOOL;
typedef unsigned int UINT;
#define TRUE	1
#define FALSE	0
//分数类型;
typedef struct _NUM
{
	int Molecule;				//分子;
	UINT Denominator;			//分母,都为正;
}NUM;
typedef struct _Item
{
	int  nType;					//是否为未知项;
	char szName[32];			//名称;
	NUM k;						//系数;
	struct _Item* next;
}Item;
typedef struct _Equation
{
	Item*FirstItem;				//储存各个项;
	struct _Equation* next;
}Equation;
typedef struct _EquationSet
{
	Equation* FirstEquation;	//储存方程式;
	struct _EquationSet* next;
}EquationSet;
//保存结果;
typedef struct _Unknown
{
	char Name[32];
	NUM value;
	struct _Unknown*next;
}Unknown;
//储存输入的方程式;
typedef struct _EquationString
{
	char szEquation[256];
	char szEquationLeft[128];
	char szEquationRight[128];
	struct _EquationString* next;
}EquationString;
//储存多组解;
typedef struct _Ret
{
	Unknown*unknown;
	struct _Ret*next;
}Ret;
EquationSet *EquationSetHead = NULL;				//储存方程组的结构;
EquationString * EquationStrHead = NULL;			//储存输入的方程组字符串;
Unknown *AllUnknown = NULL;							//储存所有元的名称;
int UnknownCount = 0;								//记录元的个数;
Unknown *Definite = NULL;							//确定的元;
Unknown *Varied = NULL;								//主动变化的元;
Unknown *_Varied = NULL;							//被动变化的元;
Ret *RetSolution = NULL;							//结果链表首地址;
int MaxSolution = 20;								//最多求出几组解;
int CurSoluton = 0;									//已经求出的解的组数;
int MaxValue = 3;									//主动变化元最大值;
#define IsInAllUnknown(name)	InUnknownList(AllUnknown,name)
#define IsDefinite(name)		InUnknownList(Definite,name)
#define IsVaried(name)			InUnknownList(Varied,name)
#define Is_Varied(name)			InUnknownList(_Varied,name)
//统计确定元的个数;
int GetDefiniteNum()
{
	int n = 0;
	Unknown*uk;
	for (uk = Definite; uk != NULL; uk = uk->next)
		n++;
	return n;
}
//是否在unknownlist中;
Unknown* InUnknownList(Unknown*List, char *Name)
{
	Unknown *uk;
	for (uk = List; uk != NULL; uk = uk->next)
	{
		if (!strcmp(uk->Name, Name))
			return uk;
	}	
	return NULL;
}
//将字符串起始部分的数字字符转为整数,返回长度;
int StrToNum(char *str, int *n)
{
	char *p = str;
	int len = 0,i;
	*n = 0;
	while (*p >= '0'&&*p <= '9')
	{
		p++;
		len++;
	}
	p = str;
	for (i = 0; i < len; i++, p++)
	{
		*n += (int)(*p - '0')*pow(10.0, len - 1 - i);
	}
	return len;
}
//将整数转为字符串,返回长度;
int NumToStr(char *buff, int n)
{
	sprintf(buff, "%d", n);
	return strlen(buff);
}
//释放Unknownlist;
void DelUnknownList(Unknown*UkListHead)
{
	Unknown*TempUnknown, *pUk = UkListHead;
	while (pUk != NULL)
	{
		TempUnknown = pUk;
		pUk = pUk->next;
		free(TempUnknown);
	}
}
//释放Item;
#define DelItem(pItem) free(pItem)
//释放方程;
void DelEquation(Equation*eqt)
{
	Item*TempItem,*pItem = eqt->FirstItem;
	while (pItem != NULL)
	{
		TempItem = pItem;
		pItem = pItem->next;
		DelItem(TempItem);
	}
	free(eqt);
}
//释放方程组;
void DelEquationSet(EquationSet*EqtSet)
{
	Equation*TempEqt, *pEqt = EqtSet->FirstEquation;
	while (pEqt != NULL)
	{
		TempEqt = pEqt;
		pEqt = pEqt->next;
		DelEquation(TempEqt);
	}
	free(EqtSet);
}
//释放方程组的集合;
void DelEquationSetSet(EquationSet*EquationSetListHead)
{
	EquationSet*TempEqtSet, *pEqtSet = EquationSetListHead;
	while (pEqtSet != NULL)
	{
		TempEqtSet = pEqtSet;
		pEqtSet = pEqtSet->next;
		DelEquationSet(TempEqtSet);
	}
}
//释放储存的方程字符串;
void DelEquationStr()
{
	EquationString*EqtStr = EquationStrHead,*Temp;
	while(EqtStr)
	{
		Temp=EqtStr;
		EqtStr = EqtStr->next;
		free(Temp);
	}
}
//取字符串中的分子分母和元,存到Item中;
BOOL GetStrItem(char *str, Item*pItem)
{
	char *p = str;
	int StrLen = strlen(str),Sign;
	int Molecule, Denominator,MoleculeLen, DenominatorLen;
	if (StrLen == 0)					//长度为0无效，返回false;
		return FALSE;

	Sign = 1;
	if (*p == '-')
		Sign = -1;
	p++;
	if (*p < '0' || *p > '9')																			//第一个字符(除了正负号)不是数字,将整个字符串当作元名称;
	{
		pItem->nType = TYPE_UNKNOWN;
		strcpy(pItem->szName, p);
		pItem->k.Molecule = 1, pItem->k.Denominator = 1;												//分子分母都为1
		goto success;
	}
	//----------------------------------------------------------------------
	//储存分子;
	MoleculeLen = StrToNum(p, &Molecule);
	if (*(p + MoleculeLen) == NULL)																		//只有一个数是常数项;
	{
		pItem->nType = TYPE_CONSTANT;
		pItem->k.Molecule = Molecule, pItem->k.Denominator = 1;
		goto success;
	}
	if (*(p + MoleculeLen) == '/' && (*(p + MoleculeLen + 1) >= '0' && *(p + MoleculeLen + 1) <= '9'))	//找到分数线且分数线后是一个数;
	{
		DenominatorLen = StrToNum(p + MoleculeLen + 1, &Denominator);
		if ((DenominatorLen + MoleculeLen + 2/*分数线和第一个正负号*/) == StrLen)							//只有分数;
		{
			pItem->nType = TYPE_CONSTANT;
		}
		else																							//是分数和一个元;
		{
			pItem->nType = TYPE_UNKNOWN;
			strcpy(pItem->szName, p + MoleculeLen + DenominatorLen + 1);
		}
		pItem->k.Molecule = Molecule, pItem->k.Denominator = Denominator;
		if (pItem->k.Denominator == 0)		//分母为0，无效
			return FALSE;
		goto success;
	}
	else																								//一个整数加元
	{
		pItem->nType = TYPE_UNKNOWN;
		strcpy(pItem->szName, p + MoleculeLen);
		pItem->k.Denominator = 1, pItem->k.Molecule = Molecule;
		goto success;
	}
success:
	pItem->k.Molecule *= Sign;
	return TRUE;
}
//取绝对值;
UINT ABS(int n)
{
	return n > 0 ? n : -n;
}
//取最大公因数;
UINT GetMaxCommonFactor(int a, int b)
{
	int r = 0;
	a = ABS(a), b = ABS(b);
	if (a == 0 || b == 0)
		return 1;
	do
	{
		if (a < b) a = a^b, b = a^b, a = a^b;
		r = a%b;
		a = b, b = r;
	} while (r);
	return a;
}
//取最小公倍数;
UINT GetMinCommonMultiple(int a, int b)
{
	a = ABS(a), b = ABS(b);
	return a*b / GetMaxCommonFactor(a, b);
}
//在一个unknownlist中创建一项;
Unknown*CreateUnknownInUkList(Unknown**UkListHead)
{
	Unknown*pNew = (Unknown*)malloc(sizeof(Unknown));
	memset(pNew, 0, sizeof(Unknown));
	pNew->next = (*UkListHead);
	(*UkListHead) = pNew;
	return pNew;
}
//在一个方程组链表中创建一个方程组;
EquationSet * CreateEquationSetInEqtSetList(EquationSet**EqtSetListHead)
{
	//头插法
	EquationSet* pEqtSet = (EquationSet*)malloc(sizeof(EquationSet));
	pEqtSet->next = (*EqtSetListHead);
	(*EqtSetListHead) = pEqtSet;
	(*EqtSetListHead)->FirstEquation = NULL;
	return pEqtSet;
}
//在一个方程组中创建一个方程;
Equation *CreateEquationInEqtSet(EquationSet*EqtSet)
{
	//头插法
	Equation *pEqt = (Equation*)malloc(sizeof(Equation));
	pEqt->FirstItem = NULL;
	pEqt->next = EqtSet->FirstEquation;
	EqtSet->FirstEquation = pEqt;
	return pEqt;
}
//在一个方程组创建一个项;
Item* CreateItemInEqt(Equation*Eqt)
{
	//头插法
	Item *pItem = (Item*)malloc(sizeof(Item));
	memset(pItem, 0, sizeof(Item));
	pItem->next = Eqt->FirstItem;
	Eqt->FirstItem = pItem;
	return pItem;
}
//用';'分割方程组;
BOOL DevideEquationSet(char*szEquation)
{
	char *pStart, *pEnd;
	char *DevideSign;
	pStart = pEnd = szEquation;
	while (1)
	{
		if (*pStart == NULL)
		{
			break;
		}
		if (*pEnd == ';' || *pEnd == NULL)
		{
			EquationString*Eqt = (EquationString*)malloc(sizeof(EquationString));
			Eqt->next = EquationStrHead;
			EquationStrHead = Eqt;
			//将整个方程存到->szEquation中;
			memset(EquationStrHead->szEquation, 0, sizeof(EquationStrHead->szEquation));
			memcpy(EquationStrHead->szEquation, pStart, pEnd - pStart);
			memset(EquationStrHead->szEquationLeft, 0, sizeof(EquationStrHead->szEquationLeft));
			memset(EquationStrHead->szEquationRight, 0, sizeof(EquationStrHead->szEquationRight));
			if (EquationStrHead->szEquationLeft[0] != '+' && EquationStrHead->szEquationLeft[0] != '-')
				EquationStrHead->szEquationLeft[0] = '+';
			if (EquationStrHead->szEquationRight[0] != '+' && EquationStrHead->szEquationRight[0] != '-')
			EquationStrHead->szEquationRight[0] = '+';
			//----------------------------------------------------------------------------------
			//分割方程左右两部份;

			//找到等于号;
			DevideSign = EquationStrHead->szEquation;
			//等号左边没有东西;
			if (*DevideSign == '=') return FALSE;
			while (*DevideSign != '='&&*DevideSign != NULL) DevideSign++;
			//没有等于号;
			if (*DevideSign == NULL) return FALSE;
			//储存左边;
			memcpy(EquationStrHead->szEquationLeft+1, EquationStrHead->szEquation, DevideSign - EquationStrHead->szEquation);
			while (*DevideSign == '=' && *DevideSign != NULL) DevideSign++;
			// 等号右边没有东西;
			if (*DevideSign == NULL) return FALSE;
			strcpy(EquationStrHead->szEquationRight+1, DevideSign);
			//----------------------------------------------------------------------------------
			if (*pEnd != NULL)
				pEnd++;
			pStart = pEnd;
		}
		else
		{
			pEnd++;
		}
	}
	return TRUE;
}
//设置起始方程组;
BOOL SetStartEquationSet()
{
	char buff[256];
	Item*pItem;
	EquationSet*EqtSet = CreateEquationSetInEqtSetList(&EquationSetHead);
	EquationString*EqtStr;
	for (EqtStr = EquationStrHead; EqtStr != NULL; EqtStr = EqtStr->next)
	{
		Equation *NewEquation = CreateEquationInEqtSet(EqtSet);

		char *pStart,*pEnd;
		int i;
		for (i = 0; i < 2; i++)
		{
			//第一次分割左部分，第二次分割右部分;
			if (i == 0)
				pStart = EqtStr->szEquationLeft;
			else
				pStart = EqtStr->szEquationRight;
			pEnd = pStart + 1;
			
			while (1)
			{
				if (*pStart == NULL)
					break;
				if (*pEnd == '+' ||*pEnd=='-' ||*pEnd == NULL)
				{
					memset(buff, 0, sizeof(buff));
					memcpy(buff, pStart, pEnd - pStart);
					pItem = CreateItemInEqt(NewEquation);
					if (!GetStrItem(buff, pItem))
						return FALSE;						//无效的项;
					//第一次出现的元,在allunknownlist中添加;
					if (pItem->nType == TYPE_UNKNOWN && !IsInAllUnknown(pItem->szName))
					{
						Unknown*pUk = CreateUnknownInUkList(&AllUnknown);
						UnknownCount++;
						strcpy(pUk->Name, pItem->szName);
					}
					//分割右边的项每个系数乘-1;
					if (i == 1)
						pItem->k.Molecule *= -1;
					pStart = pEnd;
					if (pEnd != NULL)
						pEnd++;
				}
				else
				{
					pEnd++;
				}
			}
		}
	}
	return TRUE;
}
//化简每一项(约分);
void SimplyItem(Item*pItem)
{
	int MaxCommonFactor = GetMaxCommonFactor(pItem->k.Molecule, pItem->k.Denominator);
	pItem->k.Molecule /= MaxCommonFactor;
	pItem->k.Denominator /= MaxCommonFactor;
}
//是否为同类项;
BOOL IsSameClassItem(Item*pItem1, Item*pItem2)
{
	return ((pItem1->nType == pItem2->nType) && !strcmp(pItem1->szName, pItem2->szName));
}
//合并两项储存到第一项中;
void MergeItem(Item*pItem1, Item*pItem2)
{
	//计算分母的最小公倍数;
	int MinCommonMutiple = GetMinCommonMultiple(pItem1->k.Denominator, pItem2->k.Denominator);
	//通分;
	pItem1->k.Molecule *= MinCommonMutiple / pItem1->k.Denominator;
	pItem2->k.Molecule *= MinCommonMutiple / pItem2->k.Denominator;
	pItem1->k.Denominator = pItem2->k.Denominator = MinCommonMutiple;
	//分子相加;
	pItem1->k.Molecule += pItem2->k.Molecule;
}
//化简方程式;
void SimplyEquation(Equation*eqt)
{
	int MaxCommonFactor = 0;
	//-----------------------------------合并同类项----------------------------;
	Item*pItem1,*pItem2,*front,*pItem;
	for (pItem1 = eqt->FirstItem; pItem1 != NULL;pItem1=pItem1->next)
	{
		for (pItem2 = pItem1->next, front = pItem1; pItem2 != NULL;)
		{
			if (IsSameClassItem(pItem1,pItem2))
			{
				MergeItem(pItem1, pItem2);
				front->next = pItem2->next;
				DelItem(pItem2);
				pItem2 = front->next;
			}
			else
			{
				front = front->next;
				pItem2 = pItem2->next;
			}
		}
	}
	//-------------------------------------清除系数为0的项---------------------------;
	for (pItem = eqt->FirstItem,front=NULL; pItem != NULL; )
	{
		if (0 == pItem->k.Molecule)
		{
			if (front == NULL)
			{
				eqt->FirstItem = pItem->next;
				DelItem(pItem);
				pItem = eqt->FirstItem;
			}
			else
			{
				front->next = pItem->next;
				DelItem(pItem);
				pItem = front->next;
			}
		}
		else
		{
			if (front == NULL)
				front = eqt->FirstItem;
			else
				front = front->next;
			pItem = front->next;
		}
	}
	//每一项约分;
	for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		SimplyItem(pItem);
	}
	//获取所有项的最大公因数;
	for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		if (MaxCommonFactor == 0)
			MaxCommonFactor = pItem->k.Molecule;
		else
			MaxCommonFactor = GetMaxCommonFactor(MaxCommonFactor, pItem->k.Molecule);
	}
	//每一项同除以最大公因数;
	if (MaxCommonFactor != 1)
	{
		for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
		{
			pItem->k.Molecule /= MaxCommonFactor;
		}
	}
}
//获取常数项或元个数;
int GetConstOrUnknownNum(Equation*Eqt,int nType)
{
	int n=0;
	Item*pItem;
	for(pItem = Eqt->FirstItem;pItem!=NULL;pItem=pItem->next)
	{
		if(pItem->nType == nType)
			n++;
	}
	return n;
}
//方程中是否存在某一个元;
Item* EqtExistItem(Equation *eqt,int nType,char *szUnknownName)
{
	Item*pItem;
	for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		if ((nType == pItem->nType) && !strcmp(pItem->szName, szUnknownName))
			break;
	}
	return pItem;
}
////获取方程组出现次数最少的元,储存到Name中;
void GetLeastUnknown(EquationSet*EqtSet, char*Name,int size)
{
	Unknown *UnknInfo = NULL,*LeastUnknown = NULL,*pFindUnk = NULL,*pNewUnkn = NULL,*pUnknInfo = NULL;
	Equation*eqt;
	Item*pItem;
	//把所有的元储存到链表中并记录出现的次数;
	for (eqt = EqtSet->FirstEquation; eqt != NULL; eqt = eqt->next)
	{
		for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
		{
			if (pItem->nType == TYPE_UNKNOWN)
			{
				//查找是否存在;
				pFindUnk = InUnknownList(UnknInfo,pItem->szName);
				//不存在，加到链表中;
				if (pFindUnk == NULL)
				{
					pNewUnkn = CreateUnknownInUkList(&UnknInfo);
					pNewUnkn->value.Molecule = 1;
					strcpy(pNewUnkn->Name, pItem->szName);
				}
				else
				{
					pFindUnk->value.Molecule++;
				}
			}
		}
	}
	for (pUnknInfo = UnknInfo; pUnknInfo != NULL; pUnknInfo = pUnknInfo->next)
	{
		//如果是第一个且出现次数大于1;
		if (LeastUnknown == NULL && pUnknInfo->value.Molecule > 1)
				LeastUnknown = pUnknInfo;
		//否则，如果小于leastUnknown则设置LeastUnknown==pUnknInfo;
		else if (pUnknInfo->value.Molecule > 1 && pUnknInfo->value.Molecule<LeastUnknown->value.Molecule)
				LeastUnknown = pUnknInfo;
	}
	memset(Name, 0, size);
	if (LeastUnknown != NULL)//每个元只出现了一次为空;
		strcpy(Name, LeastUnknown->Name);
	//释放内存;
	DelUnknownList(UnknInfo);
}
//化解每个方程并删除方程组中的空方程;
void SimplyEquationSet(EquationSet*EqtSet)
{
	Equation*Eqt = NULL,*front = NULL;
	//删除空方程;
	for(Eqt = EqtSet->FirstEquation,front = NULL;Eqt!=NULL;)
	{
		SimplyEquation(Eqt);
		//空方程删除;
		if(Eqt->FirstItem==NULL)
		{
			if(front==NULL)
			{
				EqtSet->FirstEquation=Eqt->next;
				DelEquation(Eqt);
				Eqt = EqtSet->FirstEquation;
			}
			else
			{
				front->next=Eqt->next;
				DelEquation(Eqt);
				Eqt=front->next;
			}
		}
		else
		{
			if(front == NULL)
				front=EqtSet->FirstEquation;
			else
				front=front->next;
			Eqt=front->next;
		}
	}
}
//两个方程消元，反回新的方程;
Equation* ClearEquationUnknown(Equation*eqt1, Equation*eqt2,Item*pItem1,Item*pItem2)
{
	Item*pItem,*NewItem,*FindItem;
	Equation*NewEquation;
	int DenoMinCommonMultiple,MoleMinCommonMultiple,Eqt1Multiply,Eqt2Multiply;
	//记录两个方程相加还是相减;
	BOOL bChangeSign;
	bChangeSign = (pItem1->k.Molecule*pItem2->k.Molecule)>=0;	//同号;
	//取分母最小公倍数;
	DenoMinCommonMultiple = GetMinCommonMultiple(pItem1->k.Denominator, pItem2->k.Denominator);
	pItem1->k.Molecule*= DenoMinCommonMultiple / pItem1->k.Denominator;
	pItem2->k.Molecule *= DenoMinCommonMultiple / pItem2->k.Denominator;
	pItem1->k.Denominator= pItem2->k.Denominator = DenoMinCommonMultiple;
	//取分子最小公倍数;
	MoleMinCommonMultiple = GetMinCommonMultiple(pItem1->k.Molecule, pItem2->k.Molecule);
	//计算第一个方程每一项要乘的数;
	Eqt1Multiply = MoleMinCommonMultiple / ABS(pItem1->k.Molecule);
	//计算第一个方程每一项要乘的数;
	Eqt2Multiply = MoleMinCommonMultiple / ABS(pItem2->k.Molecule);
	//给第一个方程的每一项乘上Eqt1Multiply;
	for (pItem = eqt1->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		pItem->k.Molecule*=Eqt1Multiply;
	}
	//给第一个方程的每一项乘上Eqt1Multiply;
	for (pItem = eqt2->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		pItem->k.Molecule*=Eqt2Multiply;
		//如果同号,将方程2所有的符号取反;
		if (bChangeSign)
		{
			pItem->k.Molecule*=-1;
		}
	}
	//储存生成的新方程;
	NewEquation = (Equation*)malloc(sizeof(Equation));
	memset(NewEquation,0,sizeof(Equation));
	//先把eqt1复制到NewEquation中;
	for (pItem = eqt1->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		NewItem = CreateItemInEqt(NewEquation);
		memcpy(NewItem, pItem, sizeof(Item)-4/*减去四个字节（next指针）*/);
	}
	//遍历eqt2各个项,在NewEquation中查找是否存在,不存在添加,存在合并;
	for (pItem = eqt2->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		FindItem = EqtExistItem(NewEquation, pItem->nType, pItem->szName);
		if (FindItem == NULL)		//不存在;
		{
			//在NewEquation中添加该项
			Item*NewItem = CreateItemInEqt(NewEquation);
			memcpy(NewItem, pItem, sizeof(Item)-4/*减去四个字节（next指针）*/);
		}
		else						//存在该项;
			MergeItem(FindItem, pItem);
	}
	SimplyEquation(eqt1);
	SimplyEquation(eqt2);
	SimplyEquation(NewEquation);
	return NewEquation;
}
//方程组开始消元;
void BeginEliminationUnknown()
{
	char LeastUnknown[32] = { 0 };
	while(1)
	{
		Equation*eqt1,*eqt2,*front;
		EquationSet*EqtSet = EquationSetHead;
		int EqtSetExist = 0;
		SimplyEquationSet(EqtSet);
		//找出出现次数最少的元;
		GetLeastUnknown(EqtSet, LeastUnknown, sizeof(LeastUnknown));
		//不存在出现次数最少的元;
		if (*LeastUnknown == '\0')
			break;
		for (eqt1 = EqtSet->FirstEquation, front = NULL; eqt1 != NULL;)
		{
			Item*FindItem1 = EqtExistItem(eqt1, TYPE_UNKNOWN, LeastUnknown);
			Item*FindItem2;
			//标记只创建一个下一个方程组;
			//如果不存在最少次数元,把这个方程放到下一个方程组;
			if (FindItem1 == NULL)
			{
				if(EqtSetExist==0)
				{
					CreateEquationSetInEqtSetList(&EquationSetHead);
					EqtSetExist=1;
				}
				//移动这个方程,下一个方程组是EquationSetHead;
				if (front == NULL)
				{
					EqtSet->FirstEquation = eqt1->next;
					eqt1->next = EquationSetHead->FirstEquation;
					EquationSetHead->FirstEquation= eqt1;
					eqt1 = EqtSet->FirstEquation;
				}
				else
				{
					front->next = eqt1->next;
					eqt1->next = EquationSetHead->FirstEquation;
					EquationSetHead->FirstEquation= eqt1;
					eqt1 = front->next;
				}
			}
			else						//如果这个方程存在出现最少次数元，则从下一个方程开始寻找;
			{
				for (eqt2 = eqt1->next; eqt2 != NULL; eqt2 = eqt2->next)
				{
					FindItem2 = EqtExistItem(eqt2, TYPE_UNKNOWN, LeastUnknown);
					if (FindItem2 != NULL)
					{
						Equation*NewEquation = ClearEquationUnknown(eqt1,eqt2,FindItem1,FindItem2);
						//不存在下一个方程组添加;
						if (EqtSetExist==0)
						{
							CreateEquationSetInEqtSetList(&EquationSetHead);
							EqtSetExist=1;
						}
						NewEquation->next = EquationSetHead->FirstEquation;
						EquationSetHead->FirstEquation = NewEquation;
					}
				}	
				if (front == NULL)
					front = EqtSet->FirstEquation;
				else
					front = front->next;
				eqt1 = front->next;
			}
			
		}
	}
}
//将方程组中的方程按未知项个数排序;
void SortEquation(EquationSet*EqtSet)
{
	//记录方程个数;
	int EquationNum = 0;
	int i,j;
	Equation*Eqt;
	for(Eqt = EqtSet->FirstEquation;Eqt!=NULL;Eqt=Eqt->next)
		EquationNum++;
	for (i = 0; i < EquationNum - 1; i++)
	{
		Equation*CurEqt = EqtSet->FirstEquation, *front = NULL;
		for (j = 0; j < EquationNum-1-i; j++)
		{
			//如果当前的方程式未知项项数多于下一项,交换位置;
			if (GetConstOrUnknownNum(CurEqt,TYPE_UNKNOWN)>GetConstOrUnknownNum(CurEqt->next,TYPE_UNKNOWN))
			{
				if (front == NULL)
				{
					EqtSet->FirstEquation = CurEqt->next;
					front = EqtSet->FirstEquation;
					CurEqt->next = CurEqt->next->next;
					front->next = CurEqt;
				}
				else
				{
					front->next = CurEqt->next;
					CurEqt->next = CurEqt->next->next;
					front->next->next = CurEqt;
					front = front->next;
				}
			}
			else
			{
				if (front == NULL)
					front = EqtSet->FirstEquation;
				else
					front = front->next;
				CurEqt = front->next;
			}
		}
	}
}
//从EqtSet最后一组起设置确定量和变化量
BOOL SetDefinite(EquationSet*EqtSetHead)
{
	EquationSet*CurEqtSet;
	for (CurEqtSet = EqtSetHead; CurEqtSet != NULL; CurEqtSet = CurEqtSet->next)
	{
		Equation*eqt;
		SimplyEquationSet(CurEqtSet);
		//按项数多少排序;
		SortEquation(CurEqtSet);
		for (eqt = CurEqtSet->FirstEquation; eqt != NULL; eqt = eqt->next)
		{
			//查找元的值是否确定，将确定的值带入其中;
			Unknown* FindUnknown = NULL;
			Item*pItem;
			int UnknownNum,ConstNum,_VariedSet = 0;;
			for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
			{
				if (pItem->nType == TYPE_UNKNOWN)
				{
					FindUnknown = IsDefinite(pItem->szName);
					//值已经确定;
					if (FindUnknown != NULL)
					{
						//设为常数项;
						pItem->nType = TYPE_CONSTANT;
						memset(pItem->szName, 0, sizeof(pItem->szName));
						//分子相乘,分母相乘;
						pItem->k.Molecule*= FindUnknown->value.Molecule;
						pItem->k.Denominator*=FindUnknown->value.Denominator;
					}
				}
			}
			//化简一次;
			SimplyEquation(eqt);
			UnknownNum = GetConstOrUnknownNum(eqt,TYPE_UNKNOWN);
			ConstNum = GetConstOrUnknownNum(eqt,TYPE_CONSTANT);
			//所有项都消完;
			if (UnknownNum == 0 && ConstNum == 0)
				continue;
			//左边没有未知项，只有常数项且不等于0;
			if (UnknownNum == 0 && ConstNum != 0)
				return FALSE;
			//只有一个未知项,可以确定值;
			if (UnknownNum == 1)
			{
				Item*pUnknownItem,*pConstItem;
				Unknown*pNew;
				if (eqt->FirstItem->nType == TYPE_UNKNOWN)
				{	
					pUnknownItem = eqt->FirstItem;
					pConstItem = pUnknownItem->next;
				}
				else
				{
					pConstItem = eqt->FirstItem;
					pUnknownItem = pConstItem->next;
				}
				//如果是被动变化量,尾插法;
				if (Is_Varied(pUnknownItem->szName))
				{
					Unknown*pTail = Definite;
					while (pTail != NULL && pTail->next != NULL)
						pTail = pTail->next;
					 pNew = (Unknown*)malloc(sizeof(Unknown));
					if (pTail == NULL)
						Definite = pNew;
					else
						pTail->next = pNew;
					memset(pNew,0,sizeof(Unknown));
				}
				else
					//头插法;
					pNew = CreateUnknownInUkList(&Definite);
				pNew->value.Molecule=pNew->value.Denominator=1;
				//复制名称;
				strcpy(pNew->Name, pUnknownItem->szName);
				if (pConstItem == 0)		//元的值为0;
					pNew->value.Molecule=0;
				else                        //元的值不为0;
				{
					pNew->value.Molecule = ABS(pUnknownItem->k.Denominator)*ABS(pConstItem->k.Molecule);
					pNew->value.Denominator=ABS(pUnknownItem->k.Molecule)*ABS(pConstItem->k.Denominator);
					if(pUnknownItem->k.Molecule*pConstItem->k.Molecule>0)			//结果为负;
							pNew->value.Molecule*=-1;
				}
				continue;
			}
			//留一个被动变化值的位置;
			for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
			{
				if (pItem->nType == TYPE_UNKNOWN && !IsVaried(pItem->szName) && !Is_Varied(pItem->szName))
				{
					//还没有设置被动变化量;
					if(_VariedSet == 0)
					{
						//加到被动变化量中;
						Unknown*unk = CreateUnknownInUkList(&_Varied);
						strcpy(unk->Name, pItem->szName);
						_VariedSet = 1;
					}
					else
					{
						//加到主动变化量中;
						Unknown*unk = CreateUnknownInUkList(&Varied);
						unk->value.Denominator = unk->value.Molecule=1;
						strcpy(unk->Name, pItem->szName);
					}
				}
			}
		}
	}
	return TRUE;
}
//创建EquationSet的副本
EquationSet* CopyEquationSet()
{
	EquationSet*RetEquationSet = NULL,*NewCurEqtSet = NULL,*EqtSet;
	for (EqtSet = EquationSetHead; EqtSet != NULL; EqtSet = EqtSet->next)
	{
		Equation*eqt;
		//没有头,尾插法;
		if (NewCurEqtSet == NULL)
			NewCurEqtSet = RetEquationSet =(EquationSet*)malloc(sizeof(EquationSet));
		else
		{
			NewCurEqtSet->next = (EquationSet*)malloc(sizeof(EquationSet));
			NewCurEqtSet = NewCurEqtSet->next;
		}
		memset(NewCurEqtSet,0,sizeof(EquationSet));

		for (eqt = EqtSet->FirstEquation; eqt != NULL; eqt = eqt->next)
		{
			Equation * NewEquation = CreateEquationInEqtSet(NewCurEqtSet);
			Item*pItem;
			for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
			{
				Item*NewItem = CreateItemInEqt(NewEquation);
				memcpy(NewItem, pItem, sizeof(Item)-4);
			}
		}
	}
	return RetEquationSet;
}
//保存结果;
void SaveRet()
{
	Unknown *NewUnknown = NULL,*uk;
	Ret*pRet = (Ret*)malloc(sizeof(Ret));
	pRet->unknown = NULL;
	pRet->next = RetSolution;
	RetSolution = pRet;
	for (uk = Definite; uk != NULL; uk = uk->next)
	{
		if (NewUnknown == NULL)
			NewUnknown = pRet->unknown = (Unknown*)malloc(sizeof(Unknown));
		else
		{
			NewUnknown->next = (Unknown*)malloc(sizeof(Unknown));
			NewUnknown = NewUnknown->next;
		}
		NewUnknown->next = NULL;
		memcpy(NewUnknown, uk, sizeof(Unknown)-4);
	}
}
//当存在主动变化值时用枚举法解出给定个数组解;
int Solve(Unknown * unknown)
{
	//没有主动变化量,都是确定的量;
	if (Varied == NULL)
		return SUCCESS;
	//主动变化量链表尾端,主动变化量已经设置好;
	if (unknown == NULL)
	{
		EquationSet*TempEquationSet;
		//已经解完,退出;
		if (CurSoluton >= MaxSolution)
			return SUCCESS;
		//创建方程组s的副本;
		TempEquationSet = CopyEquationSet();
		if (FALSE == SetDefinite(TempEquationSet))	//无解,退出;
		{
			//删除方程组s的副本;
			DelEquationSet(TempEquationSet);
			return FAIL;
		}
		else                                    //保存该组解;
		{
			CurSoluton++;
			SaveRet();
			//删除方程组s的副本;
			DelEquationSet(TempEquationSet);
			return NEXT;
		}
	}
	else                                                     //设置该主动变化量的值;
	{
		while (1)
		{
			int ret;
			//-----------------------------------------------------------------------------
			//在Definite中添加该项
			Unknown*unk = Definite,*_unk,*front;
			while (unk != NULL && unk->next!=NULL)
				unk = unk->next;
			if (unk == NULL)
				unk = Definite = (Unknown*)malloc(sizeof(Unknown));
			else
			{
				unk->next = (Unknown*)malloc(sizeof(Unknown));
				unk = unk->next;
			}
			memset(unk, 0, sizeof(Unknown));
			memcpy(unk, unknown, sizeof(Unknown)-4);
			
			//-----------------------------------------------------------------------------
			ret = Solve(unknown->next);
			//-----------------------------------------------------------------------------
			//删除Definite中当前元及后面的项;
			for (_unk = Definite,front = NULL; _unk != NULL;)
			{
				Unknown*__unk;
				if (strcmp(_unk->Name, unknown->Name) == 0)
				{
					if (front == NULL)
						Definite = NULL;
					else
						front->next = NULL;
					for (__unk = _unk; __unk != NULL;)
					{
						Unknown*tp = __unk;
						__unk = __unk->next;
						free(tp);
					}
					break;
				}
				else
				{
					front = _unk;
					_unk = front->next;
				}
			}
			//-----------------------------------------------------------------------------
			if (ret == FAIL)
				return FAIL;
			if (ret == SUCCESS)
				return SUCCESS;
			if (ret == NEXT)
			{
				if (unknown->value.Molecule >= MaxValue)
				{
					if (unknown == Varied)
					{
						Unknown*last;
						MaxValue += 3;
						//找到最后一个Varied,值加一;
						last = Varied;
						while (last->next != NULL)
							last = last->next;
						last->value.Molecule++;
					}
					else
						return NEXT;
				}
				else
					unknown->value.Molecule++;
			}
		}
	}
}
//显示结果;
void ShowResult()
{
	Ret*pRet;
	printf("---------------------------------Result---------------------------------\n");
	for(pRet = RetSolution;pRet!=NULL;pRet = pRet->next)
	{
		Unknown*pUk;
		printf("---------\n");
		for(pUk = pRet->unknown;pUk!=NULL;pUk=pUk->next)
		{
			printf("%s=%d",pUk->Name,pUk->value.Molecule);
			if(pUk->value.Denominator!=1)
			{
				printf("/%d",pUk->value.Denominator);
			}
			printf("\n");
		}
	}
	printf("---------------------------------Result---------------------------------\n");
}

int main(int argc,char* argv[])
{
	EquationSet*EqtSet;
	char* szEquation;
	//B==2D;5B==3C+8D;2A==1C;3A==1D;12A+B==3C+4D;
	if(argc !=3)
	{	
		printf("Invalid command!\n");
		return 0;
	}
	szEquation = argv[1];
	StrToNum(argv[2],&MaxSolution);
	if(FALSE == DevideEquationSet(szEquation))
	{
		printf("Devide EquationSet Failed\n");
		goto finish;
	}
	if(FALSE == SetStartEquationSet())
	{
		printf("Set Start EquationSet Failed\n");
		goto finish;
	}
	BeginEliminationUnknown();
	for(EqtSet = EquationSetHead;EqtSet!=NULL;EqtSet = EqtSet->next)
	{
		Equation*Eqt;
		printf("---------------------------EquationSet------------------------\n");
		for(Eqt = EqtSet->FirstEquation;Eqt!=NULL;Eqt=Eqt->next)
		{
			Item*pItem;
			for(pItem = Eqt->FirstItem;pItem!=NULL;pItem = pItem->next)
			{
				if(pItem->k.Molecule>=0)
					printf("+");
				printf("%d",pItem->k.Molecule);
				if(pItem->k.Denominator!=1)
					printf("/,%d",pItem->k.Denominator);
				printf(pItem->szName);	
			}
			printf("====0\n");
		}
	}
	if(FALSE == SetDefinite(EquationSetHead))
	{
		printf("Set Definite Value Failed\n");
		goto finish;
	}
	if(GetDefiniteNum() == UnknownCount)
	{
		printf("Finish!\n");
		SaveRet();
		goto show;
	}
	if(FAIL == Solve(Varied))
	{
		printf("No solution\n");
		goto finish;
	}
show:
	ShowResult();
finish:
	DelEquationStr();
	DelEquationSet(EquationSetHead);
	return 0;
}
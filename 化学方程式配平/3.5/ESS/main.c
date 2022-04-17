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
//��������;
typedef struct _NUM
{
	int Molecule;				//����;
	UINT Denominator;			//��ĸ,��Ϊ��;
}NUM;
typedef struct _Item
{
	int  nType;					//�Ƿ�Ϊδ֪��;
	char szName[32];			//����;
	NUM k;						//ϵ��;
	struct _Item* next;
}Item;
typedef struct _Equation
{
	Item*FirstItem;				//���������;
	struct _Equation* next;
}Equation;
typedef struct _EquationSet
{
	Equation* FirstEquation;	//���淽��ʽ;
	struct _EquationSet* next;
}EquationSet;
//������;
typedef struct _Unknown
{
	char Name[32];
	NUM value;
	struct _Unknown*next;
}Unknown;
//��������ķ���ʽ;
typedef struct _EquationString
{
	char szEquation[256];
	char szEquationLeft[128];
	char szEquationRight[128];
	struct _EquationString* next;
}EquationString;
//��������;
typedef struct _Ret
{
	Unknown*unknown;
	struct _Ret*next;
}Ret;
EquationSet *EquationSetHead = NULL;				//���淽����Ľṹ;
EquationString * EquationStrHead = NULL;			//��������ķ������ַ���;
Unknown *AllUnknown = NULL;							//��������Ԫ������;
int UnknownCount = 0;								//��¼Ԫ�ĸ���;
Unknown *Definite = NULL;							//ȷ����Ԫ;
Unknown *Varied = NULL;								//�����仯��Ԫ;
Unknown *_Varied = NULL;							//�����仯��Ԫ;
Ret *RetSolution = NULL;							//��������׵�ַ;
int MaxSolution = 20;								//�����������;
int CurSoluton = 0;									//�Ѿ�����Ľ������;
int MaxValue = 3;									//�����仯Ԫ���ֵ;
#define IsInAllUnknown(name)	InUnknownList(AllUnknown,name)
#define IsDefinite(name)		InUnknownList(Definite,name)
#define IsVaried(name)			InUnknownList(Varied,name)
#define Is_Varied(name)			InUnknownList(_Varied,name)
//ͳ��ȷ��Ԫ�ĸ���;
int GetDefiniteNum()
{
	int n = 0;
	Unknown*uk;
	for (uk = Definite; uk != NULL; uk = uk->next)
		n++;
	return n;
}
//�Ƿ���unknownlist��;
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
//���ַ�����ʼ���ֵ������ַ�תΪ����,���س���;
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
//������תΪ�ַ���,���س���;
int NumToStr(char *buff, int n)
{
	sprintf(buff, "%d", n);
	return strlen(buff);
}
//�ͷ�Unknownlist;
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
//�ͷ�Item;
#define DelItem(pItem) free(pItem)
//�ͷŷ���;
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
//�ͷŷ�����;
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
//�ͷŷ�����ļ���;
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
//�ͷŴ���ķ����ַ���;
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
//ȡ�ַ����еķ��ӷ�ĸ��Ԫ,�浽Item��;
BOOL GetStrItem(char *str, Item*pItem)
{
	char *p = str;
	int StrLen = strlen(str),Sign;
	int Molecule, Denominator,MoleculeLen, DenominatorLen;
	if (StrLen == 0)					//����Ϊ0��Ч������false;
		return FALSE;

	Sign = 1;
	if (*p == '-')
		Sign = -1;
	p++;
	if (*p < '0' || *p > '9')																			//��һ���ַ�(����������)��������,�������ַ�������Ԫ����;
	{
		pItem->nType = TYPE_UNKNOWN;
		strcpy(pItem->szName, p);
		pItem->k.Molecule = 1, pItem->k.Denominator = 1;												//���ӷ�ĸ��Ϊ1
		goto success;
	}
	//----------------------------------------------------------------------
	//�������;
	MoleculeLen = StrToNum(p, &Molecule);
	if (*(p + MoleculeLen) == NULL)																		//ֻ��һ�����ǳ�����;
	{
		pItem->nType = TYPE_CONSTANT;
		pItem->k.Molecule = Molecule, pItem->k.Denominator = 1;
		goto success;
	}
	if (*(p + MoleculeLen) == '/' && (*(p + MoleculeLen + 1) >= '0' && *(p + MoleculeLen + 1) <= '9'))	//�ҵ��������ҷ����ߺ���һ����;
	{
		DenominatorLen = StrToNum(p + MoleculeLen + 1, &Denominator);
		if ((DenominatorLen + MoleculeLen + 2/*�����ߺ͵�һ��������*/) == StrLen)							//ֻ�з���;
		{
			pItem->nType = TYPE_CONSTANT;
		}
		else																							//�Ƿ�����һ��Ԫ;
		{
			pItem->nType = TYPE_UNKNOWN;
			strcpy(pItem->szName, p + MoleculeLen + DenominatorLen + 1);
		}
		pItem->k.Molecule = Molecule, pItem->k.Denominator = Denominator;
		if (pItem->k.Denominator == 0)		//��ĸΪ0����Ч
			return FALSE;
		goto success;
	}
	else																								//һ��������Ԫ
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
//ȡ����ֵ;
UINT ABS(int n)
{
	return n > 0 ? n : -n;
}
//ȡ�������;
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
//ȡ��С������;
UINT GetMinCommonMultiple(int a, int b)
{
	a = ABS(a), b = ABS(b);
	return a*b / GetMaxCommonFactor(a, b);
}
//��һ��unknownlist�д���һ��;
Unknown*CreateUnknownInUkList(Unknown**UkListHead)
{
	Unknown*pNew = (Unknown*)malloc(sizeof(Unknown));
	memset(pNew, 0, sizeof(Unknown));
	pNew->next = (*UkListHead);
	(*UkListHead) = pNew;
	return pNew;
}
//��һ�������������д���һ��������;
EquationSet * CreateEquationSetInEqtSetList(EquationSet**EqtSetListHead)
{
	//ͷ�巨
	EquationSet* pEqtSet = (EquationSet*)malloc(sizeof(EquationSet));
	pEqtSet->next = (*EqtSetListHead);
	(*EqtSetListHead) = pEqtSet;
	(*EqtSetListHead)->FirstEquation = NULL;
	return pEqtSet;
}
//��һ���������д���һ������;
Equation *CreateEquationInEqtSet(EquationSet*EqtSet)
{
	//ͷ�巨
	Equation *pEqt = (Equation*)malloc(sizeof(Equation));
	pEqt->FirstItem = NULL;
	pEqt->next = EqtSet->FirstEquation;
	EqtSet->FirstEquation = pEqt;
	return pEqt;
}
//��һ�������鴴��һ����;
Item* CreateItemInEqt(Equation*Eqt)
{
	//ͷ�巨
	Item *pItem = (Item*)malloc(sizeof(Item));
	memset(pItem, 0, sizeof(Item));
	pItem->next = Eqt->FirstItem;
	Eqt->FirstItem = pItem;
	return pItem;
}
//��';'�ָ����;
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
			//���������̴浽->szEquation��;
			memset(EquationStrHead->szEquation, 0, sizeof(EquationStrHead->szEquation));
			memcpy(EquationStrHead->szEquation, pStart, pEnd - pStart);
			memset(EquationStrHead->szEquationLeft, 0, sizeof(EquationStrHead->szEquationLeft));
			memset(EquationStrHead->szEquationRight, 0, sizeof(EquationStrHead->szEquationRight));
			if (EquationStrHead->szEquationLeft[0] != '+' && EquationStrHead->szEquationLeft[0] != '-')
				EquationStrHead->szEquationLeft[0] = '+';
			if (EquationStrHead->szEquationRight[0] != '+' && EquationStrHead->szEquationRight[0] != '-')
			EquationStrHead->szEquationRight[0] = '+';
			//----------------------------------------------------------------------------------
			//�ָ������������;

			//�ҵ����ں�;
			DevideSign = EquationStrHead->szEquation;
			//�Ⱥ����û�ж���;
			if (*DevideSign == '=') return FALSE;
			while (*DevideSign != '='&&*DevideSign != NULL) DevideSign++;
			//û�е��ں�;
			if (*DevideSign == NULL) return FALSE;
			//�������;
			memcpy(EquationStrHead->szEquationLeft+1, EquationStrHead->szEquation, DevideSign - EquationStrHead->szEquation);
			while (*DevideSign == '=' && *DevideSign != NULL) DevideSign++;
			// �Ⱥ��ұ�û�ж���;
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
//������ʼ������;
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
			//��һ�ηָ��󲿷֣��ڶ��ηָ��Ҳ���;
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
						return FALSE;						//��Ч����;
					//��һ�γ��ֵ�Ԫ,��allunknownlist�����;
					if (pItem->nType == TYPE_UNKNOWN && !IsInAllUnknown(pItem->szName))
					{
						Unknown*pUk = CreateUnknownInUkList(&AllUnknown);
						UnknownCount++;
						strcpy(pUk->Name, pItem->szName);
					}
					//�ָ��ұߵ���ÿ��ϵ����-1;
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
//����ÿһ��(Լ��);
void SimplyItem(Item*pItem)
{
	int MaxCommonFactor = GetMaxCommonFactor(pItem->k.Molecule, pItem->k.Denominator);
	pItem->k.Molecule /= MaxCommonFactor;
	pItem->k.Denominator /= MaxCommonFactor;
}
//�Ƿ�Ϊͬ����;
BOOL IsSameClassItem(Item*pItem1, Item*pItem2)
{
	return ((pItem1->nType == pItem2->nType) && !strcmp(pItem1->szName, pItem2->szName));
}
//�ϲ�����浽��һ����;
void MergeItem(Item*pItem1, Item*pItem2)
{
	//�����ĸ����С������;
	int MinCommonMutiple = GetMinCommonMultiple(pItem1->k.Denominator, pItem2->k.Denominator);
	//ͨ��;
	pItem1->k.Molecule *= MinCommonMutiple / pItem1->k.Denominator;
	pItem2->k.Molecule *= MinCommonMutiple / pItem2->k.Denominator;
	pItem1->k.Denominator = pItem2->k.Denominator = MinCommonMutiple;
	//�������;
	pItem1->k.Molecule += pItem2->k.Molecule;
}
//���򷽳�ʽ;
void SimplyEquation(Equation*eqt)
{
	int MaxCommonFactor = 0;
	//-----------------------------------�ϲ�ͬ����----------------------------;
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
	//-------------------------------------���ϵ��Ϊ0����---------------------------;
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
	//ÿһ��Լ��;
	for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		SimplyItem(pItem);
	}
	//��ȡ��������������;
	for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		if (MaxCommonFactor == 0)
			MaxCommonFactor = pItem->k.Molecule;
		else
			MaxCommonFactor = GetMaxCommonFactor(MaxCommonFactor, pItem->k.Molecule);
	}
	//ÿһ��ͬ�����������;
	if (MaxCommonFactor != 1)
	{
		for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
		{
			pItem->k.Molecule /= MaxCommonFactor;
		}
	}
}
//��ȡ�������Ԫ����;
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
//�������Ƿ����ĳһ��Ԫ;
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
////��ȡ��������ִ������ٵ�Ԫ,���浽Name��;
void GetLeastUnknown(EquationSet*EqtSet, char*Name,int size)
{
	Unknown *UnknInfo = NULL,*LeastUnknown = NULL,*pFindUnk = NULL,*pNewUnkn = NULL,*pUnknInfo = NULL;
	Equation*eqt;
	Item*pItem;
	//�����е�Ԫ���浽�����в���¼���ֵĴ���;
	for (eqt = EqtSet->FirstEquation; eqt != NULL; eqt = eqt->next)
	{
		for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
		{
			if (pItem->nType == TYPE_UNKNOWN)
			{
				//�����Ƿ����;
				pFindUnk = InUnknownList(UnknInfo,pItem->szName);
				//�����ڣ��ӵ�������;
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
		//����ǵ�һ���ҳ��ִ�������1;
		if (LeastUnknown == NULL && pUnknInfo->value.Molecule > 1)
				LeastUnknown = pUnknInfo;
		//�������С��leastUnknown������LeastUnknown==pUnknInfo;
		else if (pUnknInfo->value.Molecule > 1 && pUnknInfo->value.Molecule<LeastUnknown->value.Molecule)
				LeastUnknown = pUnknInfo;
	}
	memset(Name, 0, size);
	if (LeastUnknown != NULL)//ÿ��Ԫֻ������һ��Ϊ��;
		strcpy(Name, LeastUnknown->Name);
	//�ͷ��ڴ�;
	DelUnknownList(UnknInfo);
}
//����ÿ�����̲�ɾ���������еĿշ���;
void SimplyEquationSet(EquationSet*EqtSet)
{
	Equation*Eqt = NULL,*front = NULL;
	//ɾ���շ���;
	for(Eqt = EqtSet->FirstEquation,front = NULL;Eqt!=NULL;)
	{
		SimplyEquation(Eqt);
		//�շ���ɾ��;
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
//����������Ԫ�������µķ���;
Equation* ClearEquationUnknown(Equation*eqt1, Equation*eqt2,Item*pItem1,Item*pItem2)
{
	Item*pItem,*NewItem,*FindItem;
	Equation*NewEquation;
	int DenoMinCommonMultiple,MoleMinCommonMultiple,Eqt1Multiply,Eqt2Multiply;
	//��¼����������ӻ������;
	BOOL bChangeSign;
	bChangeSign = (pItem1->k.Molecule*pItem2->k.Molecule)>=0;	//ͬ��;
	//ȡ��ĸ��С������;
	DenoMinCommonMultiple = GetMinCommonMultiple(pItem1->k.Denominator, pItem2->k.Denominator);
	pItem1->k.Molecule*= DenoMinCommonMultiple / pItem1->k.Denominator;
	pItem2->k.Molecule *= DenoMinCommonMultiple / pItem2->k.Denominator;
	pItem1->k.Denominator= pItem2->k.Denominator = DenoMinCommonMultiple;
	//ȡ������С������;
	MoleMinCommonMultiple = GetMinCommonMultiple(pItem1->k.Molecule, pItem2->k.Molecule);
	//�����һ������ÿһ��Ҫ�˵���;
	Eqt1Multiply = MoleMinCommonMultiple / ABS(pItem1->k.Molecule);
	//�����һ������ÿһ��Ҫ�˵���;
	Eqt2Multiply = MoleMinCommonMultiple / ABS(pItem2->k.Molecule);
	//����һ�����̵�ÿһ�����Eqt1Multiply;
	for (pItem = eqt1->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		pItem->k.Molecule*=Eqt1Multiply;
	}
	//����һ�����̵�ÿһ�����Eqt1Multiply;
	for (pItem = eqt2->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		pItem->k.Molecule*=Eqt2Multiply;
		//���ͬ��,������2���еķ���ȡ��;
		if (bChangeSign)
		{
			pItem->k.Molecule*=-1;
		}
	}
	//�������ɵ��·���;
	NewEquation = (Equation*)malloc(sizeof(Equation));
	memset(NewEquation,0,sizeof(Equation));
	//�Ȱ�eqt1���Ƶ�NewEquation��;
	for (pItem = eqt1->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		NewItem = CreateItemInEqt(NewEquation);
		memcpy(NewItem, pItem, sizeof(Item)-4/*��ȥ�ĸ��ֽڣ�nextָ�룩*/);
	}
	//����eqt2������,��NewEquation�в����Ƿ����,���������,���ںϲ�;
	for (pItem = eqt2->FirstItem; pItem != NULL; pItem = pItem->next)
	{
		FindItem = EqtExistItem(NewEquation, pItem->nType, pItem->szName);
		if (FindItem == NULL)		//������;
		{
			//��NewEquation����Ӹ���
			Item*NewItem = CreateItemInEqt(NewEquation);
			memcpy(NewItem, pItem, sizeof(Item)-4/*��ȥ�ĸ��ֽڣ�nextָ�룩*/);
		}
		else						//���ڸ���;
			MergeItem(FindItem, pItem);
	}
	SimplyEquation(eqt1);
	SimplyEquation(eqt2);
	SimplyEquation(NewEquation);
	return NewEquation;
}
//�����鿪ʼ��Ԫ;
void BeginEliminationUnknown()
{
	char LeastUnknown[32] = { 0 };
	while(1)
	{
		Equation*eqt1,*eqt2,*front;
		EquationSet*EqtSet = EquationSetHead;
		int EqtSetExist = 0;
		SimplyEquationSet(EqtSet);
		//�ҳ����ִ������ٵ�Ԫ;
		GetLeastUnknown(EqtSet, LeastUnknown, sizeof(LeastUnknown));
		//�����ڳ��ִ������ٵ�Ԫ;
		if (*LeastUnknown == '\0')
			break;
		for (eqt1 = EqtSet->FirstEquation, front = NULL; eqt1 != NULL;)
		{
			Item*FindItem1 = EqtExistItem(eqt1, TYPE_UNKNOWN, LeastUnknown);
			Item*FindItem2;
			//���ֻ����һ����һ��������;
			//������������ٴ���Ԫ,��������̷ŵ���һ��������;
			if (FindItem1 == NULL)
			{
				if(EqtSetExist==0)
				{
					CreateEquationSetInEqtSetList(&EquationSetHead);
					EqtSetExist=1;
				}
				//�ƶ��������,��һ����������EquationSetHead;
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
			else						//���������̴��ڳ������ٴ���Ԫ�������һ�����̿�ʼѰ��;
			{
				for (eqt2 = eqt1->next; eqt2 != NULL; eqt2 = eqt2->next)
				{
					FindItem2 = EqtExistItem(eqt2, TYPE_UNKNOWN, LeastUnknown);
					if (FindItem2 != NULL)
					{
						Equation*NewEquation = ClearEquationUnknown(eqt1,eqt2,FindItem1,FindItem2);
						//��������һ�����������;
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
//���������еķ��̰�δ֪���������;
void SortEquation(EquationSet*EqtSet)
{
	//��¼���̸���;
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
			//�����ǰ�ķ���ʽδ֪������������һ��,����λ��;
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
//��EqtSet���һ��������ȷ�����ͱ仯��
BOOL SetDefinite(EquationSet*EqtSetHead)
{
	EquationSet*CurEqtSet;
	for (CurEqtSet = EqtSetHead; CurEqtSet != NULL; CurEqtSet = CurEqtSet->next)
	{
		Equation*eqt;
		SimplyEquationSet(CurEqtSet);
		//��������������;
		SortEquation(CurEqtSet);
		for (eqt = CurEqtSet->FirstEquation; eqt != NULL; eqt = eqt->next)
		{
			//����Ԫ��ֵ�Ƿ�ȷ������ȷ����ֵ��������;
			Unknown* FindUnknown = NULL;
			Item*pItem;
			int UnknownNum,ConstNum,_VariedSet = 0;;
			for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
			{
				if (pItem->nType == TYPE_UNKNOWN)
				{
					FindUnknown = IsDefinite(pItem->szName);
					//ֵ�Ѿ�ȷ��;
					if (FindUnknown != NULL)
					{
						//��Ϊ������;
						pItem->nType = TYPE_CONSTANT;
						memset(pItem->szName, 0, sizeof(pItem->szName));
						//�������,��ĸ���;
						pItem->k.Molecule*= FindUnknown->value.Molecule;
						pItem->k.Denominator*=FindUnknown->value.Denominator;
					}
				}
			}
			//����һ��;
			SimplyEquation(eqt);
			UnknownNum = GetConstOrUnknownNum(eqt,TYPE_UNKNOWN);
			ConstNum = GetConstOrUnknownNum(eqt,TYPE_CONSTANT);
			//���������;
			if (UnknownNum == 0 && ConstNum == 0)
				continue;
			//���û��δ֪�ֻ�г������Ҳ�����0;
			if (UnknownNum == 0 && ConstNum != 0)
				return FALSE;
			//ֻ��һ��δ֪��,����ȷ��ֵ;
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
				//����Ǳ����仯��,β�巨;
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
					//ͷ�巨;
					pNew = CreateUnknownInUkList(&Definite);
				pNew->value.Molecule=pNew->value.Denominator=1;
				//��������;
				strcpy(pNew->Name, pUnknownItem->szName);
				if (pConstItem == 0)		//Ԫ��ֵΪ0;
					pNew->value.Molecule=0;
				else                        //Ԫ��ֵ��Ϊ0;
				{
					pNew->value.Molecule = ABS(pUnknownItem->k.Denominator)*ABS(pConstItem->k.Molecule);
					pNew->value.Denominator=ABS(pUnknownItem->k.Molecule)*ABS(pConstItem->k.Denominator);
					if(pUnknownItem->k.Molecule*pConstItem->k.Molecule>0)			//���Ϊ��;
							pNew->value.Molecule*=-1;
				}
				continue;
			}
			//��һ�������仯ֵ��λ��;
			for (pItem = eqt->FirstItem; pItem != NULL; pItem = pItem->next)
			{
				if (pItem->nType == TYPE_UNKNOWN && !IsVaried(pItem->szName) && !Is_Varied(pItem->szName))
				{
					//��û�����ñ����仯��;
					if(_VariedSet == 0)
					{
						//�ӵ������仯����;
						Unknown*unk = CreateUnknownInUkList(&_Varied);
						strcpy(unk->Name, pItem->szName);
						_VariedSet = 1;
					}
					else
					{
						//�ӵ������仯����;
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
//����EquationSet�ĸ���
EquationSet* CopyEquationSet()
{
	EquationSet*RetEquationSet = NULL,*NewCurEqtSet = NULL,*EqtSet;
	for (EqtSet = EquationSetHead; EqtSet != NULL; EqtSet = EqtSet->next)
	{
		Equation*eqt;
		//û��ͷ,β�巨;
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
//������;
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
//�����������仯ֵʱ��ö�ٷ���������������;
int Solve(Unknown * unknown)
{
	//û�������仯��,����ȷ������;
	if (Varied == NULL)
		return SUCCESS;
	//�����仯������β��,�����仯���Ѿ����ú�;
	if (unknown == NULL)
	{
		EquationSet*TempEquationSet;
		//�Ѿ�����,�˳�;
		if (CurSoluton >= MaxSolution)
			return SUCCESS;
		//����������s�ĸ���;
		TempEquationSet = CopyEquationSet();
		if (FALSE == SetDefinite(TempEquationSet))	//�޽�,�˳�;
		{
			//ɾ��������s�ĸ���;
			DelEquationSet(TempEquationSet);
			return FAIL;
		}
		else                                    //��������;
		{
			CurSoluton++;
			SaveRet();
			//ɾ��������s�ĸ���;
			DelEquationSet(TempEquationSet);
			return NEXT;
		}
	}
	else                                                     //���ø������仯����ֵ;
	{
		while (1)
		{
			int ret;
			//-----------------------------------------------------------------------------
			//��Definite����Ӹ���
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
			//ɾ��Definite�е�ǰԪ���������;
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
						//�ҵ����һ��Varied,ֵ��һ;
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
//��ʾ���;
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
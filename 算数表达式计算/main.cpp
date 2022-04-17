
/*
	Project:	�������ʽ����
	����ԭ��:�����ڲ�����Ϊһ����Ԫ���ڸ�������ֻ�и�����������ߺ������������ȼ����㼴��
	����ǰ����������ȼ�С�ڵ���ǰһ������������ȼ������Լ���
	Date:		2021/06/20
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include <stack>
#include <string>
#include <string.h>
typedef unsigned int BOOL;
#define FALSE	0
#define TRUE	1

using namespace std;

typedef vector<double> NUMSET;

struct Operator;

typedef BOOL(*pProcessFunc)(NUMSET&in, NUMSET&out, Operator*OperatorInfo);
//===================================================
struct Operator
{
	BOOL			IsFunc;					//�Ƿ�Ϊ����
	const char		OperatorName[16];		//���������
	unsigned char	Priority;				//���ȼ�
	int				ReqiureArgc;			//�����������,-1Ϊ�ɱ����
	pProcessFunc 	pProcessFunc;			//������
};

//===================================================


//�ӷ����㶨��
BOOL Add(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	out.push_back(in[1] + in[0]);
	return TRUE;
}

//�������㶨��
BOOL Sub(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	out.push_back(in[1] - in[0]);
	return TRUE;
}

//�˷����㶨��
BOOL Mul(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	out.push_back(in[1] * in[0]);
	return TRUE;
}

//�������㶨��
BOOL Div(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	if (in[0] == 0)							
	{
		printf("Error:The divisor is zero.\n");
		return FALSE;
	}
	out.push_back(in[1] / in[0]);
	return TRUE;
}

//pow���㶨��
BOOL Pow(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	if (in[0] == 0 && in[1] == 0)
	{
		printf("Error:Pow(0,0) is invalid!\n");
		return FALSE;
	}
	
	out.push_back(pow(in[0],in[1]));
	return TRUE;
}

//abs���㶨��
BOOL Abs(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	out.push_back(abs(in[0]));
	return TRUE;
}

//max���㶨��
BOOL Max(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc!=-1&&in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	double maxvalue = 0;

	for (auto it = in.begin(); it != in.end(); it++)
	{
		if (it == in.begin())
		{
			maxvalue = *it;
		}
		else
		{
			if (*it>maxvalue)
				maxvalue = *it;
		}
	}
	out.push_back(maxvalue);
	return TRUE;
}

//Min���㶨��
BOOL Min(NUMSET&in, NUMSET&out, Operator*OperatorInfo)
{
	if (OperatorInfo->ReqiureArgc != -1 && in.size() != OperatorInfo->ReqiureArgc)
	{
		printf("Error:%s 's argc is invalid!\n", OperatorInfo->OperatorName);
		return FALSE;
	}
	double minvalue = 0;

	for (auto it = in.begin(); it != in.end(); it++)
	{
		if (it == in.begin())
		{
			minvalue = *it;
		}
		else
		{
			if (*it<minvalue)
				minvalue = *it;
		}
	}
	out.push_back(minvalue);
	return TRUE;
}

Operator operators[] =
{
	{ 0, "+", 0, 2, Add },
	{ 0, "-", 0, 2, Sub },
	{ 0, "*", 1, 2, Mul },
	{ 0, "/", 1, 2, Div },
	{ 1, "pow", 0, 2, Pow },
	{ 1, "abs", 0, 1, Abs },
	{ 1, "max", 0, -1, Max },
	{ 1, "min", 0, -1, Min },

	{ 0, "NULL", 0, 0, 0 }
};

Operator* GetOperatorInfo(const char*Name,BOOL IsFunc)	//��ȡ�������Ϣ
{
	for (int i = 0; strcmp(operators[i].OperatorName, "NULL"); i++)
	{
		if (!strcmp(operators[i].OperatorName, Name) && operators[i].IsFunc == IsFunc)
			return &operators[i];
	}
	return NULL;
}




//���ַ�����ʼ����ȡһ������
BOOL ReadNum(const char*pExpression,double&num,int&NumStrLen)
{
	const char*pSign = pExpression;
	BOOL HasSign = FALSE;
	NumStrLen = 0;
	//�ӵ�һ���ַ��жϲ�������
	if ((*pExpression < '0' || *pExpression > '9') && (*pExpression != '-') && (*pExpression != '+'))
		return FALSE;

	if ((*pExpression == '-') || (*pExpression == '+'))
	{
		HasSign = TRUE;
		++NumStrLen;
		++pSign;
	}
	int PointCount = 0;
	while ((*pSign >= '0' && *pSign <= '9') || (*pSign=='.'))
	{
		if (*pSign == '.')
			++PointCount;
		if (PointCount > 1)
			return FALSE;
		++pSign;
		++NumStrLen;
	}

	sscanf(pExpression, "%lf", &num);

	return NumStrLen - HasSign;
}

//��ȡ������
BOOL ReadFunc(const char*pExpression, Operator* &pOperator,int&FuncNameLen)
{
	const char*pSign = pExpression;
	while (*pSign != '0')
	{
		if (*pSign == '(')
		{
			FuncNameLen = pSign - pExpression;
			char*temp = (char*)malloc(FuncNameLen + 1);
			temp[FuncNameLen] = 0;

			memcpy(temp, pExpression, FuncNameLen);

			pOperator = GetOperatorInfo(temp,TRUE);
			
			free(temp);
			return (BOOL)pOperator;
		}
		pSign++;
	}
	return FALSE;
}



//��ȡ���������
BOOL ReadOperator(const char*pExpression, Operator* &pOperator, int&OperatorNameLen)
{
	const char*pSign = pExpression;
	for (int i = 0; strcmp(operators[i].OperatorName, "NULL"); i++)
	{
		if (!memcmp(operators[i].OperatorName, pSign, strlen(operators[i].OperatorName)) && operators[i].IsFunc == FALSE)
		{
			pOperator = &operators[i];
			OperatorNameLen = strlen(operators[i].OperatorName);
			return TRUE;
		}
	}
	return FALSE;
}

//����ֻʣ����������������ڱ��ʽ��ֵ
BOOL Calculate(stack<double>&Nums, stack<string>&Operators)
{
	NUMSET in,out;

	in.push_back(Nums.top());
	Nums.pop();

	in.push_back(Nums.top());
	Nums.pop();


	Operator*pOperator = GetOperatorInfo(Operators.top().c_str(), 0);
	Operators.pop();

	if (!pOperator)
		return FALSE;
	
	pOperator->pProcessFunc(in, out, pOperator);

	Nums.push(out.back());
	return TRUE;
}
//������/����/���ţ��� �����������ȡ
const char*CalculateExpression(const char*pExpression,BOOL IsFunc,NUMSET&ret)
{

	stack<double>Nums;					//����ջ
	stack<string>Operators;				//�����ջ

	const char*pSign = pExpression + 1;	//pExpessionָ�����'('

	
	int NumStrLen, FuncNameLen,OperatorNameLen;
	double TempNum;
	Operator* pOperator;
	
	int n = 1;
	while (TRUE)
	{
		if (*pSign == ')' || *pSign == ',')			//ջ�е����ְ��մ��������˳����㼴��
		{
			NUMSET _ret;
			//��ջ����
			while (Operators.size())
			{
				Calculate(Nums, Operators);
			}	
			ret.push_back(Nums.top());				//����������ÿһ���ֵĽ��

			if (*pSign == ')')						//���Ž���������
				return pSign + 1;
			if (*pSign == ',')						//���ֽ�����������һ����
			{
				++pSign;
				n = 1;
			}
		}
		else
		{
			if (n & 1)								//�����ڶ�ȡ�ĵ�һ��һ�������֣�����ǰ�������+,-��
			{
				//���������ź����֣����ն���һ����
				//�����ֻ��ߺ���������
				if (ReadNum(pSign, TempNum, NumStrLen))
				{
					pSign += NumStrLen;
					Nums.push(TempNum);
				}
				else if (ReadFunc(pSign, pOperator, FuncNameLen))
				{
					
					NUMSET func_arg_ret, _ret;
					pSign += FuncNameLen;

					//����ÿһ������
					if (FALSE == (pSign = CalculateExpression(pSign, TRUE, func_arg_ret)))
					{
						printf("Error:function %s 's argvs exist fault!\n", pOperator->OperatorName);
						return NULL;
					}
					
					pOperator->pProcessFunc(func_arg_ret, _ret, pOperator);
					//������ջ
					Nums.push(_ret.front());			//_ret.front()Ϊ��������Ľ��

				}
				else if (*pSign == '(')
				{
					NUMSET _ret;
					pSign = CalculateExpression(pSign, FALSE, _ret);
					if (pSign == FALSE)
					{
						return NULL;
					}
					//������ջ
					Nums.push(_ret.front());
				}
				else
				{
					printf("Error:Invalid expression!\n");
					return NULL;
				}
			}
			else
			{
				if (ReadOperator(pSign, pOperator, OperatorNameLen))
				{
					while ((Operators.size() > 0) &&
						(GetOperatorInfo(Operators.top().c_str(), FALSE)->Priority >= pOperator->Priority))
					{
						//�����ǰ����������ȼ�С�ڵ�����һ������ô��һ���Ϳ��Լ���
						Calculate(Nums, Operators);
					}
					Operators.push(pOperator->OperatorName);
					pSign += OperatorNameLen;
				}
				else
				{
					printf("Error:Invalid expression!\n");
					return NULL;
				}
			}
			n++;
		}
	}
}

int main()
{
	printf("Please input expression:");
	NUMSET ret;
	char Expression[256] = { '(' };
	gets(Expression+1);

	strcat(Expression, ")");

	if (CalculateExpression(Expression, FALSE, ret))
	{
		printf("result:%lf\n", ret.front());
	}
	return 0;
}


/*
	Project:	算数表达式计算
	计算原理:以最内层括号为一个单元，在该括号内只有各种运算符或者函数，按照优先级计算即可
	若当前运算符的优先级小于等于前一个运算符的优先级，可以计算
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
	BOOL			IsFunc;					//是否为函数
	const char		OperatorName[16];		//运算符类型
	unsigned char	Priority;				//优先级
	int				ReqiureArgc;			//所需参数个数,-1为可变参数
	pProcessFunc 	pProcessFunc;			//处理函数
};

//===================================================


//加法运算定义
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

//减法运算定义
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

//乘法运算定义
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

//除法运算定义
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

//pow运算定义
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

//abs运算定义
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

//max运算定义
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

//Min运算定义
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

Operator* GetOperatorInfo(const char*Name,BOOL IsFunc)	//获取运算符信息
{
	for (int i = 0; strcmp(operators[i].OperatorName, "NULL"); i++)
	{
		if (!strcmp(operators[i].OperatorName, Name) && operators[i].IsFunc == IsFunc)
			return &operators[i];
	}
	return NULL;
}




//从字符串起始处读取一个数字
BOOL ReadNum(const char*pExpression,double&num,int&NumStrLen)
{
	const char*pSign = pExpression;
	BOOL HasSign = FALSE;
	NumStrLen = 0;
	//从第一个字符判断不是数字
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

//读取函数名
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



//读取运算符名称
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

//计算只剩下四则运算的括号内表达式的值
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
//（数字/函数/括号）和 运算符轮流读取
const char*CalculateExpression(const char*pExpression,BOOL IsFunc,NUMSET&ret)
{

	stack<double>Nums;					//数字栈
	stack<string>Operators;				//运算符栈

	const char*pSign = pExpression + 1;	//pExpession指向的是'('

	
	int NumStrLen, FuncNameLen,OperatorNameLen;
	double TempNum;
	Operator* pOperator;
	
	int n = 1;
	while (TRUE)
	{
		if (*pSign == ')' || *pSign == ',')			//栈中的数字按照从右向左的顺序计算即可
		{
			NUMSET _ret;
			//出栈运算
			while (Operators.size())
			{
				Calculate(Nums, Operators);
			}	
			ret.push_back(Nums.top());				//保存括号中每一部分的结果

			if (*pSign == ')')						//括号结束，返回
				return pSign + 1;
			if (*pSign == ',')						//部分结束，继续下一部分
			{
				++pSign;
				n = 1;
			}
		}
		else
		{
			if (n & 1)								//括号内读取的第一个一定是数字（数字前面可以有+,-）
			{
				//函数，括号和数字，最终都是一个数
				//读数字或者函数或括号
				if (ReadNum(pSign, TempNum, NumStrLen))
				{
					pSign += NumStrLen;
					Nums.push(TempNum);
				}
				else if (ReadFunc(pSign, pOperator, FuncNameLen))
				{
					
					NUMSET func_arg_ret, _ret;
					pSign += FuncNameLen;

					//计算每一个参数
					if (FALSE == (pSign = CalculateExpression(pSign, TRUE, func_arg_ret)))
					{
						printf("Error:function %s 's argvs exist fault!\n", pOperator->OperatorName);
						return NULL;
					}
					
					pOperator->pProcessFunc(func_arg_ret, _ret, pOperator);
					//数字入栈
					Nums.push(_ret.front());			//_ret.front()为函数计算的结果

				}
				else if (*pSign == '(')
				{
					NUMSET _ret;
					pSign = CalculateExpression(pSign, FALSE, _ret);
					if (pSign == FALSE)
					{
						return NULL;
					}
					//数字入栈
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
						//如果当前的运算符优先级小于等于上一个，那么上一个就可以计算
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

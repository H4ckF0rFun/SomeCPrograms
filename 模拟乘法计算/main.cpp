#include <IOSTREAM>
#include <VECTOR>
#include <STRING>

using namespace std;

typedef vector<int> NUM;


void StrToNum(const char *num,NUM&Ret)
{
	for(int i = strlen(num) - 1;i>=0;i--)
	{
		Ret.push_back(num[i]-'0');
	}
}

void GetHILO(int n,int& high,int& low)
{
	char str[32] = {0};
	sprintf(str,"%d",n);

	
	low = *(str + strlen(str) - 1) - '0';
	if(strlen(str) == 2)
		high = str[0] - '0';
}

void Add(NUM &num1,NUM &num2,NUM &ret)
{
	int add = 0;
	for(int i= 0;i<=(num1.size()-1) || i<=(num2.size()-1);i++)
	{
		int n = 0;

		if(i <= (num1.size()-1))
			n+=num1[i];
		if(i <= (num2.size()-1))
			n+=num2[i];
		n+=add;
		
		int HI = 0,LO = 0;
		GetHILO(n,HI,LO);
		add = HI;
		ret.push_back(LO);
	}
}
int main()
{
	system("mode con cols=130 lines=40");

	string s_num1;
	string s_num2;

	cout<<"请输入两个整数:"<<endl;

	cin>>s_num1>>s_num2;

	NUM num1,num2;
	StrToNum(s_num1.c_str(),num1);
	StrToNum(s_num2.c_str(),num2);

	vector<NUM>ProcessNum;
	//相乘
	//65425 45325
	for(int j = 0;j<num2.size();j++)
	{
		NUM tp;
		//添加0
		for(int a = 0;a<j;a++)
			tp.push_back(0);
		ProcessNum.push_back(tp);
		int add = 0;
		for(int i = 0;i<num1.size();i++)
		{
			int n = num2[j] * num1[i];
			n+=add;
			int HI = 0,LO = 0;
			GetHILO(n,HI,LO);
			add = HI;
			ProcessNum[j].push_back(LO);
			if(i == num1.size()-1 && add!=0)
				ProcessNum[j].push_back(add);
		}
	}

	
	cout<<"计算过程:"<<endl<<endl;
	for(int i = 0 ;i<ProcessNum.size();i++)
	{
		for(int j = 0;j<ProcessNum[i].size();j++)
		{
			cout<<ProcessNum[i][j];
		}
		cout<<endl;
	}
	//相加
	NUM Ret;
	Ret.push_back(0);
	for(int a = 0;a<ProcessNum.size();a++)
	{
		NUM temp;
		Add(Ret,ProcessNum[a],temp);
		Ret = temp;
	}

	cout<<"结果:"<<endl;
	for(int b = Ret.size() - 1; b>=0;b--)
	{
		cout<<Ret[b];
	}
	cout<<endl;
	return 0;
}

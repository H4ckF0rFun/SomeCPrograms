#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

void LogOn();

int main()
{

	LogOn();

	return 0;
}

void LogOn()
{
	while (1)
	{
		char ch;
		int n;		//记录密码位数
		char UserName[20] = { 0 };
		char Password[20] = { 0 };
		system("cls");
		printf("请输入用户名:");
		scanf("%s", UserName);
		printf("请输入密码(最多19位):");

		n = 0;//n始终记录位数
		while ((ch = getch()) != '\r'/*回车键*/)
		{

			if (ch == '\b')		//按下的是退格键Backspace
			{
				//没有输入密码就按下backspace，不处理
				if (n > 0)
				{
					printf("%c%c%c", '\b'/*光标向左移动*/, '\0'/*用空白符替换删除的位,光标会向右移一位*/, '\b'/*g光标再向左移动,输入新的一位替换空白*/);
					n--;
				}
			}
			else
			{
				if (n < 19)
				{
					printf("*");
					Password[n++] = ch;
				}

			}
		}
		//将第n位,密码最后一位的下一位设置为结尾符号0
		Password[n] = 0;

		if (!strcmp("zhangsan", UserName) && !strcmp("123456", Password))
		{
			printf("\n登录成功!\n");
			system("pause");
			break;
		}
		else
		{
			printf("\n登录失败!请重新登录\n");
			system("pause");
		}
	}
}
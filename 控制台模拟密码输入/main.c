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
		int n;		//��¼����λ��
		char UserName[20] = { 0 };
		char Password[20] = { 0 };
		system("cls");
		printf("�������û���:");
		scanf("%s", UserName);
		printf("����������(���19λ):");

		n = 0;//nʼ�ռ�¼λ��
		while ((ch = getch()) != '\r'/*�س���*/)
		{

			if (ch == '\b')		//���µ����˸��Backspace
			{
				//û����������Ͱ���backspace��������
				if (n > 0)
				{
					printf("%c%c%c", '\b'/*��������ƶ�*/, '\0'/*�ÿհ׷��滻ɾ����λ,����������һλ*/, '\b'/*g����������ƶ�,�����µ�һλ�滻�հ�*/);
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
		//����nλ,�������һλ����һλ����Ϊ��β����0
		Password[n] = 0;

		if (!strcmp("zhangsan", UserName) && !strcmp("123456", Password))
		{
			printf("\n��¼�ɹ�!\n");
			system("pause");
			break;
		}
		else
		{
			printf("\n��¼ʧ��!�����µ�¼\n");
			system("pause");
		}
	}
}
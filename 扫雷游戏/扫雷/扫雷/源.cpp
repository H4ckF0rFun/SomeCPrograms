#include<graphics.h>
#include<stdio.h>
#include<time.h>
#define S 50
#define N 10
IMAGE img[12];
void initgame();//��ʼ����Ϸ
void drawmap(int map[N + 2][N + 2]);//���Ƶ�ͼ
int msclick(int map[N + 2][N + 2]);//��갴��
void fk(int map[N + 2][N + 2], int, int);
int num;
void main()
{
	while (1)
	{
		num = 0;
		initgame();
		if (MessageBox(GetHWnd(), L"�Ƿ��ؿ�һ��", L"��ʾ", MB_YESNO) != IDYES)
		{
			break;
		}
	}
	return;
}
void initgame(){
	int map[N + 2][N + 2] = { 0 };//Ϊʲô����ȫ�ֱ�����ԭ��ȫ�ֱ����Ļ�ÿ�ζ�Ҫ��144��Ԫ�ض���ֵΪ0�����ֲ�����ÿ�ζ����ض���һ������
	srand((unsigned)time(NULL));
	initgraph(500, 500);
	//����
	for (int n = 1; n <= N;)
	{
		int i, j;

		//��������׵�����
		i = rand() % N + 1;
		j = rand() % N + 1;
		//�ж��Ƿ�����
		if (map[i][j] != -1){
			map[i][j] = -1;
			n++;
		}
	}
	//������
	//����
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			//�ж���������
			if (map[i][j] != -1)
			{
				//������Χ�м�����
				for (int a = i - 1; a <= i + 1; a++)
				{
					for (int b = j - 1; b <= j + 1; b++)
					{
						if (map[a][b] == -1)
							map[i][j]++;//��1���׾ͼ�1
					}
				}
			}
		}
	}

	loadimage(&img[0], L"0.jpg", S, S);
	loadimage(&img[1], L"1.jpg", S, S);
	loadimage(&img[2], L"2.jpg", S, S);
	loadimage(&img[3], L"3.jpg", S, S);
	loadimage(&img[4], L"4.jpg", S, S);
	loadimage(&img[5], L"5.jpg", S, S);
	loadimage(&img[6], L"6.jpg", S, S);
	loadimage(&img[7], L"7.jpg", S, S);
	loadimage(&img[8], L"8.jpg", S, S);
	loadimage(&img[9], L"9.jpg", S, S);//��ͼ
	loadimage(&img[10], L"10.jpg", S, S);//���
	loadimage(&img[11], L"11.jpg", S, S);//��                                                                                                                                                                                                                                                        
	drawmap(map);
	while (1)
	{
		int a = msclick(map);
		drawmap(map);
		if (a == 9)
		{
			MessageBox(GetHWnd(), L"������", L"��ʾ", MB_OK);
			break;
		}
		if (num == 90)
		{
			MessageBox(GetHWnd(), L"��Ӯ��", L"��ʾ", MB_OK);
			break;
		}
	}
	return;
}
void drawmap(int map[N + 2][N + 2]){
	//����
	BeginBatchDraw();
	cleardevice();
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			switch (map[i][j])
			{
			case 9://-1+10����갴�º��ʼ��-1+10=9�ͻ���һ��ͼ
				putimage((i - 1) * 50, (j - 1) * 50, &img[11]);
				break;
			case 10:
				putimage((i - 1) * 50, (j - 1) * 50, &img[0]);
				break;
			case 11:
				putimage((i - 1) * 50, (j - 1) * 50, &img[1]);
				break;
			case 12:
				putimage((i - 1) * 50, (j - 1) * 50, &img[2]);
				break;
			case 13:
				putimage((i - 1) * 50, (j - 1) * 50, &img[3]);
				break;
			case 14:
				putimage((i - 1) * 50, (j - 1) * 50, &img[4]);
				break;
			case 15:
				putimage((i - 1) * 50, (j - 1) * 50, &img[5]);
				break;
			case 16:
				putimage((i - 1) * 50, (j - 1) * 50, &img[6]);
				break;
			case 17:
				putimage((i - 1) * 50, (j - 1) * 50, &img[7]);
				break;
			case 18:
				putimage((i - 1) * 50, (j - 1) * 50, &img[8]);
				break;
			default:
				if (map[i][j] > 20)//��������
				{
					putimage((i - 1) * 50, (j - 1) * 50, &img[10]);
				}
				else{//���ʲôҲû��
					putimage((i - 1) * 50, (j - 1) * 50, &img[9]);		//û�еĻ���һ�ŵ�ͼ
				}
				break;
			}
		}
	}
	EndBatchDraw();
	return;
}
int msclick(int map[N + 2][N + 2])
{
	MOUSEMSG m = GetMouseMsg();
	//�������
	if (m.mkLButton == true)
	{
		//���û�з���
		if (map[m.x / S + 1][m.y / S + 1] < 9)
		{
			map[m.x / S + 1][m.y / S + 1] += 10;
			num++;
			//�����0����һƬ
			if (map[m.x / S + 1][m.y / S + 1] == 10)
			{
				fk(map, m.x / S + 1, m.y / S + 1);
			}
		}
		return map[m.x / S + 1][m.y / S + 1];
	}

	//�Ҽ�����
	if (m.mkRButton == true)
	{
		//���û�з���
		if (map[m.x / S + 1][m.y / S + 1] < 9)
		{
			map[m.x / S + 1][m.y / S + 1] += 30;	    //���ϱ��
		}
		else
		{
			//�ж��Ƿ񼺾����
			if (map[m.x / S + 1][m.y / S + 1]>20)		//����������
				map[m.x / S + 1][m.y / S + 1] -= 30;    //ȡ����� 
		}

	}
	return 0;

}
void fk(int map[N + 2][N + 2], int x, int y){
	//������Χ
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 1 && i <= 10 && j >= 1 && j <= 10)                 //�ж��Ƿ�Խ��
			{
				if (map[i][j] < 9)										//�ж��Ƿ񱻷���
				{

					if (map[i][j] == 0)									//�ж��Ƿ�Ϊ0
					{
						num++;											//num��¼��������(��������)
						map[i][j] = 10;
						fk(map, i, j);
					}
					else
					{
						map[i][j] += 10;
						num++;											//num��¼��������(��������)
					}
				}
			}
		}
	}
	return;
}
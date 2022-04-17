#include<graphics.h>
#include<stdio.h>
#include<time.h>
#define S 50
#define N 10
IMAGE img[12];
void initgame();//初始化游戏
void drawmap(int map[N + 2][N + 2]);//绘制地图
int msclick(int map[N + 2][N + 2]);//鼠标按下
void fk(int map[N + 2][N + 2], int, int);
int num;
void main()
{
	while (1)
	{
		num = 0;
		initgame();
		if (MessageBox(GetHWnd(), L"是否重开一局", L"提示", MB_YESNO) != IDYES)
		{
			break;
		}
	}
	return;
}
void initgame(){
	int map[N + 2][N + 2] = { 0 };//为什么不用全局变量的原因：全局变量的话每次都要给144个元素都赋值为0，而局部变量每次都是重定义一个数组
	srand((unsigned)time(NULL));
	initgraph(500, 500);
	//布雷
	for (int n = 1; n <= N;)
	{
		int i, j;

		//随机生成雷的坐标
		i = rand() % N + 1;
		j = rand() % N + 1;
		//判断是否有雷
		if (map[i][j] != -1){
			map[i][j] = -1;
			n++;
		}
	}
	//加数字
	//遍历
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			//判断若不是雷
			if (map[i][j] != -1)
			{
				//遍历周围有几个雷
				for (int a = i - 1; a <= i + 1; a++)
				{
					for (int b = j - 1; b <= j + 1; b++)
					{
						if (map[a][b] == -1)
							map[i][j]++;//有1个雷就加1
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
	loadimage(&img[9], L"9.jpg", S, S);//地图
	loadimage(&img[10], L"10.jpg", S, S);//标记
	loadimage(&img[11], L"11.jpg", S, S);//雷                                                                                                                                                                                                                                                        
	drawmap(map);
	while (1)
	{
		int a = msclick(map);
		drawmap(map);
		if (a == 9)
		{
			MessageBox(GetHWnd(), L"你输了", L"提示", MB_OK);
			break;
		}
		if (num == 90)
		{
			MessageBox(GetHWnd(), L"你赢了", L"提示", MB_OK);
			break;
		}
	}
	return;
}
void drawmap(int map[N + 2][N + 2]){
	//遍历
	BeginBatchDraw();
	cleardevice();
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			switch (map[i][j])
			{
			case 9://-1+10当鼠标按下后初始的-1+10=9就会贴一张图
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
				if (map[i][j] > 20)//如果被标记
				{
					putimage((i - 1) * 50, (j - 1) * 50, &img[10]);
				}
				else{//如果什么也没有
					putimage((i - 1) * 50, (j - 1) * 50, &img[9]);		//没有的话放一张地图
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
	//左键按下
	if (m.mkLButton == true)
	{
		//如果没有翻开
		if (map[m.x / S + 1][m.y / S + 1] < 9)
		{
			map[m.x / S + 1][m.y / S + 1] += 10;
			num++;
			//如果是0开翻一片
			if (map[m.x / S + 1][m.y / S + 1] == 10)
			{
				fk(map, m.x / S + 1, m.y / S + 1);
			}
		}
		return map[m.x / S + 1][m.y / S + 1];
	}

	//右键按下
	if (m.mkRButton == true)
	{
		//如果没有翻开
		if (map[m.x / S + 1][m.y / S + 1] < 9)
		{
			map[m.x / S + 1][m.y / S + 1] += 30;	    //加上标记
		}
		else
		{
			//判断是否己经标记
			if (map[m.x / S + 1][m.y / S + 1]>20)		//如果己经标记
				map[m.x / S + 1][m.y / S + 1] -= 30;    //取消标记 
		}

	}
	return 0;

}
void fk(int map[N + 2][N + 2], int x, int y){
	//遍历周围
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 1 && i <= 10 && j >= 1 && j <= 10)                 //判断是否越界
			{
				if (map[i][j] < 9)										//判断是否被翻开
				{

					if (map[i][j] == 0)									//判断是否为0
					{
						num++;											//num记录翻开个数(不包括雷)
						map[i][j] = 10;
						fk(map, i, j);
					}
					else
					{
						map[i][j] += 10;
						num++;											//num记录翻开个数(不包括雷)
					}
				}
			}
		}
	}
	return;
}
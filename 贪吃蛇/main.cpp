#include "head.h"
int main()
{
	InitGame();
	while (1)
	{

		
		MoveSnake();
		IsFail();
		CreateFood();
		DrawGame();

		if (kbhit())
		{
			ChangeDirect();
		}
		FoodIsEaten();
		if(snake.length==50)
		{
			MessageBox(GetHWnd(),L"你赢了",L"提示",MB_OK);
			exit(0);
		}
		Sleep(100);
		printf("蛇头坐标：X=%d,Y=%d\t\n食物坐标：X=%d,Y=%d\n",snake.Coor[0].x,snake.Coor[0].y,food.xy.x,food.xy.y);

	}
	return 0;
}
void InitGame()
{
	initgraph(500,500,SHOWCONSOLE);
	srand((unsigned)time(NULL));
	snake.direction = left;
	snake.length = 4;
	snake.Coor[0].x = 100;
	snake.Coor[0].y = 100;
	snake.Coor[1].x = 110;
	snake.Coor[1].y = 100;
	snake.Coor[2].x = 120;
	snake.Coor[2].y = 100;
	snake.Coor[3].x = 130;
	snake.Coor[3].y = 100;
	food.isEaten=true;
	return;
}
void DrawGame()
{
	BeginBatchDraw();
	cleardevice();
	for (int i = 0; i < snake.length; i++)
	{
		rectangle(snake.Coor[i].x, snake.Coor[i].y, snake.Coor[i].x + 10, snake.Coor[i].y + 10);
	}
	roundrect(food.xy.x,food.xy.y,food.xy.x+10,food.xy.y+10,10,10);
	EndBatchDraw();
	return;
}
void MoveSnake()
{
	//设置蛇身体的坐标
	for (int i = snake.length - 1; i>0; i--)
	{
		snake.Coor[i].x = snake.Coor[i - 1].x;
		snake.Coor[i].y = snake.Coor[i - 1].y;
	}
	//头
	switch (snake.direction)
	{
	case left:
		snake.Coor[0].x -= 10;
		break;
	case right:
		snake.Coor[0].x += 10;
		break;
	case up:
		snake.Coor[0].y -= 10;
		break;
	case down:
		snake.Coor[0].y += 10;
		break;
	}
	return;
}
void ChangeDirect()
{
	char c;
		c = getch();
		switch (c)
		{
		case up:
			if (snake.direction!=down)
				snake.direction = up;
			break;
		case down:
			if (snake.direction != up)
				snake.direction = down;
			break;
		case left:
			if (snake.direction != right)
				snake.direction = left;
			break;
		case right:
			if (snake.direction != left)
				snake.direction = right;
			break;
		}
	return;
}
void CreateFood()
{
	if(food.isEaten==true)
	{
		do{
		food.xy.x=rand()%50*10;
		food.xy.y=rand()%50*10;
		}while(Check());
		food.isEaten=false;
	}
	return;
}
bool Check()
{
	for(int i=0;i<snake.length;i++)
	{
		if(food.xy.x==snake.Coor[i].x&&food.xy.y==snake.Coor[i].y)
		{
			return true;
		}
	}
	return false;
}
void FoodIsEaten()
{
	if(snake.Coor[0].x==food.xy.x&&snake.Coor[0].y==food.xy.y)
	{
		food.isEaten=true;
		snake.length++;
	}
	return;
}
 void IsFail()
 {
	 bool fail=false;
	 HWND hwnd=GetHWnd();
	 if(snake.Coor[0].x<0||snake.Coor[0].x>490||snake.Coor[0].y<0||snake.Coor[0].y>490)
		 {
			 fail=true;
		}
	 for(int i=1;i<snake.length;i++)
	 {
		 if(snake.Coor[0].x==snake.Coor[i].x&&snake.Coor[0].y==snake.Coor[i].y)
		 {
			 fail=true;
		 }
	 }
	 if(fail==true)
	 {
		 MessageBox(hwnd,L"你输了",L"提示",MB_OK);
		 closegraph();
		 exit(0);
	 }
	 return ;
 }
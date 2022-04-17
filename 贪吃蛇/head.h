#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#define MAXLENGTH 50

enum direct				//方向
{
	up		= 72,		//上
	down	= 80,		//下
	left	= 75,		//左
	right	= 77		//右
};
struct coor				//坐标
{
	int x;
	int y;
};
struct Snake			//头
{
	direct  direction;
	int     length;
	coor	Coor[50];
};
Snake snake;
struct Food
{
	struct coor xy;
	bool isEaten;
}food;
void InitGame();
void DrawGame();
void MoveSnake();
void ChangeDirect();
void CreateFood();
bool Check();			//检测食物是否与蛇重合
void FoodIsEaten();
void IsFail();			//是否失败
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#define MAXLENGTH 50

enum direct				//����
{
	up		= 72,		//��
	down	= 80,		//��
	left	= 75,		//��
	right	= 77		//��
};
struct coor				//����
{
	int x;
	int y;
};
struct Snake			//ͷ
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
bool Check();			//���ʳ���Ƿ������غ�
void FoodIsEaten();
void IsFail();			//�Ƿ�ʧ��
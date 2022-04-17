#pragma once

#include <iostream>
#include <conio.h>
using namespace std;
#define WALL	-1
#define FLOOR	0
#define PERSON	1
#define BOX		2
#define AIM		3
#define PANDA   4
#define BANDA	5
class BoxGame
{
private:
	int*	m_pMap;
	int		m_Height;
	int		m_Width;
	int		m_current;
	int		m_total;

	
	int* map(int i, int j);
	//记录每一步
	struct node;
	node* m_head;
	void AddToStepsList(char direction,bool boxmove);
	void GoToLastPos();
	void InitStepsList();
	void ClearStepsList();

	//移动人物
	void Move(char ch);

	//完成度
	int m_BoxNum;
	int m_FinishNum;
	bool IsOver();
	void InitBoxNum();

	//更新显示
	
	void UpdateShow();
public:
	//1.退出 2.重来 3.上一关 4.下一关
	void ShowTips();
	int StartGame(int *pSrcMap,int Height,int Width,int Current,int Total);
};


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
	//��¼ÿһ��
	struct node;
	node* m_head;
	void AddToStepsList(char direction,bool boxmove);
	void GoToLastPos();
	void InitStepsList();
	void ClearStepsList();

	//�ƶ�����
	void Move(char ch);

	//��ɶ�
	int m_BoxNum;
	int m_FinishNum;
	bool IsOver();
	void InitBoxNum();

	//������ʾ
	
	void UpdateShow();
public:
	//1.�˳� 2.���� 3.��һ�� 4.��һ��
	void ShowTips();
	int StartGame(int *pSrcMap,int Height,int Width,int Current,int Total);
};


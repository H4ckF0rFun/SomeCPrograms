#include "BoxGame.h"




struct BoxGame::node
{
	char PerDirection;
	bool BoxMove;
	node *next;
};

int* BoxGame::map(int i, int j)
{
	return (m_pMap + i*m_Width + j);
}
void BoxGame::AddToStepsList(char direction, bool boxmove)
{
	node*p = new node;
	p->next = m_head->next;
	m_head->next = p;
	p->PerDirection = direction;
	p->BoxMove = boxmove;
}
void BoxGame::GoToLastPos()
{
	node*last = m_head->next;
	if (last == 0)
	{
		return;
	}

	int *p = m_pMap;
	int i;
	for (i = 0; i < m_Width*m_Height; i++)
	{
		if (p[i] == PERSON || p[i] == PANDA)
		{
			break;
		}
	}
	int WidthPos = i%m_Width;
	int HeightPos = i / m_Width;

	*map(HeightPos,WidthPos) -= PERSON;

	switch (last->PerDirection)
	{
	case 'w':
		//人物往下移动
		*map(HeightPos + 1, WidthPos) += PERSON;
		if (last->BoxMove == true)
		{
			if (*map(HeightPos - 1, WidthPos) == BANDA)
				m_FinishNum--;
			//箱子往下移动
			*map(HeightPos - 1, WidthPos) -= BOX;
			*map(HeightPos, WidthPos) += BOX;
			if (*map(HeightPos, WidthPos) == BANDA)
				m_FinishNum++;
		}
		break;
	case 's':
		*map(HeightPos - 1, WidthPos) += PERSON;
		if (last->BoxMove == true)
		{
			if (*map(HeightPos + 1, WidthPos) == BANDA)
				m_FinishNum--;
			//箱子往上移动
			*map(HeightPos + 1, WidthPos) -= BOX;
			*map(HeightPos, WidthPos) += BOX;
			if (*map(HeightPos, WidthPos) == BANDA)
				m_FinishNum++;
		}
		break;
	case 'a':
		*map(HeightPos , WidthPos+1) += PERSON;
		if (last->BoxMove == true)
		{
			if (*map(HeightPos , WidthPos-1) == BANDA)
				m_FinishNum--;
			//箱子往右移动
			*map(HeightPos , WidthPos-1) -= BOX;
			*map(HeightPos, WidthPos) += BOX;
			if (*map(HeightPos, WidthPos) == BANDA)
				m_FinishNum++;
		}
		break;
	case 'd':
		*map(HeightPos, WidthPos - 1) += PERSON;
		if (last->BoxMove == true)
		{
			if (*map(HeightPos, WidthPos + 1) == BANDA)
				m_FinishNum--;
			//箱子往左移动
			*map(HeightPos, WidthPos + 1) -= BOX;
			*map(HeightPos, WidthPos) += BOX;
			if (*map(HeightPos, WidthPos) == BANDA)
				m_FinishNum++;
		}
		break;
	default:
		break;
	}
	m_head->next = last->next;
	delete last;
}

bool BoxGame::IsOver()
{
	return !(m_BoxNum - m_FinishNum);
}

void BoxGame::ShowTips()
{
	cout << " ____________________________" << endl
		<< "│                           │" << endl
		<< "│       控制台推箱子        │" << endl
		<< "│___________________________│" << endl
		<< "│       玩法说明:           │" << endl
		<< "│___________________________│" << endl
		<< "│       -w 向上移动         │" << endl
		<< "│       -s 向下移动	     │" << endl
		<< "│       -a 向左移动         │" << endl
		<< "│       -d 向右移动	     │" << endl
		<< "│       -r 重新开始	     │" << endl
		<< "│       -b 回退一步	     │" << endl
		<< "│       -l 上一关           │" << endl
		<< "│       -n 下一关           │" << endl
		<< "│       -q 退出游戏         │" << endl
		<< "│___________________________│" << endl
		<< "按下任意键开始游戏.........." << endl;
	getch();
}

void BoxGame::UpdateShow()
{
	system("cls");

	printf(" _____________________\n");
	printf("│   当前关卡:%2d/%2d   │\n",m_current,m_total);
	printf("│____________________│\n");
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			switch (*map(i, j))
			{
			case WALL:
				cout << "■";
				break;
			case FLOOR :
				cout << "  ";
				break;
			case PERSON:
				cout << "♀";//
				break;
			case BOX:
				cout << "□";//
				break;
			case AIM :
				cout <<"○" ;
				break;
			case PANDA:
				cout << "♀";//
				break;
			case BANDA :
				cout << "●";
				break;
			default :
				break;
			}
		}
		cout<<endl;
	}
}

void BoxGame::InitStepsList()
{
	m_head = new node;
	m_head->next = NULL;
}

void BoxGame::ClearStepsList()
{
	node*p = m_head;
	node*tp;
	while (p != NULL)
	{
		tp = p;
		p = p->next;
		delete tp;
	}
}

void BoxGame::InitBoxNum()
{
	m_BoxNum = 0;
	m_FinishNum = 0;
	int *p = m_pMap;
	for (int i = 0; i < m_Width*m_Height; i++)
	{
		if (p[i] == BOX || p[i] == BANDA)
		{
			m_BoxNum++;
			if (p[i] == BANDA)
				m_FinishNum++;
		}
	}
}

void BoxGame::Move(char ch)
{
	int *p = m_pMap;
	int i;
	for (i = 0; i < m_Width*m_Height; i++)
	{
		if (p[i] == PERSON || p[i] == PANDA)
			break;
	}
	int WidthPos = i%m_Width;
	int HeightPos = i / m_Width;
	switch (ch)
	{
	case 'w':
		if (*map(HeightPos - 1, WidthPos) == FLOOR || *map(HeightPos - 1, WidthPos) == AIM)
		{
			*map(HeightPos - 1, WidthPos) += PERSON;
			*map(HeightPos, WidthPos) -= PERSON;
			AddToStepsList('w', false);
		}
		else
		{
			if (*map(HeightPos - 1, WidthPos) == BOX || *map(HeightPos - 1, WidthPos) == BANDA)
			{
				if (*map(HeightPos - 2, WidthPos) == FLOOR || *map(HeightPos - 2, WidthPos) == AIM)
				{
					if (*map(HeightPos - 1, WidthPos) == BANDA)
						m_FinishNum--;
					*map(HeightPos, WidthPos) -= PERSON;
					*map(HeightPos-1, WidthPos) -= BOX;
					*map(HeightPos - 1, WidthPos) += PERSON;
					*map(HeightPos - 2, WidthPos) += BOX;
					if (*map(HeightPos - 2, WidthPos) == BANDA)
						m_FinishNum++;
					AddToStepsList('w', true);
				}
			}
		}



		break;
	case 's':
		if (*map(HeightPos + 1, WidthPos) == FLOOR || *map(HeightPos + 1, WidthPos) == AIM)
		{
			*map(HeightPos + 1, WidthPos) += PERSON;
			*map(HeightPos, WidthPos) -= PERSON;
			AddToStepsList('s', false);
		}
		else
		{
			if (*map(HeightPos + 1, WidthPos) == BOX || *map(HeightPos + 1, WidthPos) == BANDA)
			{
				if (*map(HeightPos + 2, WidthPos) == FLOOR || *map(HeightPos + 2, WidthPos) == AIM)
				{
					if (*map(HeightPos + 1, WidthPos) == BANDA)
						m_FinishNum--;
					*map(HeightPos, WidthPos) -= PERSON;
					*map(HeightPos + 1, WidthPos) -= BOX;
					*map(HeightPos + 1, WidthPos) += PERSON;
					*map(HeightPos + 2, WidthPos) += BOX;
					if (*map(HeightPos + 2, WidthPos) == BANDA)
						m_FinishNum++;
					AddToStepsList('s', true);
				}
			}
		}
		break;
	case 'a':
		if (*map(HeightPos , WidthPos-1) == FLOOR || *map(HeightPos, WidthPos-1) == AIM)
		{
			*map(HeightPos , WidthPos-1) += PERSON;
			*map(HeightPos, WidthPos) -= PERSON;
			AddToStepsList('a', false);
		}
		else
		{
			if (*map(HeightPos , WidthPos-1) == BOX || *map(HeightPos , WidthPos-1) == BANDA)
			{
				if (*map(HeightPos , WidthPos-2) == FLOOR || *map(HeightPos , WidthPos-2) == AIM)
				{
					if (*map(HeightPos , WidthPos-1) == BANDA)
						m_FinishNum--;
					*map(HeightPos, WidthPos) -= PERSON;
					*map(HeightPos , WidthPos-1) -= BOX;
					*map(HeightPos , WidthPos-1) += PERSON;
					*map(HeightPos , WidthPos-2) += BOX;
					if (*map(HeightPos, WidthPos-2) == BANDA)
						m_FinishNum++;
					AddToStepsList('a', true);
				}
			}
		}
		break;
	case 'd':
		if (*map(HeightPos, WidthPos + 1) == FLOOR || *map(HeightPos, WidthPos + 1) == AIM)
		{
			*map(HeightPos, WidthPos + 1) += PERSON;
			*map(HeightPos, WidthPos) -= PERSON;
			AddToStepsList('d', false);
		}
		else
		{
			if (*map(HeightPos, WidthPos + 1) == BOX || *map(HeightPos, WidthPos + 1) == BANDA)
			{
				if (*map(HeightPos, WidthPos + 2) == FLOOR || *map(HeightPos, WidthPos + 2) == AIM)
				{
					if (*map(HeightPos, WidthPos + 1) == BANDA)
						m_FinishNum--;
					*map(HeightPos, WidthPos) -= PERSON;
					*map(HeightPos, WidthPos + 1) -= BOX;
					*map(HeightPos, WidthPos + 1) += PERSON;
					*map(HeightPos, WidthPos + 2) += BOX;
					if (*map(HeightPos, WidthPos + 2) == BANDA)
						m_FinishNum++;
					AddToStepsList('d', true);
				}
			}
		}
		break;

	default:
		break;
	}
}

int BoxGame::StartGame(int *pSrcMap, int Height, int Width, int Current, int Total)
{
	m_pMap = new int[Height*Width];
	memcpy(m_pMap, pSrcMap, sizeof(int)*Height*Width);

	m_Height = Height;
	m_Width = Width;
	m_current = Current;
	m_total = Total;
	InitBoxNum();
	InitStepsList();
	

	char ch;
	int opt, end;
	while (1)
	{
		UpdateShow();
		if (IsOver())
		{
			cout << endl << "游戏结束!!!恭喜您赢了。" << endl
				<< "-q 退出" << endl
				<< "-r 重来" << endl
				<< "-l 上一关" << endl
				<< "-n 下一关" << endl;
			end = 1;
		}
		else
		{
			end = 0;
		}
		ch = getch();
		if (ch == 'q')
		{
			opt = 1;
			break;
		}
		else if (ch == 'r')
		{
			opt = 2;
			break;
		}
		else if (ch == 'l')
		{
			opt = 3;
			break;
		}
		else if (ch == 'n')
		{
			opt = 4;
			break;
		}
		else if (ch == 'b'&&end != 1)
		{
			GoToLastPos();
			continue;
		}
		Move(ch);
	}
	ClearStepsList();
	return opt;
}
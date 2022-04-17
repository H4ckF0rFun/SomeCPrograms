#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include "block.h"
#define WIDTH (10+7)
#define HEIGHT (16)

#define BLOCKSIZE 25
using namespace std;
HBRUSH hBrush[9] = { 0 };

struct Node
{
	int m_value;
	int type;
};

int CurrentX=0, CurrentY=0;
int CurrentBlock=-1;
int*CurrentBlockBuf=0;
int NextBlock=-1;
int*NextBlockBuf=0;
int lines = 0;
bool IsPlaying = false;
bool Pause = false;
bool GameOver = false;
Node map[20][14];


int* getaddr(int *data, int i, int j, int stride)
{
	return (data + i*stride + j);
}

void change(int* shape, int stride)
{
	int temp;
	for (int count = stride, dt = 0; count>1; count -= 2, dt++)
	{
		for (int i = count - 1; i>0; i--)
		{
			temp = *getaddr(shape, 0 + dt, count - 1 - i + dt, stride);
			*getaddr(shape, dt + 0, dt + count - i - 1, stride) = *getaddr(shape, dt + i, dt + 0, stride);
			*getaddr(shape, dt + i, dt + 0, stride) = *getaddr(shape, dt + count - 1, dt + i, stride);
			*getaddr(shape, dt + count - 1, dt + i, stride) = *getaddr(shape, dt + count - i - 1, dt + count - 1, stride);
			*getaddr(shape, dt + count - i - 1, dt + count - 1, stride) = temp;
		}
	}
}
void InitGame()
{
	//清空地图
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (i >= 2 && i <= 17 && j >= 2 && j <= 11)
				map[i][j].m_value = 0;
			else
				map[i][j].m_value = 1;
			map[i][j].type = 0;
		}
	}
	//
	CurrentBlock = NextBlock = -1;
	if (CurrentBlockBuf)	delete[]CurrentBlockBuf;
	if (NextBlockBuf)	delete[]NextBlockBuf;
	CurrentBlockBuf = NextBlockBuf = NULL;
	CurrentX = CurrentY = 0;
	lines = 0;
	IsPlaying = true;
	Pause = false;
	GameOver = false;
}

//获取重叠个数
int GetOverlapsCount()
{
	int count = 0;
	for (int i = CurrentY; i < CurrentY + BlockSet[CurrentBlock].stride; i++)
	{
		for (int j = CurrentX; j < CurrentX + BlockSet[CurrentBlock].stride; j++)
		{
			if (map[i][j].m_value != 0 && map[i][j].m_value != 1)
				count++;
		}
	}
	return count;
}
//把当前的块放到,或移出地图上
void ToMap(BOOL put=TRUE)
{
	for (int i = CurrentY; i < CurrentY + BlockSet[CurrentBlock].stride; i++)
	{
		for (int j = CurrentX; j < CurrentX + BlockSet[CurrentBlock].stride; j++)
		{
			if (put)
			{
				if (*getaddr(CurrentBlockBuf, i - CurrentY, j - CurrentX, BlockSet[CurrentBlock].stride))
				{
					map[i][j].m_value += *getaddr(CurrentBlockBuf, i - CurrentY, j - CurrentX, BlockSet[CurrentBlock].stride);
					map[i][j].type += CurrentBlock;
				}
			}
			else
			{
				if (*getaddr(CurrentBlockBuf, i - CurrentY, j - CurrentX, BlockSet[CurrentBlock].stride))
				{
					map[i][j].m_value -= *getaddr(CurrentBlockBuf, i - CurrentY, j - CurrentX, BlockSet[CurrentBlock].stride);
					map[i][j].type -= CurrentBlock;
				}
			}
		}
	}
}
void Move(int direction)
{
	//上 0，右 1，下 2，左 3
	ToMap(FALSE);
	switch (direction)
	{
	case 0:
		CurrentY--;
		break;
	case 1:
		CurrentX++;
		break;
	case 2:
		CurrentY++;
		break;
	case 3:
		CurrentX--;
	}
	ToMap(TRUE);
}
//生成一个块
bool MakeBlock()
{
	if (NextBlock == -1)
	{
		CurrentBlock = rand() % 9;
		if (CurrentBlockBuf) delete[]CurrentBlockBuf;
		CurrentBlockBuf = new int[BlockSet[CurrentBlock].stride*BlockSet[CurrentBlock].stride*sizeof(int)];
		memcpy(CurrentBlockBuf, BlockSet[CurrentBlock].m_pData, BlockSet[CurrentBlock].stride*BlockSet[CurrentBlock].stride*sizeof(int));
	}
	else
	{
		CurrentBlock = NextBlock;
		if (CurrentBlockBuf) delete[]CurrentBlockBuf;
		CurrentBlockBuf = new int[BlockSet[NextBlock].stride*BlockSet[NextBlock].stride*sizeof(int)];
		memcpy(CurrentBlockBuf, NextBlockBuf, BlockSet[NextBlock].stride*BlockSet[NextBlock].stride*sizeof(int));
	}
	//make next block
	NextBlock = rand() % 9;
	if (NextBlockBuf) delete[]NextBlockBuf;
	NextBlockBuf = new int[BlockSet[NextBlock].stride*BlockSet[NextBlock].stride*sizeof(int)];
	memcpy(NextBlockBuf, BlockSet[NextBlock].m_pData, BlockSet[NextBlock].stride*BlockSet[NextBlock].stride*sizeof(int));
	//随机旋转n次
	int j1 = rand() % 4 + 1;
	for (int i = 0; i < j1; i++)
		change(NextBlockBuf, BlockSet[NextBlock].stride);
	
	CurrentY = 0;
	CurrentX = 7 - BlockSet[CurrentBlock].stride / 2;
	ToMap(TRUE);
	bool ret = true;
	int count;
	while ((count=GetOverlapsCount()))
	{
		int count1 = 0;
		for (int i = 0; i < 2; i++)
			for (int j = 2; j < 12; j++)
				if (map[i][j].m_value != 0 && map[i][j].m_value != 1)
					count1++;
		if (count != count1)
			return false;
		Move(2);
	}
	return true;
}

int GetLineBlockCount(int line)
{
	int count = 0;
	for (int j = 2; j < 12; j++)
		if (map[line][j].m_value == 1)
			count++;
	return count;
}
void ClearLine(int line)
{
	lines++;
	for (int i = line; i > 2; i--)
	{
		for (int j = 2; j < 12; j++)
		{
			map[i][j].m_value = map[i - 1][j].m_value;
			map[i][j].type = map[i - 1][j].type;
		}
	}
	memset(&map[2][2], 0, 10 * sizeof(Node));
}

DWORD t1, t2;

void OnTimer(HWND hWnd)
{
	//向下移动一格
	if (!Pause)
	{
		if ((t2 = GetTickCount()) - t1 >= 500)
		{
			t1 = t2;
			Move(2);
			if (GetOverlapsCount())
			{
				Move(0);
				for (int i = 17; i >= 2;)
				{
					if (GetLineBlockCount(i) == 10)
						ClearLine(i);
					else
						i--;
				}
				if (MakeBlock() == false)
				{
					GameOver = true;
					KillTimer(hWnd, 10086);
				}
			}
		}
	}
	//post wm_paint to redraw window;
	RECT rect;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, FALSE);
}

void OnKeyDown(char ch)
{
	int*temp;
	switch (ch)
	{
	case 'w':
		temp = new int[BlockSet[CurrentBlock].stride*BlockSet[CurrentBlock].stride*sizeof(int)];
		memcpy(temp, CurrentBlockBuf, BlockSet[CurrentBlock].stride*BlockSet[CurrentBlock].stride*sizeof(int));
		ToMap(FALSE);
		change(CurrentBlockBuf, BlockSet[CurrentBlock].stride);
		ToMap(TRUE);
		if (GetOverlapsCount())
		{
			ToMap(FALSE);
			memcpy(CurrentBlockBuf, temp, BlockSet[CurrentBlock].stride*BlockSet[CurrentBlock].stride*sizeof(int));
			ToMap(TRUE);
		}
		delete[]temp;
		break;
	case 's':
		Move(2);
		if (GetOverlapsCount())
			Move(0);
		break;
	case 'a':
		Move(3);
		if (GetOverlapsCount())
			Move(1);
		break;
	case 'd':
		Move(1);
		if (GetOverlapsCount())
			Move(3);
		break;

	}
}

void SetBlockColor(HDC hdc, int type)
{
	if (type >= 0 && type <= 8)
		SelectObject(hdc, hBrush[type]);
}
void DrawGame(HWND hWnd,HDC hdc)
{
	HDC mdc = CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc,rect.right-rect.left,rect.bottom - rect.top);
	SelectObject(mdc, hBitmap);
	FillRect(mdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	SelectObject(mdc, GetStockObject(WHITE_BRUSH));
	//绘制三个矩形
	Rectangle(mdc, 10 + 0, 10 + 0, 10 + 10 * BLOCKSIZE, 10 + 16 * BLOCKSIZE);
	Rectangle(mdc, 20 + 10 * BLOCKSIZE, 10, 20 + 17 * BLOCKSIZE, 10 + 5 * BLOCKSIZE);
	Rectangle(mdc, 20 + 10 * BLOCKSIZE, 20 + 5 * BLOCKSIZE, 20 + 17 * BLOCKSIZE, 10 + 16 * BLOCKSIZE);

	//绘制地图
	for (int i = 2; i <=17; i++)
	{
		for (int j = 2; j <= 11; j++)
		{
			if (map[i][j].m_value == 0)continue;
			SetBlockColor(mdc, map[i][j].type);
			Rectangle(mdc, 10 + (j - 2)*BLOCKSIZE, 10 + (i - 2)*BLOCKSIZE, 10 + (j - 2)*BLOCKSIZE + BLOCKSIZE, 10 + (i - 2)*BLOCKSIZE + BLOCKSIZE);
		}
	}
	//绘制提示区域内容
	if (NextBlock != -1 && NextBlockBuf)
	{
		SetBlockColor(mdc, NextBlock);
		int StartX = 20 + 10 * BLOCKSIZE + (3 - BlockSet[NextBlock].stride / 2)*BLOCKSIZE;
		int StartY = 10 + (2 - BlockSet[NextBlock].stride / 2)*BLOCKSIZE;
		for (int i = 0; i < BlockSet[NextBlock].stride; i++)
		{
			for (int j = 0; j < BlockSet[NextBlock].stride; j++)
			{
				if (*getaddr(NextBlockBuf, i, j, BlockSet[NextBlock].stride))
				{
					Rectangle(mdc, StartX + j*BLOCKSIZE, StartY + i*BLOCKSIZE, StartX + (j + 1)*BLOCKSIZE, StartY + (i + 1)*BLOCKSIZE);
				}
			}
		}
	}
	
	//
	char str_lines[32] = { 0 };
	sprintf(str_lines, "Lines:  %d", lines);
	TextOutA(mdc, 20 + 10 * BLOCKSIZE + 50, 20 + 5 * BLOCKSIZE + 20, str_lines, strlen(str_lines));
	if (GameOver)
		TextOutA(mdc, 20 + 10 * BLOCKSIZE + 50, 20 + 5 * BLOCKSIZE + 40, "GameOver", strlen("GameOver"));
	SetBkColor(mdc, RGB(193, 224, 220));
	//SetTextColor(hdc,)
	TextOutA(mdc, 20 + 10 * BLOCKSIZE + 50, 20 + 5 * BLOCKSIZE + 80, "开始新游戏", strlen("开始新游戏"));
	TextOutA(mdc, 20 + 10 * BLOCKSIZE + 60, 20 + 5 * BLOCKSIZE + 130, "暂停游戏", strlen("暂停游戏"));
	TextOutA(mdc, 20 + 10 * BLOCKSIZE + 60, 20 + 5 * BLOCKSIZE + 180, "退出游戏", strlen("退出游戏"));

	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(mdc);
}

void OnMouseClick(HWND hWnd,int x, int y)
{
	if (x >= 320 && y >= 225 &&x<=394&&y<=240)
	{
		if (IsPlaying)
			KillTimer(hWnd, 10086);
		InitGame();
		MakeBlock();
		t1 = GetTickCount();
		SetTimer(hWnd, 10086, 80, NULL);
	}
	if (x >= 330 && y >= 275 && x <= 390 && y <= 290)
	{
		//Pause
		if (IsPlaying)
		{
			if (Pause)
				Pause = false;
			else
				Pause = true;
		}
	}
	if (x >= 330 && y >= 325 && x <= 390 && y <= 340)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnMouseClick(hWnd,LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CHAR:
		OnKeyDown((char)wParam);
		break;
	case WM_CREATE:

		hBrush[0] = CreateSolidBrush(RGB(255,0, 0));
		hBrush[1] = CreateSolidBrush(RGB(199, 162, 0));
		hBrush[2] = CreateSolidBrush(RGB(255, 255, 0));
		hBrush[3] = CreateSolidBrush(RGB(0, 255,0));
		hBrush[4] = CreateSolidBrush(RGB(102, 204, 255));
		hBrush[5] = CreateSolidBrush(RGB(204, 0, 255));
		hBrush[6] = CreateSolidBrush(RGB(0, 0, 255));
		hBrush[7] = CreateSolidBrush(RGB(255, 102, 0));
		hBrush[8] = CreateSolidBrush(RGB(255, 159, 0));
	
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hWnd, TEXT("确定要退出游戏吗?"), TEXT("提示"), MB_YESNO))
			DestroyWindow(hWnd);
		break;
	case WM_TIMER:
		OnTimer(hWnd);
		break;
	case WM_DESTROY:
		for (int i = 0; i < 9; i++)
			DeleteObject(hBrush[i]);
		InitGame();
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawGame(hWnd,hdc);
		EndPaint(hWnd, &ps);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	srand((unsigned)time(NULL));

	WNDCLASS wndclass = { 0 };
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wndclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = TEXT("elsfk");
	wndclass.lpfnWndProc = WndProc;
	
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(TEXT("elsfk"), TEXT("俄罗斯方块"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 40+WIDTH*BLOCKSIZE, 50+HEIGHT*BLOCKSIZE, 0, 0, hInstance, 0);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
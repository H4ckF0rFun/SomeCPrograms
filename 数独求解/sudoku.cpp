#include <iostream>
#include <ctime>
#include <fstream>
#define START 0
using namespace std;
struct Num
{
	int probablity;
	int probably_n[9];
	int current_num;
	int pos_h;
	int pos_l;
};

Num num[9][9];

Num* p_pos = num[0];
int totalsloution;
int maxsolution;
void Input();
void get_h_existed(int h, int start_l, bool*record);
void get_l_existed(int start_h, int l, bool*record);
void get_block_existed(int start_h, int start_l, bool*record);
inline int get_start_hl(int hl);
void set_probablity();
void FindNext(int n);
void showprobablity();
void show_result();

int main()
{
	while (1)
	{
		totalsloution = 0;
		maxsolution = 0;
		system("cls");
		Input();
		clock_t start = clock();
		//求出每一个位置可能的情况
		set_probablity();

		//输出每一个位置可能的情况
		showprobablity();
		//开始查找解法
		FindNext(START);
		cout << "完成！" << endl << endl;
		cout << "用时:" << (clock() - start) / CLOCKS_PER_SEC << "s" << endl;
		cout << endl << "共找出" << totalsloution << "种解法" << endl;
		cin.get();
		cin.get();
	}
	return 0;
}


void Input()
{
	cout << "-------开始输入数独(待填处用0表示)-------" << endl<<endl;
	for (int i = 0; i < 9; i++)
	{
		cout << "第" << i + 1 << "行:" << endl;
		for (int j = 0; j < 9; j++)
		{
			cin >> num[i][j].current_num;
			num[i][j].pos_h = i;
			num[i][j].pos_l = j;
			if (num[i][j].current_num)
			{
				num[i][j].probablity = -1;
			}
			else
			{
				num[i][j].probablity = 0;
			}
		}
		cout << endl;
	}
	cout << "输入完成！" << endl;
	while (1)
	{
		cout << endl << "最多找出几种解法(负数找出所有):";
		cin >> maxsolution;
		if (maxsolution != 0)
			break;
		cout << "?????????" << endl;
	}
	
}

void showprobablity()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << endl;
			cout << num[i][j].current_num << '\t';
			if (num[i][j].probablity == -1)
				continue;
			for (int a = 0; a < num[i][j].probablity; a++)
				cout << num[i][j].probably_n[a] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

void get_h_existed(int h, int start_l, bool*record)
{
	for (int i = start_l - 1; i >= 0; i--)
	{
		record[num[h][i].current_num] = true;
	}
	for (int i = start_l + 3; i <= 8; i++)
	{
		record[num[h][i].current_num] = true;
	}
}

void get_l_existed(int start_h, int l, bool*record)
{
	for (int i = start_h - 1; i >= 0; i--)
	{
		record[num[i][l].current_num] = true;
	}
	for (int i = start_h + 3; i <= 8; i++)
	{
		record[num[i][l].current_num] = true;
	}
}

void get_block_existed(int start_h, int start_l, bool*record)
{
	for (int i = start_h; i < start_h + 3; i++)
	{
		for (int j = start_l; j < start_l + 3; j++)
		{
			record[num[i][j].current_num] = true;
		}
	}
}

inline int get_start_hl(int hl)
{
	return ((hl + 1) % 3 == 0) ? ((hl>0) ? (hl - 2) : hl) : (hl - ((hl + 1) % 3 - 1));
}

void set_probablity()
{
	for (int i = 0; i < 9; i += 3)
	{
		for (int j = 0; j < 9; j += 3)
		{
			bool record_block[10] = { 0 };
			bool record_h[3][10] = { 0 };
			bool record_l[3][10] = { 0 };
			get_block_existed(i, j, record_block);
			for (int n = 0; n < 3; n++)
			{
				get_h_existed(i + n, j, record_h[n]);
				get_l_existed(i, j + n, record_l[n]);
			}
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 3; n++)
				{
					if (num[i + m][j + n].probablity == -1)
						continue;
					for (int a = 1; a < 10; a++)
					{
						if (!record_block[a] && !record_h[m][a] && !record_l[n][a])
						{
							num[i + m][j + n].probablity++;
							num[i + m][j + n].probably_n[num[i + m][j + n].probablity - 1] = a;
						}
					}
				}
			}
		}
	}
}


void FindNext(int n)
{

	if (n == 81)
	{
		show_result();
		totalsloution++;
		return;
	}

	if (p_pos[n].probablity == -1)
	{
		FindNext(n + 1);
		return;
	}


	bool record[10] = { 0 };
	int start_h = get_start_hl(p_pos[n].pos_h);
	int start_l = get_start_hl(p_pos[n].pos_l);

	get_block_existed(start_h, start_l, record);
	get_h_existed(p_pos[n].pos_h, start_l, record);
	get_l_existed(start_h, p_pos[n].pos_l, record);


	for (int i = 0; i < p_pos[n].probablity; i++)
	{
		if (totalsloution == maxsolution)
			return;
		if (!record[p_pos[n].probably_n[i]])
		{
			p_pos[n].current_num = p_pos[n].probably_n[i];
			FindNext(n + 1);
		}
		p_pos[n].current_num = 0;
	}
	return;
}
//输出结果
void show_result()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << num[i][j].current_num << '\t';
		}
		cout << endl << endl;
	}
	cout << endl << endl;
}

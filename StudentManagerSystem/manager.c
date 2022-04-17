#include "common.h"
#include "list.h"
#include "manager.h"
/*
    这个是成绩管理系统的主程序
*/

//姓名查询
int search_cmp_name(void *it, void *target)
{
	return strcmp(((pStu)it)->name, ((pStu)target)->name);
}

//id查询
int search_cmp_id(void *it, void *target)
{
	return strcmp(((pStu)it)->id, ((pStu)target)->id);
}

int sort_cmp_math(void*it, void*target)
{
	return (((pStu)it)->math) - (((pStu)target)->math);
}

int sort_cmp_english(void*it, void*target)
{
	return (((pStu)it)->english) - (((pStu)target)->english);
}

int sort_cmp_c(void*it, void*target)
{
	return (((pStu)it)->c_language) - (((pStu)target)->c_language);
}

int sort_cmp_total(void*it, void*target)
{
	pStu stu1 = (pStu)it;
	pStu stu2 = (pStu)target;

	int total1, total2 = 0;
	total1 = stu1->math + stu1->english + stu1->c_language;
	total2 = stu2->math + stu2->english + stu2->c_language;
	return total1 - total2;
}

int sort_cmp_id(void*it, void *target)
{
	pStu stu1 = (pStu)it;
	pStu stu2 = (pStu)target;
	//长度不相同
	if (strlen(stu1->id) != strlen(stu2->id))
	{
		return (strlen(stu1->id) - strlen(stu2->id));
	}
	for (char*p1 = stu1->id, *p2 = stu2->id; *p1 && *p2; p1++, p2++)
	{
		if (*p1 != *p2)
		{
			return ((*p1) - (*p2));
		}
	}
	return 0;
}


int dump_do_callback(pNode listhead, pNode it)
{
	pStu pstu = (pStu)it;
	int total = pstu->math + pstu->english + pstu->c_language;
	printf("%-10s%-10s%-8d%-8d%-8d%-8d\n", pstu->id, pstu->name, pstu->math, pstu->english, pstu->c_language, total);
	//dump info
	return 1;
}


int edit_do_callback(pNode listhead, pNode it)
{

	pStu pstu = (pStu)it;
	//先把旧的信息打印一下
	printf("%-10s%-10s%-8s%-8s%-8s\n", "学号", "姓名", "数学", "英语", "C语言");
	printf("%-10s%-10s%-8d%-8d%-8d\n", pstu->id, pstu->name, pstu->math, pstu->english, pstu->c_language);
	puts("请输入新的信息");

	printf("姓名:");
	scanf("%s",pstu->name);
	printf("数学:");
	scanf("%d", &pstu->math);

	printf("英语:");
	scanf("%d", &pstu->english);

	printf("C语言:");
	scanf("%d", &pstu->c_language);
	return 1;
}


int del_do_callback(pNode listhead, pNode it)
{
	pStu pstu = (pStu)it;
	printf("%-10s%-10s%-8s%-8s%-8s\n", "学号", "姓名", "数学", "英语", "C语言");
	printf("%-10s%-10s%-8d%-8d%-8d\n", pstu->id, pstu->name, pstu->math, pstu->english, pstu->c_language);
	deletenode(listhead, it);
	return 1;
}



//每一个节点都是目标节点,都会调用callback函数
int search_travel_all(void*it, void*target)
{
	return 0;
}
void manager_menu()
{
    system("cls");

    puts("[#]1.查询学生成绩");
    puts("[#]2.打印学生成绩");
    puts("[#]3.排序学生成绩");

    puts("[#]4.添加学生成绩");
    puts("[#]5.删除学生成绩");
    puts("[#]6.编辑学生成绩");
    puts("[#]7.退出程序"); //必须从这里退出，不能直接点关闭，否则信息会丢失，不会保存到文件中
}

//----------------------------------------添加学生信息------------------------------------
void add_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };

		system("cls");
		printf("学号:");
		scanf("%s",temp.id);
		printf("姓名:");
		scanf("%s",temp.name);
		printf("数学:");
		scanf("%d", &temp.math);

		printf("英语:");
		scanf("%d", &temp.english);

		printf("C语言:");
		scanf("%d", &temp.c_language);
		//搜索学号是否已经存在
		if (search(listhead, search_cmp_id, &temp, 0))
		{
			puts("错误,学号重复");
		}
		else
		{
			pStu newstu = (pStu)malloc(sizeof(Stu));
			memcpy(newstu, &temp, sizeof(Stu));
			insertback(listhead, (pNode)newstu);
			puts("添加成功!");
		}
		puts("继续添加？(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//返回主菜单
	}
}
//------------------------------编辑学生信息(只能使用id)------------------------------------

void edit_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };
		system("cls");
		puts("请输入要编辑的学号:");
		scanf("%s",temp.id);

		if (search(listhead, search_cmp_id, &temp, edit_do_callback))
		{
			puts("编辑成功!");
		}
		else
		{
			puts("错误,没有找到该学生");
		}
		puts("继续编辑？(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//返回主菜单
	}
}
//-----------------------------删除信息(只能用学号删除,名字可能会有重复,但是学号不会)--------------------


void del_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };
		system("cls");
		puts("请输入要删除的学号:");
		scanf("%s", temp.id);

		if (search(listhead, search_cmp_id, &temp, del_do_callback))
		{
			puts("删除成功");
		}
		else
		{
			puts("错误,没有找到该学生");
		}
		puts("继续删除？(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//返回主菜单
	}
}


//----------------------------------------dump stu----------------------
void dump_stu(pNode listhead)
{
	printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "学号", "姓名", "数学", "英语", "C语言", "总分");
	search(listhead, search_travel_all, 0, dump_do_callback);
	system("pause");
}

//-----------------------------------------查找----------------------------------


//
void search_stu(pNode listhead)
{
	Stu temp = { 0 };
	int ret = 0;
    while (1)
    {
        system("cls");
        puts("请选择查询方式:");

        puts("[#]1.按姓名查询");
        puts("[#]2.按学号查询");
		//puts("[#]3.按成绩查询");
        puts("[#]3.返回主菜单");

        int choice = getch();

		switch (choice)
		{
		case '1':
			memset(&temp,0,sizeof(temp));
			puts("请输入学生姓名:");
			scanf("%s", temp.name);
			//开始查找
			printf("%-10s%-10s%-8s%-8s%-8s%-8s\n","学号","姓名","数学","英语","C语言","总分");
			ret = search(listhead, search_cmp_name,&temp ,dump_do_callback);
			printf("查找结束.共找到%d条记录.", ret);
			system("pause");
			break;
		case '2':
			memset(&temp, 0, sizeof(temp));
			puts("请输入学生学号:");
			scanf("%s", temp.id);
			//开始查找
			printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "学号", "姓名", "数学", "英语", "C语言", "总分");
			ret = search(listhead, search_cmp_id, &temp, dump_do_callback);
			printf("查找结束.共找到%d条记录.", ret);
			system("pause");
			break;
		case '3':
			return;
			break;
		}
    }
}


//-------------------------------打印学生信息-------------------------



//-------------------------------排序学生信息--------------------------
//单科排序
//总成绩排序



void sort_stu(pNode listhead)
{
	while (1)
	{
		system("cls");
		puts("[#]1.按总成绩排序");
		puts("[#]2.按英语成绩排序");
		puts("[#]3.按数学成绩排序");
		puts("[#]4.按C语言成绩排序");
		puts("[#]5.按学号排序");
		puts("[#]6.返回主菜单");

		int choice = getch();

		if (choice == '6') return;
		
		puts("升序?(Y/y)");
		char mode = getch();
		int	sortmode = (mode == 'Y' || mode == 'y') ? 0 : 1;

		printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "学号", "姓名", "数学", "英语", "C语言", "总分");
		switch (choice)
		{
		case '1':
			sort(listhead, sort_cmp_total, sortmode);
			search(listhead, search_travel_all,NULL, dump_do_callback);
			break;
		case '2':
			sort(listhead, sort_cmp_english, sortmode);
			search(listhead, search_travel_all, NULL, dump_do_callback);
			break;
		case'3':
			sort(listhead, sort_cmp_math, sortmode);
			search(listhead, search_travel_all, NULL, dump_do_callback);
			break;
		case '4':
			sort(listhead, sort_cmp_c, sortmode);
			search(listhead, search_travel_all, NULL, dump_do_callback);
			break;
		case '5':
			sort(listhead, sort_cmp_id, sortmode);
			search(listhead, search_travel_all, NULL, dump_do_callback);
			break;
		}
		system("pause");
	}
}


//读取学生成绩信息
int loadstudentinfo(const char *filename, pNode listhead)
{
    FILE *fp = fopen(filename, "rb");
    if (fp)
    {
        Stu temp = {0};
        while (1)
        {
            if (fread(((char *)&temp) + sizeof(Node), 1, sizeof(Stu) - sizeof(Node), fp) <= 0)
                break;

            pStu pnode = (pStu)malloc(sizeof(Stu));

            memcpy(pnode, &temp, sizeof(temp));

            insertback(listhead, (pNode)pnode);
        }
        fclose(fp);
		return 1;
    }
	return 0;
}

//保存学生的成绩信息
int savestudentinfo(const char *filename, pNode listhead)
{
    FILE *fp = fopen(filename, "wb");
    if (fp)
    {
        for (pNode it = listhead->next; it != listhead; it = it->next)
        {
            //不保存节点
            fwrite(((char *)it) + sizeof(Node), 1, sizeof(Stu) - sizeof(Node), fp);
        }

        fclose(fp);
		return 1;
    }
	return 0;
}


/*
puts("[#]1.查询学生成绩");
puts("[#]2.打印学生成绩");
puts("[#]3.排序学生成绩");

puts("[#]4.添加学生成绩");
puts("[#]5.删除学生成绩");
puts("[#]6.编辑学生成绩");
puts("[#]7.退出程序");
*/
//管理系统模块
void manager_module(pNode listhead)
{
    while (1)
    {
		manager_menu();
        int choice = getch();
        switch (choice)
        {
            case '1':
				search_stu(listhead);
				break;
			case '2':
				dump_stu(listhead);
				break;
			case '3':
				sort_stu(listhead);
				break;
			case '4':
				add_stu(listhead);
				break;
			case '5':
				del_stu(listhead);
				break;
			case '6':
				edit_stu(listhead);
				break; 
			case '7':
				return;
				break;
        }
    }
}
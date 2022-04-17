#include "common.h"
#include "list.h"
#include "manager.h"
/*
    ����ǳɼ�����ϵͳ��������
*/

//������ѯ
int search_cmp_name(void *it, void *target)
{
	return strcmp(((pStu)it)->name, ((pStu)target)->name);
}

//id��ѯ
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
	//���Ȳ���ͬ
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
	//�ȰѾɵ���Ϣ��ӡһ��
	printf("%-10s%-10s%-8s%-8s%-8s\n", "ѧ��", "����", "��ѧ", "Ӣ��", "C����");
	printf("%-10s%-10s%-8d%-8d%-8d\n", pstu->id, pstu->name, pstu->math, pstu->english, pstu->c_language);
	puts("�������µ���Ϣ");

	printf("����:");
	scanf("%s",pstu->name);
	printf("��ѧ:");
	scanf("%d", &pstu->math);

	printf("Ӣ��:");
	scanf("%d", &pstu->english);

	printf("C����:");
	scanf("%d", &pstu->c_language);
	return 1;
}


int del_do_callback(pNode listhead, pNode it)
{
	pStu pstu = (pStu)it;
	printf("%-10s%-10s%-8s%-8s%-8s\n", "ѧ��", "����", "��ѧ", "Ӣ��", "C����");
	printf("%-10s%-10s%-8d%-8d%-8d\n", pstu->id, pstu->name, pstu->math, pstu->english, pstu->c_language);
	deletenode(listhead, it);
	return 1;
}



//ÿһ���ڵ㶼��Ŀ��ڵ�,�������callback����
int search_travel_all(void*it, void*target)
{
	return 0;
}
void manager_menu()
{
    system("cls");

    puts("[#]1.��ѯѧ���ɼ�");
    puts("[#]2.��ӡѧ���ɼ�");
    puts("[#]3.����ѧ���ɼ�");

    puts("[#]4.���ѧ���ɼ�");
    puts("[#]5.ɾ��ѧ���ɼ�");
    puts("[#]6.�༭ѧ���ɼ�");
    puts("[#]7.�˳�����"); //����������˳�������ֱ�ӵ�رգ�������Ϣ�ᶪʧ�����ᱣ�浽�ļ���
}

//----------------------------------------���ѧ����Ϣ------------------------------------
void add_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };

		system("cls");
		printf("ѧ��:");
		scanf("%s",temp.id);
		printf("����:");
		scanf("%s",temp.name);
		printf("��ѧ:");
		scanf("%d", &temp.math);

		printf("Ӣ��:");
		scanf("%d", &temp.english);

		printf("C����:");
		scanf("%d", &temp.c_language);
		//����ѧ���Ƿ��Ѿ�����
		if (search(listhead, search_cmp_id, &temp, 0))
		{
			puts("����,ѧ���ظ�");
		}
		else
		{
			pStu newstu = (pStu)malloc(sizeof(Stu));
			memcpy(newstu, &temp, sizeof(Stu));
			insertback(listhead, (pNode)newstu);
			puts("��ӳɹ�!");
		}
		puts("������ӣ�(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//�������˵�
	}
}
//------------------------------�༭ѧ����Ϣ(ֻ��ʹ��id)------------------------------------

void edit_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };
		system("cls");
		puts("������Ҫ�༭��ѧ��:");
		scanf("%s",temp.id);

		if (search(listhead, search_cmp_id, &temp, edit_do_callback))
		{
			puts("�༭�ɹ�!");
		}
		else
		{
			puts("����,û���ҵ���ѧ��");
		}
		puts("�����༭��(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//�������˵�
	}
}
//-----------------------------ɾ����Ϣ(ֻ����ѧ��ɾ��,���ֿ��ܻ����ظ�,����ѧ�Ų���)--------------------


void del_stu(pNode listhead)
{
	while (1)
	{
		Stu temp = { 0 };
		system("cls");
		puts("������Ҫɾ����ѧ��:");
		scanf("%s", temp.id);

		if (search(listhead, search_cmp_id, &temp, del_do_callback))
		{
			puts("ɾ���ɹ�");
		}
		else
		{
			puts("����,û���ҵ���ѧ��");
		}
		puts("����ɾ����(Y/y)");
		int choice = getch();

		if (choice != 'Y' && choice != 'y')
			break;//�������˵�
	}
}


//----------------------------------------dump stu----------------------
void dump_stu(pNode listhead)
{
	printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "ѧ��", "����", "��ѧ", "Ӣ��", "C����", "�ܷ�");
	search(listhead, search_travel_all, 0, dump_do_callback);
	system("pause");
}

//-----------------------------------------����----------------------------------


//
void search_stu(pNode listhead)
{
	Stu temp = { 0 };
	int ret = 0;
    while (1)
    {
        system("cls");
        puts("��ѡ���ѯ��ʽ:");

        puts("[#]1.��������ѯ");
        puts("[#]2.��ѧ�Ų�ѯ");
		//puts("[#]3.���ɼ���ѯ");
        puts("[#]3.�������˵�");

        int choice = getch();

		switch (choice)
		{
		case '1':
			memset(&temp,0,sizeof(temp));
			puts("������ѧ������:");
			scanf("%s", temp.name);
			//��ʼ����
			printf("%-10s%-10s%-8s%-8s%-8s%-8s\n","ѧ��","����","��ѧ","Ӣ��","C����","�ܷ�");
			ret = search(listhead, search_cmp_name,&temp ,dump_do_callback);
			printf("���ҽ���.���ҵ�%d����¼.", ret);
			system("pause");
			break;
		case '2':
			memset(&temp, 0, sizeof(temp));
			puts("������ѧ��ѧ��:");
			scanf("%s", temp.id);
			//��ʼ����
			printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "ѧ��", "����", "��ѧ", "Ӣ��", "C����", "�ܷ�");
			ret = search(listhead, search_cmp_id, &temp, dump_do_callback);
			printf("���ҽ���.���ҵ�%d����¼.", ret);
			system("pause");
			break;
		case '3':
			return;
			break;
		}
    }
}


//-------------------------------��ӡѧ����Ϣ-------------------------



//-------------------------------����ѧ����Ϣ--------------------------
//��������
//�ܳɼ�����



void sort_stu(pNode listhead)
{
	while (1)
	{
		system("cls");
		puts("[#]1.���ܳɼ�����");
		puts("[#]2.��Ӣ��ɼ�����");
		puts("[#]3.����ѧ�ɼ�����");
		puts("[#]4.��C���Գɼ�����");
		puts("[#]5.��ѧ������");
		puts("[#]6.�������˵�");

		int choice = getch();

		if (choice == '6') return;
		
		puts("����?(Y/y)");
		char mode = getch();
		int	sortmode = (mode == 'Y' || mode == 'y') ? 0 : 1;

		printf("%-10s%-10s%-8s%-8s%-8s%-8s\n", "ѧ��", "����", "��ѧ", "Ӣ��", "C����", "�ܷ�");
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


//��ȡѧ���ɼ���Ϣ
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

//����ѧ���ĳɼ���Ϣ
int savestudentinfo(const char *filename, pNode listhead)
{
    FILE *fp = fopen(filename, "wb");
    if (fp)
    {
        for (pNode it = listhead->next; it != listhead; it = it->next)
        {
            //������ڵ�
            fwrite(((char *)it) + sizeof(Node), 1, sizeof(Stu) - sizeof(Node), fp);
        }

        fclose(fp);
		return 1;
    }
	return 0;
}


/*
puts("[#]1.��ѯѧ���ɼ�");
puts("[#]2.��ӡѧ���ɼ�");
puts("[#]3.����ѧ���ɼ�");

puts("[#]4.���ѧ���ɼ�");
puts("[#]5.ɾ��ѧ���ɼ�");
puts("[#]6.�༭ѧ���ɼ�");
puts("[#]7.�˳�����");
*/
//����ϵͳģ��
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
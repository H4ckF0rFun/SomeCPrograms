#include "common.h"
#include "list.h"
#include "login.h"

void input(char *username,char*password)
{
    //��ȡ�û�����
    int i = 0;
	printf("User:");
	scanf("%s", username);
	printf("Password:");

	int len = 0;
	int ch;

	while ((ch = getch()) != '\r')
	{
		//�����˸��
		if (ch == '\b' && len)
		{
			--len;
			putchar('\b');
			putchar(' ');
			putchar('\b');

		}

		if (ch != '\b' && len<19)
		{
			password[len] = ch;
			++len;
			putchar('*');
		}
	}
	password[len] = '\0';
}

//��ȡ�û���Ϣ
int readuserinfo(const char*filename,pNode listhead)
{
    FILE*fp = fopen(filename,"rb");
    if(fp)
    {
        UserInfo temp = {0};
        while(1)
        {
            if(fread(((char*)&temp)+sizeof(Node) ,1,sizeof(UserInfo) - sizeof(Node),fp)<=0)
                break;

            pUserInfo pNewUser = (pUserInfo)malloc(sizeof(UserInfo));
            memcpy(pNewUser,&temp,sizeof(temp));
                 //��������
            insertback(listhead,(pNode)pNewUser);
        }
        fclose(fp);
		return 1;
    }
	return 0;
}

//�����û���Ϣ
int saveuserinfo(const char* filename,pNode listhead)
{
    FILE*fp = fopen(filename,"wb");
    if(fp)
    {
        for(pNode it =listhead->next;it!=listhead;it = it->next)
        {
            //дһ���ڵ㵽�ļ���
            fwrite(((char*)it)+sizeof(Node),1,sizeof(UserInfo) - sizeof(Node),fp);
        }
        fclose(fp);
		return 1;
    }
	return 0;
}

void menu()
{
    system("cls");
    puts("[#]1.��¼");
    puts("[#]2.ע��");
    puts("[#]3.ɾ���û�");
    puts("[#]4.�˳�");

}

//��¼
int login(pNode listhead,char*curusername)
{
	system("cls");
    //��ȡ�û�����
	char username[20] = { 0 }, password[20] = { 0 };
    input(username,password);

    //��������,
    pNode it;
    for(it = listhead->next;it != listhead;it = it->next)
    {
        if(!strcmp(((pUserInfo)it)->username,username) && !strcmp(((pUserInfo)it)->password,password))
        {
            break;
        }
    }
    if(it!=listhead)    //��¼�ɹ�
    {
        strcpy(curusername,username);
        puts("\n��¼�ɹ�!");
		system("pause");
        return 1;
    }
    else
    {
        strcpy(curusername,"");
        puts("\n�û������������!");
		system("pause");
        return 0;
    }
}

//ע���û�
int registeruser(pNode listhead)
{
	system("cls");
	char username[20] = { 0 }, password[20] = { 0 };
	input(username,password);

    for(pNode it = listhead->next;it != listhead;it = it->next)
    {
        //����û��Ѿ���ע����
        if(!strcmp(((pUserInfo)it)->username,username))
        {
            puts("\n���û��ѱ�ע��!");
			system("pause");
			return 0;
        }
    }
    
    pUserInfo pnode = (pUserInfo)malloc(sizeof(UserInfo));

    strcpy(pnode->password,password);
    strcpy(pnode->username,username);

    insertback(listhead,(pNode)pnode);

    puts("\nע��ɹ�!");
	system("pause");
    return 1;
}

//ɾ���������Ѿ����ڵ��û�
int deluser(pNode listhead)
{
	system("cls");
	char username[20] = { 0 }, password[20] = { 0 };
    input(username,password);

    for(pNode it = listhead->next;it != listhead;it = it->next)
    {
        //�ҵ���
        if(!strcmp(((pUserInfo)it)->username,username) && !strcmp(((pUserInfo)it)->password,password))
        {
            deletenode(listhead,it);
            puts("\nɾ���ɹ�!");
			system("pause");
            return 1;
        }
    }
    puts("\n�û������������!");
	system("pause");
    return 0;
}

//��¼ģ��
int loginmodule(const char* username,pNode listhead)
{
    while(1)
    {
        menu();
        int choice = getch();

        switch(choice)
        {
            case '1':
                if(login(listhead,username))
                    return 1;
                break;
            case '2':
                registeruser(listhead);
                break;
            case '3':
                deluser(listhead);
                break;
            case '4':
                return 0;
                break;
        }
    }
}
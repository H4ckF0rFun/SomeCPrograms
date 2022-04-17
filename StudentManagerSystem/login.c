#include "common.h"
#include "list.h"
#include "login.h"

void input(char *username,char*password)
{
    //读取用户输入
    int i = 0;
	printf("User:");
	scanf("%s", username);
	printf("Password:");

	int len = 0;
	int ch;

	while ((ch = getch()) != '\r')
	{
		//输入退格键
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

//读取用户信息
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
                 //加入链表
            insertback(listhead,(pNode)pNewUser);
        }
        fclose(fp);
		return 1;
    }
	return 0;
}

//保存用户信息
int saveuserinfo(const char* filename,pNode listhead)
{
    FILE*fp = fopen(filename,"wb");
    if(fp)
    {
        for(pNode it =listhead->next;it!=listhead;it = it->next)
        {
            //写一个节点到文件中
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
    puts("[#]1.登录");
    puts("[#]2.注册");
    puts("[#]3.删除用户");
    puts("[#]4.退出");

}

//登录
int login(pNode listhead,char*curusername)
{
	system("cls");
    //读取用户输入
	char username[20] = { 0 }, password[20] = { 0 };
    input(username,password);

    //遍历链表,
    pNode it;
    for(it = listhead->next;it != listhead;it = it->next)
    {
        if(!strcmp(((pUserInfo)it)->username,username) && !strcmp(((pUserInfo)it)->password,password))
        {
            break;
        }
    }
    if(it!=listhead)    //登录成功
    {
        strcpy(curusername,username);
        puts("\n登录成功!");
		system("pause");
        return 1;
    }
    else
    {
        strcpy(curusername,"");
        puts("\n用户名或密码错误!");
		system("pause");
        return 0;
    }
}

//注册用户
int registeruser(pNode listhead)
{
	system("cls");
	char username[20] = { 0 }, password[20] = { 0 };
	input(username,password);

    for(pNode it = listhead->next;it != listhead;it = it->next)
    {
        //这个用户已经被注册了
        if(!strcmp(((pUserInfo)it)->username,username))
        {
            puts("\n该用户已被注册!");
			system("pause");
			return 0;
        }
    }
    
    pUserInfo pnode = (pUserInfo)malloc(sizeof(UserInfo));

    strcpy(pnode->password,password);
    strcpy(pnode->username,username);

    insertback(listhead,(pNode)pnode);

    puts("\n注册成功!");
	system("pause");
    return 1;
}

//删除链表中已经存在的用户
int deluser(pNode listhead)
{
	system("cls");
	char username[20] = { 0 }, password[20] = { 0 };
    input(username,password);

    for(pNode it = listhead->next;it != listhead;it = it->next)
    {
        //找到了
        if(!strcmp(((pUserInfo)it)->username,username) && !strcmp(((pUserInfo)it)->password,password))
        {
            deletenode(listhead,it);
            puts("\n删除成功!");
			system("pause");
            return 1;
        }
    }
    puts("\n用户名或密码错误!");
	system("pause");
    return 0;
}

//登录模块
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
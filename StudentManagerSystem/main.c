
#include "common.h"
#include "login.h"
#include "list.h"
#include "manager.h"

int main()
{
    //用于保存用户信息的链表
    char currentuser[20] = {0};
	char stufile[256] = { 0 };

    Node UserInfoHead = {0};
    UserInfoHead.next = &UserInfoHead;
    UserInfoHead.prev = &UserInfoHead;

	Node StuHead = { 0 };
	StuHead.next = &StuHead;
	StuHead.prev = &StuHead;


	if (0 == readuserinfo("userdata", (pNode)&UserInfoHead))
	{
		puts("Couldn't open file:userdata");
		goto exitsystem;
	}
    
    //先执行登录模块
    if(0 == loginmodule(currentuser,&UserInfoHead))
    {
        //退出程序
        goto exitsystem;
    }
	
	sprintf(stufile, "%s.dat", currentuser);

	if (0 == loadstudentinfo(stufile, &StuHead))
	{
		printf("Couldn't open file:%s", stufile);
		goto exitsystem;
	}

	manager_module(&StuHead);

exitsystem:
	savestudentinfo(stufile, (pNode)&StuHead);
    saveuserinfo("userdata",(pNode)&UserInfoHead);
    
	dellist(&UserInfoHead);
	dellist(&StuHead);
    return 0;
}
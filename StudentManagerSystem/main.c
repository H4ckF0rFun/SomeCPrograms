
#include "common.h"
#include "login.h"
#include "list.h"
#include "manager.h"

int main()
{
    //���ڱ����û���Ϣ������
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
    
    //��ִ�е�¼ģ��
    if(0 == loginmodule(currentuser,&UserInfoHead))
    {
        //�˳�����
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
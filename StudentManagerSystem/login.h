#ifndef LOGIN_H

#define LOGIN_H

#include "common.h"
#include "list.h"

//���ڴ����û���Ϣ
typedef struct _UserInfo
{
    Node linknode;
    char username[32];
    char password[32];
}UserInfo,*pUserInfo;


//�����û���Ϣ
int saveuserinfo(const char* filename,pNode listhead);

//��ȡ�û���Ϣ
int readuserinfo(const char*filename,pNode listhead);


//��¼ģ��
int loginmodule(const char* username,pNode listhead);

//ɾ���������Ѿ����ڵ��û�
int deluser(pNode listhead);

//ע���û�
int registeruser(pNode listhead);

//��¼
int login(pNode listhead,char*curusername);

#endif
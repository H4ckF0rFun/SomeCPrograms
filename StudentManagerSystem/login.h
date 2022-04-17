#ifndef LOGIN_H

#define LOGIN_H

#include "common.h"
#include "list.h"

//用于储存用户信息
typedef struct _UserInfo
{
    Node linknode;
    char username[32];
    char password[32];
}UserInfo,*pUserInfo;


//保存用户信息
int saveuserinfo(const char* filename,pNode listhead);

//读取用户信息
int readuserinfo(const char*filename,pNode listhead);


//登录模块
int loginmodule(const char* username,pNode listhead);

//删除链表中已经存在的用户
int deluser(pNode listhead);

//注册用户
int registeruser(pNode listhead);

//登录
int login(pNode listhead,char*curusername);

#endif
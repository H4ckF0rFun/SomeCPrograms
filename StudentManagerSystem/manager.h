#ifndef MANAGER_H
#define MANAGER_H

#include "list.h"

typedef struct _Stu
{
    Node linknode;
    char name[16];  //姓名       
    char id[16];    //学号
  
    int math;       //数学
    int english;    //英语
    int c_language; //C语言
    
}Stu,*pStu;

//读取学生成绩信息
int loadstudentinfo(const char *filename, pNode listhead);
//保存学生的成绩信息
int savestudentinfo(const char *filename, pNode listhead);
//manager 模块
void manager_module(pNode listhead);

#endif
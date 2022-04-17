#ifndef MANAGER_H
#define MANAGER_H

#include "list.h"

typedef struct _Stu
{
    Node linknode;
    char name[16];  //����       
    char id[16];    //ѧ��
  
    int math;       //��ѧ
    int english;    //Ӣ��
    int c_language; //C����
    
}Stu,*pStu;

//��ȡѧ���ɼ���Ϣ
int loadstudentinfo(const char *filename, pNode listhead);
//����ѧ���ĳɼ���Ϣ
int savestudentinfo(const char *filename, pNode listhead);
//manager ģ��
void manager_module(pNode listhead);

#endif
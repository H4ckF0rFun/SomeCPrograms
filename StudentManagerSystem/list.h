#ifndef LIST_H
#define LIST_H
//�����ܰ�������Ǿ���������������

typedef struct _Node
{
    struct _Node* next;
    struct _Node* prev;
}Node,*pNode;



//�ص�����
typedef int(*docallback)(pNode listhead,pNode it);


//�ȽϺ���Ӧ���Լ�����,��������,0,����,�������,����,С��
typedef int(*cmpfunc)(void* it, void* target);

//ͷ�巨
void insertfront(pNode listhead/*����*/,pNode newnode/*Ҫ����Ľڵ�*/);

//β�巨
void insertback(pNode listhead/*����*/,pNode newnode/*Ҫ����Ľڵ�*/);

//ɾ���ڵ�
void deletenode(pNode listhead/*�ĸ�����*/,pNode delnode/*Ҫɾ���Ľڵ�*/);

//ɾ������
void dellist(pNode listhead);

//�����ڵ�
int search(pNode listhead, cmpfunc compare, void* target, docallback callback);

//�����ڵ�
void swapnode(pNode* pp1,pNode* pp2);

//��������
void sort(pNode listhead,cmpfunc compare,int mode/*0����,��0����*/);


#endif
#ifndef LIST_H
#define LIST_H
//链表不能包含万物，那就让万物包含链表吧

typedef struct _Node
{
    struct _Node* next;
    struct _Node* prev;
}Node,*pNode;



//回调函数
typedef int(*docallback)(pNode listhead,pNode it);


//比较函数应该自己定义,返回正数,0,负数,代表大于,等于,小于
typedef int(*cmpfunc)(void* it, void* target);

//头插法
void insertfront(pNode listhead/*链表*/,pNode newnode/*要插入的节点*/);

//尾插法
void insertback(pNode listhead/*链表*/,pNode newnode/*要插入的节点*/);

//删除节点
void deletenode(pNode listhead/*哪个链表*/,pNode delnode/*要删除的节点*/);

//删除链表
void dellist(pNode listhead);

//搜索节点
int search(pNode listhead, cmpfunc compare, void* target, docallback callback);

//交换节点
void swapnode(pNode* pp1,pNode* pp2);

//排序链表
void sort(pNode listhead,cmpfunc compare,int mode/*0升序,非0降序*/);


#endif
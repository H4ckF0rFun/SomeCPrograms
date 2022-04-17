#include "common.h"
#include "list.h"
//头插法
void insertfront(pNode listhead/*链表*/,pNode newnode/*要插入的节点*/)
{
    newnode->next = listhead->next;
    newnode->prev = listhead;

    newnode->next->prev = newnode;
    newnode->prev->next = newnode;
}

//尾插法
void insertback(pNode listhead/*链表*/,pNode newnode/*要插入的节点*/)
{
    newnode->next = listhead;
    newnode->prev = listhead->prev;

    newnode->prev->next = newnode;
    newnode->next->prev = newnode;
}

//删除节点
void deletenode(pNode listhead/*哪个链表*/,pNode delnode/*要删除的节点*/)
{
	if (delnode == listhead)
        return;             //不应该删除头节点，至少留下头节点.
    
    //双向链表删除很方便
    delnode->prev->next = delnode->next;
    delnode->next->prev = delnode->prev;

    free(delnode);
}

//删除链表
void dellist(pNode listhead)
{
    for(pNode it = listhead->next;it!=listhead/*当下一个节点是自身的时候，说明环形链表只剩下一个头了*/;)
    {
        pNode t = it;
        it = it->next;
        free(t);
    }
}


//搜索节点
int search(pNode listhead,cmpfunc compare,void* target,docallback callback)
{
	int ncount = 0;
    for(pNode it = listhead->next;it!=listhead;)
    {
		if (0 == compare(it, target))          //找到了
		{
			pNode t = it;
			it = it->next;
			ncount++;
			if (callback)
				callback(listhead,t);
		}
		else
		{
			it = it->next;
		}
    }
    return ncount;
}

//交换节点
void swapnode(pNode* pp1,pNode* pp2)
{
	pNode p2_prev, p1_next;
	p2_prev = (*pp2)->prev;
	p1_next = (*pp1)->next;

	(*pp2)->prev = (*pp1)->prev;
	(*pp1)->next = (*pp2)->next;

	//两个相邻的节点
	if (p1_next == (*pp2) && p2_prev == (*pp1))
	{	
		(*pp2)->next = (*pp1);
		(*pp1)->prev = (*pp2);
	}
	else
	{
		(*pp2)->next = p1_next;
		(*pp1)->prev = p2_prev;
		(*pp2)->next->prev = (*pp2);
		(*pp1)->prev->next = (*pp1);
	}

	(*pp2)->prev->next = (*pp2);
	(*pp1)->next->prev = (*pp1);

    pNode temp1 = *pp1;
    *pp1 = *pp2;
    *pp2 = temp1;
}
//排序链表
void sort(pNode listhead,cmpfunc compare,int mode/*0升序,非0降序*/)
{
    //直接快速排序得了,比较方便(数据量大的时候很慢n^2,不过对付这种玩具程序足够了)
    for(pNode it = listhead->next;it!=listhead;it = it->next)
    {
        for(pNode p = it->next;p!=listhead;p=p->next)
        {
            int ret = compare(it,p);
            if(mode == 0 && ret>0) //升序排列,但是it > p,交换
            {
                swapnode(&it,&p);
            }
            else if(mode ==1 && ret<0)          //降序排列,把大的往前移动
            {
                swapnode(&it,&p);
            }
        }
    }
}
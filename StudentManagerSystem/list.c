#include "common.h"
#include "list.h"
//ͷ�巨
void insertfront(pNode listhead/*����*/,pNode newnode/*Ҫ����Ľڵ�*/)
{
    newnode->next = listhead->next;
    newnode->prev = listhead;

    newnode->next->prev = newnode;
    newnode->prev->next = newnode;
}

//β�巨
void insertback(pNode listhead/*����*/,pNode newnode/*Ҫ����Ľڵ�*/)
{
    newnode->next = listhead;
    newnode->prev = listhead->prev;

    newnode->prev->next = newnode;
    newnode->next->prev = newnode;
}

//ɾ���ڵ�
void deletenode(pNode listhead/*�ĸ�����*/,pNode delnode/*Ҫɾ���Ľڵ�*/)
{
	if (delnode == listhead)
        return;             //��Ӧ��ɾ��ͷ�ڵ㣬��������ͷ�ڵ�.
    
    //˫������ɾ���ܷ���
    delnode->prev->next = delnode->next;
    delnode->next->prev = delnode->prev;

    free(delnode);
}

//ɾ������
void dellist(pNode listhead)
{
    for(pNode it = listhead->next;it!=listhead/*����һ���ڵ��������ʱ��˵����������ֻʣ��һ��ͷ��*/;)
    {
        pNode t = it;
        it = it->next;
        free(t);
    }
}


//�����ڵ�
int search(pNode listhead,cmpfunc compare,void* target,docallback callback)
{
	int ncount = 0;
    for(pNode it = listhead->next;it!=listhead;)
    {
		if (0 == compare(it, target))          //�ҵ���
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

//�����ڵ�
void swapnode(pNode* pp1,pNode* pp2)
{
	pNode p2_prev, p1_next;
	p2_prev = (*pp2)->prev;
	p1_next = (*pp1)->next;

	(*pp2)->prev = (*pp1)->prev;
	(*pp1)->next = (*pp2)->next;

	//�������ڵĽڵ�
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
//��������
void sort(pNode listhead,cmpfunc compare,int mode/*0����,��0����*/)
{
    //ֱ�ӿ����������,�ȽϷ���(���������ʱ�����n^2,�����Ը�������߳����㹻��)
    for(pNode it = listhead->next;it!=listhead;it = it->next)
    {
        for(pNode p = it->next;p!=listhead;p=p->next)
        {
            int ret = compare(it,p);
            if(mode == 0 && ret>0) //��������,����it > p,����
            {
                swapnode(&it,&p);
            }
            else if(mode ==1 && ret<0)          //��������,�Ѵ����ǰ�ƶ�
            {
                swapnode(&it,&p);
            }
        }
    }
}
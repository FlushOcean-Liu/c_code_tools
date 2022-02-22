#ifndef _DOUBLE_LIST_H_
#define _DOUBLE_LIST_H_


typedef struct _node {
	void *data;
	struct _node *prior;
	struct _node *next;
}NODE;


typedef struct DoubleLinkList {
	NODE *head;
	NODE *last;
	int length;
}DLList;



DLList *CreateList();												                // 创建双链表
int append_list(DLList *l, void *data, int size);					  // 在链表后追加结点
int insert_list(DLList *l, int pos, void *data, int size);   // 在 pos 位置的元素前插入
int delete_node(DLList *l, int pos, void *e, int size);      // 删除位置为pos的结点，并用e返回被删除的结点数据
void print_list(DLList *l, void (*printnode)(void *));				// 打印
void clear_list(DLList *l);                                  // 清空链表
void destory_list(DLList **l);                               // 销毁链表
void print_data(void *data);



#endif
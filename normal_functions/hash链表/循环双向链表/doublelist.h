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



DLList *CreateList();												                // ����˫����
int append_list(DLList *l, void *data, int size);					  // �������׷�ӽ��
int insert_list(DLList *l, int pos, void *data, int size);   // �� pos λ�õ�Ԫ��ǰ����
int delete_node(DLList *l, int pos, void *e, int size);      // ɾ��λ��Ϊpos�Ľ�㣬����e���ر�ɾ���Ľ������
void print_list(DLList *l, void (*printnode)(void *));				// ��ӡ
void clear_list(DLList *l);                                  // �������
void destory_list(DLList **l);                               // ��������
void print_data(void *data);



#endif
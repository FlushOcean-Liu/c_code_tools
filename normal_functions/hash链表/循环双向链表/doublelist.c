#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doublelist.h"

double d[5] = {10.23, 34.23, 54.65, 122, 35.5};

// ����˫����
DLList *create_list()
{
	DLList *l = (DLList *)malloc(sizeof(DLList));
	if (NULL == l)
		exit(0);
	
	l->head = (NODE *)malloc(sizeof(NODE));					// �����ڱ���㣬�ɷ���˫����Ĳ���
	if (NULL == l->head)
		exit(0);
	memset(l->head, 0, sizeof(NODE));

	l->last = (NODE *)malloc(sizeof(NODE));
	if (NULL == l->last)
		exit(0);
	memset(l->last, 0, sizeof(NODE));

	l->head->next = l->last;
	l->last->prior = l->head;
	l->length = 0;
	return l;
}


// �������׷�ӽ��
int append_list(DLList *l, void *data, int size)
{
	NODE *p = NULL;

	if (NULL == l || NULL == data)
		return 0;

	p = (NODE *)malloc(sizeof(NODE));
	if (NULL == p)
		return 0;

	p->data = malloc(size);
	if (NULL == p->data){
		free(p);
		return 0;
	}
	memcpy(p->data, data, size);
	
	p->next = l->last;
	p->prior = l->last->prior;
	l->last->prior->next = p;
	l->last->prior = p;
	l->length++;
	return 1;
}


// �� pos λ�õ�Ԫ��ǰ����
int insert_list(DLList *l, int pos, void *data, int size)
{
  NODE *p = NULL, *q = NULL;
  int i = 0;

  if (NULL == l || NULL == data || pos < 1 || pos > l->length)
    return 0;

  p = (NODE *)malloc(sizeof(NODE));
  if (NULL == p)
    return 0;

  p->data = malloc(size);
  if (NULL == p->data){
    free(p);
    return 0;
  }
  memcpy(p->data, data, size);

  q = l->head;
  while (i < pos - 1){
    q = q->next;
    ++i;
  }
  p->next = q->next;
  q->next = p;
  p->prior = q;
  p->next->prior = p;
  l->length++;
  return 1;
}


// ɾ��λ��Ϊpos�Ľ�㣬����e���ر�ɾ���Ľ������
int delete_node(DLList *l, int pos, void *e, int size)
{
  NODE *p = NULL, *q = NULL;
  int i = 0;

  if (NULL == l || pos < 1 || pos > l->length)
    return 0;
  p = l->head;
  while (i < pos - 1){            // �����������posλ�õ�ǰһ����㣬������ֱ�ӱ�����posλ�õĽ���ϣ�ͨ��prior����ɾ��
    p = p->next;
  }
  q = p->next;
  memcpy(e, q->data, size);
  
  p->next = q->next;
  q->next->prior = p;
  free(q->data);
  free(q);
  l->length--;
  return 1;
}


// ��ӡ
void print_list(DLList *l, void (*printnode)(void *))
{
	NODE *p = NULL;
	int i = 0;

	if (NULL == l || NULL == printnode)
		return;

	p = l->head;
	while (i < l->length){
		p = p->next;
		printnode(p->data);
		++i;
	}
}


// �������
void clear_list(DLList *l)
{
  NODE *p = NULL;

  if (NULL == l->head->next)
    return;
  p = l->head->next;
  l->head->next = p->next;
  free(p);
  if (l->head->next != l->last)                 // ���p����һ����㲻��β��㣬��ֹɾ��β���
    clear_list(l);                               // �����õݹ�
}


// ��������
void destory_list(DLList **l)
{  
  DLList *p = *l;
  if (NULL == p)
    return;

  clear_list(p);
  free(p->head);            // �ͷ��ڱ����
  free(p->last);
  free(p);
  *l = NULL;
}

void print_data(void *data)
{
  double *d = (double *)data;
  printf("d = %lf\n",*d);
}

int main()
{
  int i;
  double e;
  DLList *list = create_list();
  for (i = 0; i < 4; ++i){
    append_list(list, &d[i], sizeof(d[i]));
  }
  printf("LIST ELEMENT:\n");
  print_list(list,print_data);
  printf("\n\n");
  
  printf("START TO INSERT DATA:\n");
  insert_list(list, 2, &d[4], sizeof(d[4]));
  print_list(list,print_data);
  printf("\n\n");

	printf("START TO DELETE DATA:\n");
  delete_node(list, 1, &e, sizeof(double));
  printf("delete element is: ");
  printf("%lf\n\n",e);

  printf("START TO CLEAR LIST:\n");
  destory_list(&list);
	if (NULL == list)
    printf("list is NULL\n");
  else
    printf("list not NULL\n");
 	return 0;
}
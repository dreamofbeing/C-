#ifndef LIST_H
#define LIST_H
struct list_node{
	void *data;//������ͨ������������void*��ָ������ 
	struct list_node *next, *prev;//�������˫������ 
};
typedef struct list_node ListNode;
typedef struct{
	ListNode *head, *tail;
}List;
List *NewList();
void ListInsert(List *list, void *data, int size);//��list�ڲ���data���ݣ���СΪsize 
#endif

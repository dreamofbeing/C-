#ifndef TREAP_H
#define TREAP_H
#define lson tr[now].ls
#define rson tr[now].rs
#define MAXN 20010
//MAXN��ʾ�������������
//����ƽ������fhq_treapʵ�� 
//����Ӧ��root��tot�ֱ��ʾ���ĸ��ڵ�ͽڵ����� 
struct value{
	char *funcname, *filename;
	int id, line; //��������� 
};//ƽ�����еļ�ֵ 
struct node{
	int size, ls, rs, pri;
	struct value val;
};//ƽ�����ڵ� 
void TreapInit();//��ʼ��һ��treap 
void TreapInsert(struct value val);//����һ���� 
struct value TreapKth(int now, int k);//���ҵ�k���� 
int findrank(struct value k);//�ҵ���ֵk������ 
struct value TreapFind(struct value k);//Ѱ����k������funcname��ͬ�ĺ���
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "treap.h"
int tot, root;
struct value empty;
struct node tr[MAXN << 1];//treap�Ľڵ� 
int mystrcmp(char *s1, char *s2){
	if(s1 == NULL || s2 == NULL){
		if(s1 == s2) return 0;
		return s1 == NULL ? -1 : 1;
	}
	//printf("%s %s %d\n", s1, s2, strcmp(s1, s2));
	return strcmp(s1, s2);
}
static int cmp(struct value v1, struct value v2){//�����ַ�����Сs1 < s2�ıȽϺ���;
	if((mystrcmp(v1.funcname, v2.funcname) == 0) && (mystrcmp(v1.filename, v2.filename) == 0)) return 0;//���������ļ�����ͬ����Ϊ��ȫ��ͬ 
	return (mystrcmp(v1.funcname, v2.funcname) < 0) 
	|| ((mystrcmp(v1.funcname, v2.funcname) == 0) && (mystrcmp(v1.filename, v2.filename) < 0));
}
static void pushup(int now){//���¸��ڵ�ı���ֵ 
	tr[now].size = tr[lson].size + tr[rson].size + 1;
}
static void split(int now, struct value k, int *x, int *y){//����kֵ����,xΪ���ѳ�������������Ԫ��<=k��yΪ���ѳ�������������now�����ڵĽڵ㣬�����������϶��� 
	if(!now){
		*x = *y = 0;
		return;
	}
	if(cmp(k, tr[now].val)){ //���ڽڵ��ֵ�����k�󣬾��������������� 
		*y = now;
		split(lson, k, x, &lson);
	}
	else{ //���ڽڵ������Ȼ��߱�kС������������������ 
		*x = now;
		split(rson, k, &rson, y);
	}
	pushup(now);
}
static int merge(int x, int y){//���غϲ���Ľڵ��� 
	if(!x || !y) return x + y;
	if(tr[x].pri > tr[y].pri){//�������ȼ��ϲ� 
		tr[x].rs = merge(tr[x].rs, y);
		pushup(x);
		return x;
	}
	else{
		tr[y].ls = merge(x, tr[y].ls);
		pushup(y);
		return y;
	}
}
void TreapInsert(struct value val){
	tr[++tot].val = val;
	tr[tot].val.id = tot;
	tr[tot].size = 1;
	tr[tot].pri = rand();
	int x, y;
	split(root, val, &x, &y);
	root = merge(merge(x, tot), y);
}
struct value TreapKth(int now, int k){
	for(; now;){
	    if(tr[lson].size + 1 == k) return tr[now].val;
		else if(k <= tr[lson].size) now = lson;
		else k -= tr[lson].size + 1, now = rson;
	}
	return empty;
}
int findrank(struct value k){
	int x, y;
	split(root, k, &x, &y);
	int sum = tr[x].size;
	root = merge(x, y);
	return sum;//���򽫴�0��ʼ 
}
struct value TreapFind(struct value k){//�ҵ�С�����ĵ�һ������ 
	//printf("%d\n", findrank(k)); 
	//return kth(root, findrank(k));
	int x, y;
	split(root, k, &x, &y);
	int now = x;
	while(rson)
	  now = rson;
	struct value v1 = tr[now].val;
	now = y;
	while(lson)
	  now = lson;
	struct value v2 = tr[now].val;
	root = merge(x, y);
	if(mystrcmp(v1.funcname, k.funcname) == 0) return v1;
	if(mystrcmp(v2.funcname, k.funcname) == 0) return v2;
	return empty;
}
void TreapInit(){
	empty.filename = NULL, empty.funcname = NULL;
	root = tot = 0;
	int i;
	for(i = 0; i < MAXN << 1; i++)
	  tr[i].ls = tr[i].rs = tr[i].pri = tr[i].size = 0;
} 

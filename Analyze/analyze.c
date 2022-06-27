#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "treap.h"
#include "text.h"
#include "list.h"
#include "analyze.h"
#include "PutText.h"
/*#define DEBUG*/
extern int tot, root;
extern GlobalState globalstate, prevstate;
struct value Map[MAXN];//Map��ʾid��struct value֮��Ĺ�ϵ
int topcite[MAXN], topciter[MAXN];
List *call[MAXN], *caller[MAXN];//call������õĺ�����caller����������ĺ���
static int k1, k2, usedtimes;//����FindFirstPreString�����Ķ�λ 
static void InitAllList(){
	int i;
	for(i = 1; i <= tot; i++)
	  call[i] = NewList(), caller[i] = NewList();
}
static void FindFirstPreString(char *file, int start){//�ҵ�startǰ�ĵ�һ���ǿ��ַ��� 
	k2 = start - 1;
	while(~k2 && IsSpaceOrEnter(file[k2])) k2--;//�����м�Ŀո� 
	k1 = k2;
	while(~k1 && IsLetterOrDightOrUnderline(file[k1])) k1--;//��ǰ�ҵ������� 
	k1++;
}
static char* FindNextFunction(char *filename, char *file, int len){
	int i = 0, j, jj;
	i = FindNextChar(0, file, len, '('); //�ҵ���һ�������� 
	if(i == len) return NULL;
	j = FindNextChar(i + 1, file, len, ')'); //�ҵ������ź��������
	jj = j + 1;
	while(jj < len && IsSpaceOrEnter(file[jj])) jj++;//�ҵ���һ���ǿ��ַ� 
	if(jj == len) return NULL;
	if(file[jj] == '{'){//�����)����{����Ϊ����һ������ 
		FindFirstPreString(file, i); 
		if(!IsLegalFunction(k1, k2, file))
		  return file + j;
		struct value val;
		val.filename = calloc(strlen(filename), sizeof(char));
		strcpy(val.filename, filename);
		val.funcname = calloc(k2 - k1 + 2, sizeof(char));
		strncpy(val.funcname, file + k1, k2 - k1 + 1);
		val.funcname[k2 - k1 + 1] = 0; //��ֹcalloc��ʼ������ȫ�����쳣������ 
		val.line = MyGetLine();
		TreapInsert(val);
		val.id = tot;
		Map[tot] = val;
		#ifdef DEBUG 
		printf("%s %s %d\n", val.filename, val.funcname, val.line); 
		#endif
	}
	return file + j;
}
static char *FindNextFunctionUse(char *filename, char *file, int len){
	int i = 0, j, jj, l;
	i = FindNextChar(0, file, len, '('); //�ҵ���һ�������� 
	if(i == len) return NULL;
	j = FindNextChar(i + 1, file, len, ')'); //�ҵ������ź��������
	jj = j + 1;
	while(jj < len && IsSpaceOrEnter(file[jj])) jj++;
	if(jj == len) return NULL;
	if(file[jj] == '{'){//jjָʾ������ŵ�λ��
		jj = j + 1;
		while(jj < len && IsSpaceOrEnter(file[jj])) ChangeTextState(file + jj), jj++;//����һ��״̬
		ChangeTextState(file + jj), jj++; 
		FindFirstPreString(file, i); 
		if(!IsLegalFunction(k1, k2, file))
		  return file + (jj);
		struct value val, preval;
		val.filename = calloc(strlen(filename) + 1, sizeof(char));
		strcpy(val.filename, filename);
		val.filename[strlen(filename)] = 0;//��ֹcalloc��ʼ������ȫ�����쳣������ 
		val.funcname = calloc(k2 - k1 + 2, sizeof(char));
		strncpy(val.funcname, file + k1, k2 - k1 + 1);//��val�洢���ڵĺ��������ļ��� 
		val.funcname[k2 - k1 + 1] = 0; //��ֹcalloc��ʼ������ȫ�����쳣������ 
		preval = TreapFind(val);//��Ҫ��Ϊ�˻�ȡid
//		free(val.filename);
//		free(val.funcname);//�ͷſռ�
		val = preval; 
		ChangeTextState(file + jj);
		jj++;
		Call *newcall = NULL, *newcaller = NULL;
		for(l = jj; GetBracketDiffer(); l++){//Ѱ��ֱ����������ƥ�� 
		   ChangeTextState(file + l);
		   if(!IsLegalState() || file[l] != '(') continue;
		   FindFirstPreString(file, l); 
		   struct value v, treapv;
		   v.filename = calloc(strlen(filename), sizeof(char));
		   strcpy(v.filename, filename);
		   v.funcname = calloc(k2 - k1 + 2, sizeof(char));
		   strncpy(v.funcname, file + k1, k2 - k1 + 1);
		   file[k2 - k1 + 1] = 0;
		   treapv = TreapFind(v);  //ֱ���ҵ�
//		   free(v.filename);
//		   free(v.funcname);//�ͷſռ� 
		   if(treapv.funcname == NULL) continue;//����empty��ʾû���ҵ� 
		   if(call[val.id]->tail != NULL){
				newcall = (Call *)(call[val.id]->tail->data);
		   		if(newcall->id == treapv.id && newcall->line == MyGetLine())//���ͬһ��������ͬ�е��ã��ͺ��� 
		     		continue;
		   }
		   newcall = (Call *)malloc(sizeof(Call));
		   if(newcall == NULL){
		   	  printf("NULL POINTER");
		   	  exit(0);
		   }
		   newcall->id = treapv.id;
		   newcall->line = MyGetLine();
		   ListInsert(call[val.id], (void*)newcall, sizeof(Call));
		   newcaller = (Call *)malloc(sizeof(Call));
		   if(newcaller == NULL){
		   	  printf("NULL POINTER");
		   	  exit(0);
		   }
		   newcaller->id = val.id;
		   newcaller->line = MyGetLine();
		   ListInsert(caller[treapv.id], (void*)newcaller, sizeof(Call));
//		   free(newcall);
//		   free(newcaller);
		}
		return file + l;//����Ǻ����Ļ�����ֱ���������������Ķ���֮�� 
	}
	return file + j;
}
static void FindAllFunction(char *filename, char *file){
	TextInit();
	while(file = 
	FindNextFunction(filename, file, strlen(file)));
}
static void FindAllFunctionUse(char *filename, char *file){
	TextInit();
	while(file = 
	FindNextFunctionUse(filename, file, strlen(file))); 
}
static void Debug(int id){//���Map�б��id�ĺ����ĵ��ù�ϵ�Թ����� 
	printf("%s %s %d\n", Map[id].filename, Map[id].funcname, Map[id].line);
	ListNode *j;
    printf("���õĺ�����\n");
    for(j = call[id]->head; j != NULL; j = j->next){
    	Call *a = (Call *)(j->data);
    	printf("%s %s %d\n", Map[a->id].filename, Map[a->id].funcname, a->line);
    }
    printf("�������ĺ���:\n");
    for(j = caller[id]->head; j != NULL; j = j->next){
    	Call *a = (Call *)(j->data);
    	printf("%s %s %d\n", Map[a->id].filename, Map[a->id].funcname, a->line);
    }
    printf("\n\n");
}
int FindFuncByName(char *funcname){
	struct value v, treapv;
	v.funcname = funcname;
	v.filename = NULL; 
	treapv = TreapFind(v);
	if(treapv.filename == NULL) return -1;
	return treapv.id;
}
int FindFuncByRank(int rank){
	return TreapKth(root, rank).id; 
}
char filestring[500000];
void FreeAll(){//������е��ڴ� 
	int i;
	ListNode *pre, *j;
	for(i = 1; i <= tot; i++){
	  free(Map[i].filename);
	  free(Map[i].funcname);
	}
	for(i = 1; i <= tot; i++){
		for(j = call[i]->tail; j != NULL; j = pre){
		  pre = j->prev;
		  free(j);
		}
		free(call[i]);
	}
	for(i = 1; i <= tot; i++){
		for(j = caller[i]->tail; j != NULL; j = pre){
		  pre = j->prev;
		  free(j);
		}
		free(caller[i]);
	}
}
void FindFunction(char *folderpath){
	DIR *dp = NULL;
	FILE *fp = NULL;
 	struct dirent *Dirent;
 	struct stat filestate;
 	int ret = 0, cnt = 0;
 	char filename[1024] = {0};
 	dp = opendir(folderpath);
 	if(dp == NULL){
 	//	prevstate = globalstate;
        globalstate = NoFunction;
        return;
 	}
 	while(1){
  		Dirent = readdir(dp); //��ȡ�ļ�����Ϣ 
  		if(Dirent == NULL){//����ļ����Ѿ�û������Ŀ¼�����ļ��� 
   			break;
  		}
  		strcpy(filename, folderpath);
  		if(folderpath[strlen(folderpath) - 1] != '/') //�ж�·�����Ƿ��/
   			strcat(filename,"/");
  		strcat(filename, Dirent->d_name); //���ļ�·����
  		ret = stat(filename, &filestate); //�鿴Ŀ¼���ļ�����
  		if(ret < 0){
   			//prevstate = globalstate;
        	globalstate = NoFunction;
        	return;
  		}
  		if(S_ISDIR(filestate.st_mode)){//���ΪĿ¼�ļ�
   			if(0 == strcmp("..", Dirent->d_name) || 0 == strcmp(".", Dirent->d_name)) //���Ե�ǰĿ¼����һ��Ŀ¼
    			continue;
   			else
    			FindFunction(filename);  //�ݹ��ȡ
  		}
  		else{
   			//printf("%s\n",filename);
   			int len = strlen(filename);
   			if(len >= 2 && filename[len - 1] == 'c' && filename[len - 2] == '.'){//�����.c�ļ� 
   				fp = fopen(filename, "r");
            	cnt = 0;
            	while(!feof(fp)) filestring[cnt++] = getc(fp);
            	filestring[cnt++] = 0;
            	FindAllFunction(Dirent->d_name, filestring);
            	fclose(fp);
			}
  		}
	}
 	closedir(dp);
 	return ;
}
void FindFunctionUse(char *folderpath){
	DIR *dp = NULL;
	FILE *fp = NULL;
 	struct dirent *Dirent;
 	struct stat filestate;
 	int ret = 0, cnt = 0;
 	char filename[1024] = {0};
 	dp = opendir(folderpath);
 	if(dp == NULL){
 		//prevstate = globalstate;
        globalstate = NoFunction;
        return;
 	}
 	while(1){
  		Dirent = readdir(dp); //��ȡ�ļ�����Ϣ 
  		if(Dirent == NULL){//����ļ����Ѿ�û������Ŀ¼�����ļ��� 
   			break;
  		}
  		strcpy(filename, folderpath);
  		if(folderpath[strlen(folderpath) - 1] != '/') //�ж�·�����Ƿ��/
   			strcat(filename,"/");
  		strcat(filename, Dirent->d_name); //���ļ�·����
  		ret = stat(filename, &filestate); //�鿴Ŀ¼���ļ�����
  		if(ret < 0){
   			//prevstate = globalstate;
        	globalstate = NoFunction;
        	return;
  		}
  		if(S_ISDIR(filestate.st_mode)){//���ΪĿ¼�ļ�
   			if(0 == strcmp("..", Dirent->d_name) || 0 == strcmp(".", Dirent->d_name)) //���Ե�ǰĿ¼����һ��Ŀ¼
    			continue;
   			else
    			FindFunctionUse(filename);  //�ݹ��ȡ
  		}
  		else{
   			//printf("%s\n",filename);
   			int len = strlen(filename);
   			if(len >= 2 && filename[len - 1] == 'c' && filename[len - 2] == '.'){//�����.c�ļ� 
   				fp = fopen(filename, "r");
            	cnt = 0;
            	while(!feof(fp)) filestring[cnt++] = getc(fp);
            	filestring[cnt++] = 0;
            	FindAllFunctionUse(Dirent->d_name, filestring);
            	fclose(fp);
			}
  		}
	}
 	closedir(dp);
 	return ;
}
void Analyze(char *folderpath){
    if(usedtimes)
      FreeAll();
    //InitConsole();
    usedtimes++;
    TreapInit();
    FindFunction(folderpath);
    InitAllList();
    FindFunctionUse(folderpath);
}

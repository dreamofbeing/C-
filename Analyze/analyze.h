#ifndef ANALYZE_H
#define ANALYZE_H
typedef struct{
	int id; //���õ�id
	int line;//���õ����� 
}Call;
void Analyze(char *folderpath); 
int FindFuncByName(char *funcname);//�����������Ӧ��Map���±�  
int FindFuncByRank(int rank);//�����Ӧ�������±� 
#endif

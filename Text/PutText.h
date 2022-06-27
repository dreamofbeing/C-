#ifndef PUTTEXT_H
#define PUTTEXT_H
void Changexy();
void Page();
void PutText();
void TurnPage();
void TextPlay();
void PageButton();
void ReadResult();
typedef enum{
	HomePage = 0,
	TextEnterPath = 1,
	TextFunction = 2,
	ShowHelp = 3,
	StoreFile = 4,
	NoFunction = 5,
	OverSize = 6, 
	OneFunctionDiagram = 7,
	DiagramEnterPath = 8,
	FunctionNotFound = 9,
	ReadFromFile = 10, 
}GlobalState;
/*����ö�����ͣ��������ڻ�ͼ��״̬
0��ʾ��ҳ 
1��ʾ���ı���ʽ����������ǰ��·������ 
2��ʾ���ı���ʽ����������
3��ʾ��ʾhelp
4��ʾʾ���������Ϣ���ļ�
5��ʾ��ǰĿ¼û��C�ļ� 
6��ʾ������������ 
7��ʾ��������ĳ�������ĵ���ͼ
8��ʾ��������ĳ�������ĵ���ͼǰ��·������ 
9��ʾû���ҵ��ú���
10��ʾ���ļ����ȡ���� 
*/
typedef enum{
	NoChangePage = 0,
	NextPage = 1,
	PrevPage = 2,
}TextState;
/*
0��ʾά��ԭ״
1��ʾ��һҳ
2��ʾ��һҳ 
*/
#endif

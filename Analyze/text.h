#ifndef TEXT_H
#define TEXT_H
int IsSpaceOrEnter(char c);//�ж��Ƿ��ǻس��Ϳո� 
int IsLetterOrDightOrUnderline(char c);//�ж��ǲ����ַ������ֺ��»��ߣ��������ĺ����ַ��� 
void ChangeTextState(char *c);//�ı�����������ע��״̬������,
int MyGetLine();//���ص�ǰ������
int FindNextChar(int start, char *s, int len, char c);//�ڳ���Ϊlen�ַ���s��startλ�ÿ�ʼѰ����һ���Ϸ����ַ�c������λ�� 
int IsLegalFunction(int start, int end, char *s);//�ж��ǲ��ǺϷ��ĺ��� 
int IsLegalState();//�������ڵ�״̬�Ƿ������û���ע��״̬,�����򷵻�1 
int GetBracketDiffer();//��ȡ�����������Ų� 
void TextInit();//��ʼ���ļ��������� 
#endif

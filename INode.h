#ifndef INODE
#define INODE

#define _CRT_SECURE_NO_WARNINGS //�����

#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "Define.h"

typedef struct _INODE {
	char m_cFileName[4];	//���� �̸��� �ִ� ���̰� 4��
	unsigned m_uFileSize;	//data block�� �� 128 byte�� �� 1024�� �����Ƿ� �ִ� 2^17������ �� ǥ��
	unsigned m_uFileType : 1;	//���͸��� �Ϲ� ���� 2���� ������ ���� ����
	unsigned m_uYear : 14;	//��� 16,000(2^14)������ �⵵ ǥ��
	unsigned m_uMonth : 4;	//12��(2^4)������ �⵵ ǥ��
	unsigned m_uDay : 5;	//31��(2^5)������ �� ǥ��
	unsigned m_uHour : 5;	//24��(2^5)������ �ð� ǥ��
	unsigned m_uMinute : 6;	//59��(2^6)������ �� ǥ��
	unsigned m_uSecond : 6;	//59��(2^6)������ �� ǥ��
	//data block�� �� 1024���̹Ƿ� (0~2^10-1)�� ���� ǥ���ؾ� �� -> �̸� �����ϸ� ũ�Ⱑ �ּ��� �ڷ��� short�� ���
	short m_sDirect;		
	short m_sSingleIndirect;
	short m_sDoubleIndirect;
	struct _INODE* m_spNext; //Inode List�� Linked List����

}SINode;

void InitializeInode(SINode* spINode);
SINode* CreateINode(const char* cFileName, bool bFileType);

#endif // !INODE

#ifndef INODE
#define INODE

#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include "Define.h"
#include "SuperBlock.h"
#include "DataBlock.h"

typedef struct _DATABLOCK SDataBlock;
typedef struct _INODE {
	char m_cFileName[5];	//���� �̸��� �ִ� ���̰� 4��
	int m_iNumofINode;		//�ش� inode number
	unsigned m_uFileSize;	//data block�� �� 128 byte�� �� 1024�� �����Ƿ� �ִ� 2^17������ �� ǥ��
	unsigned m_uFileType : 1;	//�����͸��� �Ϲ� ���� 2���� ������ ���� ����
	unsigned m_uYear : 14;	//��� 16,000(2^14)������ �⵵ ǥ��
	unsigned m_uMonth : 4;	//12��(2^4)������ �⵵ ǥ��
	unsigned m_uDay : 5;	//31��(2^5)������ �� ǥ��
	unsigned m_uHour : 5;	//24��(2^5)������ �ð� ǥ��
	unsigned m_uMinute : 6;	//59��(2^6)������ �� ǥ��
	unsigned m_uSecond : 6;	//59��(2^6)������ �� ǥ��
							//data block�� �� 1024���̹Ƿ� (0~2^10-1)�� ���� ǥ���ؾ� �� -> �̸� �����ϸ� ũ�Ⱑ �ּ��� �ڷ��� short�� ����
	SDataBlock* m_Direct;
	SDataBlock* m_sSingleIndirect[102]; // 102����ŭ�� datablock ����
	SDataBlock* m_sDoubleIndirect[102][102]; //102*102��ŭ�� datablock ����
	struct _INODE* m_spParent;//�θ� inode ����Ŵ
	struct _INODE* m_spChild;//�ڽ� inode ����Ŵ
	struct _INODE* m_spPrevSibling; //����(����) inode ����Ŵ
	struct _INODE* m_spNextSibling; //������(����) inode ����Ŵ
}SINode;

typedef struct _DOUBLELINKEDLIST {
	SINode *m_spHead; //���� �����͸� ǥ��
	SINode *m_spIterator; //����Ʈ���� �Դٰ����ϴ� ����
}SDoubleLinkedList;


void InitializeInode(SINode* spINode);
int CreateINode(SINode* spINodeList[MAX_NUM_OF_LIST], char* cFileName, bool bFileType, SSuperBlock* spSuperBlock);
int SearchEmptyINode(SSuperBlock* spSuperBlock);

//Double Linked List ���� �ڵ�
int InsertFile(SDoubleLinkedList* spDLL, SINode* spINodeList[MAX_NUM_OF_LIST], SSuperBlock* spSuperBlock, char* cFileName, bool bFileType);
void InitDLLIterator(SDoubleLinkedList *spDLL);	//ù ��° iterator�� head�� ��ġ��Ŵ
SDoubleLinkedList* CreateDoubleLinkedList(SINode* spINodeList[MAX_NUM_OF_LIST]);//����Ʈ���� �̵��� SDoubleLinkedList ����ü �ϳ� ����
void IterateListToLeft(SDoubleLinkedList *spDLL);
void IterateListToRight(SDoubleLinkedList *spDLL);
void IterateListToParent(SDoubleLinkedList *spDLL);
void IterateListToChild(SDoubleLinkedList *spDLL);
void EraseINode(SDoubleLinkedList *spDLL, char* cName, SINode* spINodeList[MAX_NUM_OF_LIST]);
int FindParentDirectory(SINode* spCurrentLocation);


//free�Լ�
void FreeINode(SINode *spINodeList[MAX_NUM_OF_LIST]);	//�����Ҵ� �� inode�� free����
void FreeDoubleLinkedList(SDoubleLinkedList* spDLL);	//�����Ҵ� �� doublelinkedlist�� free����

#endif // !INODE

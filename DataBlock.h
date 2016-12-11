#ifndef DATABLOCK
#define DATABLOCK

#include "Define.h"
#include "INode.h"
#include<stdlib.h>
#include<stdbool.h>

typedef struct _INODE SINode;
typedef struct _DOUBLELINKEDLIST SDoubleLinkedList;

typedef struct _DATABLOCK{
	char m_cData[DataBlockSize]; //data block 한 개의 크기가 128Byte
	struct _DATABLOCK* m_spNext; //연결된 data block이 있다면 그것을 가리키는 용도
	int iNumOfDataBlock;	//해당 데이터블록의 넘버
	int iSizeOfDataBlock;	//해당 데이터블록의 사이즈
}SDataBlock;

typedef struct _SINGLELINKEDLIST {
	SDataBlock* m_spHead;
	SDataBlock* m_spIterator;
}SSingleLinkedList;

void InitializeDataBlock(SDataBlock* spDataBlock);
int SearchEmptyDataBlock(SSuperBlock* spSuperBlock);
SDataBlock* CreateDataBlock(SSuperBlock* spSuperBlock);
void InitSLLIterator(SINode* spCurrentINode, SSingleLinkedList *spSLL);
SSingleLinkedList* Create_S_LinkedList(SDataBlock* spDataBlock[MAX_NUM_OF_DATABLOCK]);
void PutData(SSingleLinkedList* spSLL, SINode* spCurrentINode, char* cData, SSuperBlock* spSuperBlock);
void EraseFile(SINode* spINodeList[MAX_NUM_OF_LIST], SDataBlock* spDataBlock[MAX_NUM_OF_DATABLOCK], SDoubleLinkedList *spDLL, SSingleLinkedList *spSLL, char* cName, SSuperBlock* spSuperBlock);
void FreeDataBlock(SDataBlock *spDataBlock[MAX_NUM_OF_DATABLOCK]);
void FreeSingleLinkedList(SSingleLinkedList* spSLL);
#endif // !DATABLOCK

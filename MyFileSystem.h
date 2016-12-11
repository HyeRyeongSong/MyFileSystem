#ifndef MYFILESYSTEM
#define MYFILESYSTEM

#include <stdio.h>
#include "Define.h"
#include "SuperBlock.h"
#include "INode.h"
#include "DataBlock.h"
#include "FindPath.h"
#include "Inputline.h"
typedef struct _DATABLOCK SDataBlock;
typedef struct _INODE SINode;
typedef struct _SUPERBLOCK SSuperBlock;
typedef struct _SINGLELINKEDLIST SSingleLinkedList;
typedef struct _DOUBLELINKEDLIST SDoubleLinkedList;

typedef struct _MYFILESYSTEM{
	unsigned m_uBootBlock : 16;
	SSuperBlock* m_sSuperBlock;
	SINode* m_spINodeList[MAX_NUM_OF_LIST]; //�� inode�� ������ 512��
	SDataBlock* m_spDataBlock[MAX_NUM_OF_DATABLOCK]; //�� datablock�� ������ 1024��
	SSingleLinkedList* spSLL;
	SDoubleLinkedList* spDLL;
}SMyFileSystem;

void InitializeFileSystem(SMyFileSystem* );
SMyFileSystem* CreateFileSystem();

#endif

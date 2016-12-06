#ifndef MYFILESYSTEM
#define MyFILESYSTEM

#include <stdio.h.>
#include "Define.h"
#include "SuperBlock.h"
#include "INode.h"
#include "DataBlock.h"

typedef struct _MYFILESYSTEM {
	unsigned m_uBootBlock : 16;
	SSuperBlock m_sSuperBlock;
	SINode* m_spINode[MAX_NUM_OF_LIST];
	SDataBlock* m_spDataBlock[MAX_NUM_OF_DATABLOCK];
}SMyFileSystem;

void InitializeFileSystem(SMyFileSystem* spMyFileSystem);
SMyFileSystem* CreateFileSystem();

#endif

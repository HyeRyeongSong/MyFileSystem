#ifndef DATABLOCK
#define DATABLOCK

#include "Define.h"

typedef struct _DATABLOCK {
	char m_cData[DataBlockSize]; //data block 한 개의 크기가 128Byte
}SDataBlock;

void InitializeDataBlock(SDataBlock* spData);

#endif // !DATABLOCK


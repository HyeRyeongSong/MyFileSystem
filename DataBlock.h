#ifndef DATABLOCK
#define DATABLOCK

#include "Define.h"
#include<stdlib.h>
#include<stdbool.h>

typedef struct _DATABLOCK {
	char m_cData[DataBlockSize]; //data block 한 개의 크기가 128Byte
	struct _DATABLOCK* m_spNext; //연결된 data block이 있다면 그것을 가리키는 용도
}SDataBlock;

void InitializeDataBlock(SDataBlock* spData);

#endif // !DATABLOCK

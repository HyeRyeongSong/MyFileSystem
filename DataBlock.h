#ifndef DATABLOCK
#define DATABLOCK

#include "Define.h"

typedef struct _DATABLOCK {
	char m_cData[DataBlockSize]; //data block �� ���� ũ�Ⱑ 128Byte
}SDataBlock;

void InitializeDataBlock(SDataBlock* spData);

#endif // !DATABLOCK


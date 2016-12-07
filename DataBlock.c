#include "DataBlock.h"

void InitializeDataBlock(SDataBlock* spDataBlock) {
	for (int i = 0; i < DataBlockSize; ++i) {
		spDataBlock->m_cData[i] = 0;
	}
}

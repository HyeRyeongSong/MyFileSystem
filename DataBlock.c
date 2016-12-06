#include "DataBlock.h"

void InitializeDataBlock(SDataBlock* spData) {
	for (int i = 0; i < DataBlockSize; ++i) {
		spData->m_cData[i] = 0;
	}
}
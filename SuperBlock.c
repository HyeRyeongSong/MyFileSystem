#include "SuperBlock.h"

void InitializeSuperBlock(SSuperBlock* spSSuperBlock) {
	for (int i = 0; i < 8; ++i) {
		spSSuperBlock->m_uInodeList[i] = 0;
	}
	for (int i = 0; i < 16; ++i) {
		spSSuperBlock->m_uDataBlock[i] = 0;
	}
}

SSuperBlock* MaskInodeList(SSuperBlock* spSuperBlock, int iNum) {
	unsigned long long uMask = 1;
	uMask <<= 63;
	if (iNum <= 64) {
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[0]) |= uMask;
	}
	else if (64 < iNum <= 128) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[1]) |= uMask;
	}
	else if (128 < iNum <= 192) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[2]) |= uMask;
	}
	else if (192 < iNum <= 256) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[3]) |= uMask;
	}
	else if (256 < iNum <= 320) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[4]) |= uMask;
	}
	else if (320 < iNum <= 384) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[5]) |= uMask;
	}
	else if (384 < iNum <= 448) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[6]) |= uMask;
	}
	else if (448 < iNum <= 512) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[7]) |= uMask;
	}
	else
		printf("ERROR\n");
	return spSuperBlock;
}
SSuperBlock* MaskDataBlock(SSuperBlock* spSuperBlock, int iNum) {
	unsigned long long uMask = 1;
	uMask <<= 63;
	if (iNum <= 64) {
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[0]) |= uMask;
	}
	else if (64 < iNum <= 128) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[1]) |= uMask;
	}
	else if (128 < iNum <= 192) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[2]) |= uMask;
	}
	else if (192 < iNum <= 256) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[3]) |= uMask;
	}
	else if (256 < iNum <= 320) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[4]) |= uMask;
	}
	else if (320 < iNum <= 384) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[5]) |= uMask;
	}
	else if (384 < iNum <= 448) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[6]) |= uMask;
	}
	else if (448 < iNum <= 512) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[7]) |= uMask;
	}
	else if (512 < iNum <= 576) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[8]) |= uMask;
	}
	else if (576 < iNum <= 640) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[9]) |= uMask;
	}
	else if (640 < iNum <= 704) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[10]) |= uMask;
	}
	else if (704 < iNum <= 768) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[11]) |= uMask;
	}
	else if (768 < iNum <= 832) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[12]) |= uMask;
	}
	else if (832 < iNum <= 896) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[13]) |= uMask;
	}
	else if (896 < iNum <= 960) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[14]) |= uMask;
	}
	else if (960 < iNum <= 1024) {
		iNum %= 64;
		uMask >>= (iNum - 1);
		(spSuperBlock->m_uInodeList[15]) |= uMask;
	}
	else
		printf("ERROR\n");
	return spSuperBlock;
}

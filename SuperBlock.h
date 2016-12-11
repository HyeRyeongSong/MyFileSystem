#ifndef SUPERBLOCK
#define SUPERBLOCK
#include<stdio.h>

typedef struct _SUPERBLOCK {
	unsigned long long m_uInodeList[8]; //512bit(=64Byte) : inode list ���뿩�� ����
	unsigned long long m_uDataBlock[16]; //1024bit(=128Byte) : datablock ���뿩�� ����
}SSuperBlock;

void InitializeSuperBlock(SSuperBlock* spSSuperBlock);
SSuperBlock* MaskInodeList(SSuperBlock* spSuperBlock, int iNum);
SSuperBlock* MaskDataBlock(SSuperBlock* spSuperBlock, int iNum);
SSuperBlock* MaskInodeListToZero(SSuperBlock* spSuperBlock, int iNum);
SSuperBlock* MaskDataBlockToZero(SSuperBlock* spSuperBlock, int iNum);
#endif

#include "INode.h"
#include "SuperBlock.h"


void InitializeInode(SINode* spINode) {
	for (int i = 0; i < 4; ++i) {
		spINode->m_cFileName[i] = 0;
	}
	spINode->m_uFileSize = 0;
	spINode->m_uFileType = 0;
	spINode->m_uYear = 0;
	spINode->m_uMonth = 0;
	spINode->m_uDay = 0;
	spINode->m_uHour = 0;
	spINode->m_uMinute = 0;
	spINode->m_uSecond = 0;
	spINode->m_sDirect = 0;
	for (int i = 0; i < 102; ++i) {
		spINode->m_sSingleIndirect[i] = 0;
		for (int j = 0; j < 102; ++j) {
			spINode->m_sDirect[i][j] = 0;
		}
	}
	spINode->m_spChild = NULL;
	spINode->m_spSibling = NULL;
}

SINode* CreateINode(const char* cFileName, bool bFileType) {
	SINode* spNewINode = (SINode*)calloc(1, sizeof(SINode));
	InitializeInode(spNewINode);
	strcpy(spNewINode->m_cFileName, cFileName);
	spNewINode->m_uFileType = bFileType;
	struct tm *t;
	time_t tTime = time(NULL);
	t = localtime(&tTime);
	spNewINode->m_uYear = t->tm_year + 1900;
	spNewINode->m_uMonth = t->tm_mon + 1;
	spNewINode->m_uDay = t->tm_mday;
	spNewINode->m_uHour = t->tm_hour;
	spNewINode->m_uMinute = t->tm_min;
	spNewINode->m_uSecond = t->tm_sec;

	return spNewINode;
}

int SearchINode(SSuperBlock* spSuperBlock) {
	unsigned long long uMask = 1;
	int iCount = 1;
	uMask <<= 63;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 64; ++j) {
			if (0==(uMask &= spSuperBlock->m_uInodeList[i])) {
				return iCount;
			}
			else {
				spSuperBlock->m_uInodeList[i] >>= 1;
			}
		}
	}
	return 0;
}
int SearchDataBlock(SSuperBlock* spSuperBlock) {
	unsigned long long uMask = 1;
	int iCount = 1;
	uMask <<= 63;
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 64; ++j) {
			if (0 == (uMask &= spSuperBlock->m_uInodeList[i])) {
				return iCount;
			}
			else {
				spSuperBlock->m_uInodeList[i] >>= 1;
			}
		}
	}
	return 0;
}

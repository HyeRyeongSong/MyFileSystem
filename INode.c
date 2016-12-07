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
	spINode->m_sSingleIndirect = 0;
	spINode->m_sDoubleIndirect = 0;
	spINode->m_spNext = 0;
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
	uMask <<= 63;
	for (int i = 0; i < 8; ++i) {
		for (int i = 0; i < 64; ++i) {
			if(uMask &= spSuperBlock->m_)
		}
	}
}
int SearchDataBlock(SSuperBlock* spSuperBlock);

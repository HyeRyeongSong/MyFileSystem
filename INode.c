#include "INode.h"

void InitializeInode(SINode* spINode) {
	for (int i = 0; i < 4; ++i) {
		spINode->m_cFileName[i] = 0;
	}
	spINode->m_iNumofINode = 0;
	spINode->m_uFileSize = 0;
	spINode->m_uFileType = 0;
	spINode->m_uYear = 0;
	spINode->m_uMonth = 0;
	spINode->m_uDay = 0;
	spINode->m_uHour = 0;
	spINode->m_uMinute = 0;
	spINode->m_uSecond = 0;
	spINode->m_Direct = 0;
	for (int i = 0; i < 102; ++i) {
		spINode->m_sSingleIndirect[i] = 0;
		for (int j = 0; j < 102; ++j) {
			spINode->m_sDoubleIndirect[i][j] = 0;
		}
	}
	spINode->m_spParent = NULL;
	spINode->m_spChild = NULL;
	spINode->m_spPrevSibling = NULL;
	spINode->m_spNextSibling = NULL;
}

//새 inode 생성(file size 제외하고 나머지 정보 모두 넣어줌)	//리턴 값으로 새로 생성된 inode의 inode number을 줌
int CreateINode(SINode* spINodeList[MAX_NUM_OF_LIST], char* cFileName, bool bFileType, SSuperBlock* spSuperBlock) {
	SINode* spNewINode = (SINode*)calloc(1, sizeof(SINode));
	InitializeInode(spNewINode);
	strcpy(spNewINode->m_cFileName, cFileName);
	spNewINode->m_cFileName[4] = '\0';	
	spNewINode->m_uFileType = bFileType;
	spNewINode->m_iNumofINode = SearchEmptyINode(spSuperBlock);
	MaskInodeList(spSuperBlock, spNewINode->m_iNumofINode);	
	struct tm *t;
	time_t tTime = time(NULL);
	t = localtime(&tTime);
	spNewINode->m_uYear = t->tm_year + 1900;
	spNewINode->m_uMonth = t->tm_mon + 1;
	spNewINode->m_uDay = t->tm_mday;
	spNewINode->m_uHour = t->tm_hour;
	spNewINode->m_uMinute = t->tm_min;
	spNewINode->m_uSecond = t->tm_sec;

	spINodeList[spNewINode->m_iNumofINode] = spNewINode;	

	return spNewINode->m_iNumofINode;
}

//superblock 마스킹을 통해 비어있는 inode를 찾아줌
int SearchEmptyINode(SSuperBlock* spSuperBlock) {
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

//////////////////////////////////////////////
//이진 트리의 double linked list 형태 관련 함수
//////////////////////////////////////////////


//새로운 디렉터리 혹은 파일을 현재 디렉터리에 추가하는 함수
////iCurrentDirINodeNum는 pwd했을 때 나오는 디렉터리의 inode number임
int InsertFile(SDoubleLinkedList* spDLL, SINode* spINodeList[MAX_NUM_OF_LIST], SSuperBlock* spSuperBlock, char* cFileName, bool bFileType) {
	int iNumofINode = CreateINode(spINodeList, cFileName, bFileType, spSuperBlock);
	SINode *spFindINode;

	spFindINode = spDLL->m_spHead;

	if (spFindINode->m_spChild == NULL) {
		spFindINode->m_spChild = spINodeList[iNumofINode];
		spINodeList[iNumofINode]->m_spParent = spFindINode;
	}

	else {			
		spFindINode = spFindINode->m_spChild;
		while (!(strncmp(cFileName, spFindINode->m_cFileName, 1) < 0)){
			spFindINode = spFindINode->m_spNextSibling;
		}
		spINodeList[iNumofINode]->m_spPrevSibling = spFindINode->m_spPrevSibling;
		spINodeList[iNumofINode]->m_spNextSibling = spFindINode;
		spFindINode->m_spPrevSibling = spINodeList[iNumofINode];
		spINodeList[iNumofINode]->m_spPrevSibling->m_spNextSibling = spINodeList[iNumofINode];
	}
	return iNumofINode;
}

void InitDLLIterator(SDoubleLinkedList *spDLL) {
	spDLL->m_spIterator = spDLL->m_spHead;
}

SDoubleLinkedList* CreateDoubleLinkedList(SINode* spINodeList[MAX_NUM_OF_LIST]) {	
	SDoubleLinkedList* spNewList;
	spNewList = (SDoubleLinkedList*)calloc(1, sizeof(SDoubleLinkedList));
	spNewList->m_spHead = spINodeList[1];	
	InitDLLIterator(spNewList);
	return spNewList;
}

//interator를 왼쪽으로 이동시키는 함수
void IterateListToLeft(SDoubleLinkedList *spDLL) {
	if (spDLL->m_spIterator != NULL) {
		spDLL->m_spIterator = spDLL->m_spIterator->m_spPrevSibling;
	}
}

//interator를 오른쪽으로 이동시키는 함수
void IterateListToRight(SDoubleLinkedList *spDLL) {
	if (spDLL->m_spIterator != NULL) {
		spDLL->m_spIterator = spDLL->m_spIterator->m_spNextSibling;
	}
}

//interator를 위로 이동시키는 함수
void IterateListToParent(SDoubleLinkedList *spDLL) {
	if (spDLL->m_spIterator != NULL) {
		spDLL->m_spIterator = spDLL->m_spIterator->m_spParent;
	}
}

//해당 이름의 inode하나 삭제(연결 끊기 및 재배치 + inode list에서 삭제)
void IterateListToChild(SDoubleLinkedList *spDLL) {
	if (spDLL->m_spIterator != NULL) {
		spDLL->m_spIterator = spDLL->m_spIterator->m_spChild;
	}
}

//해당 이름의 inode하나 삭제(연결 끊기 및 재배치 + inode list에서 삭제)
void EraseINode(SDoubleLinkedList *spDLL, char* cName, SINode* spINodeList[MAX_NUM_OF_LIST]) {
	int iDeletedINodeNum;
	SINode* spFindINode = spDLL->m_spHead;
	if (0 == strcmp(cName, spFindINode->m_spChild->m_cFileName)) {
		spFindINode->m_spChild = spFindINode->m_spChild->m_spNextSibling;
		spFindINode->m_spChild->m_spParent = spFindINode;
		iDeletedINodeNum = spFindINode->m_spChild->m_spPrevSibling->m_iNumofINode;
		spINodeList[iDeletedINodeNum]->m_spParent = NULL; 
		spINodeList[iDeletedINodeNum]->m_spNextSibling = NULL; 
		spFindINode->m_spChild->m_spPrevSibling = NULL;
		spINodeList[iDeletedINodeNum] = NULL;
	}
	else {
		spFindINode = spFindINode->m_spChild;
		while (0 != strcmp(cName, spFindINode->m_cFileName)) {
			spFindINode = spFindINode->m_spNextSibling;
		}
		spFindINode->m_spPrevSibling->m_spNextSibling = spFindINode->m_spNextSibling;
		spFindINode->m_spNextSibling->m_spPrevSibling = spFindINode->m_spPrevSibling;
		iDeletedINodeNum = spFindINode->m_iNumofINode;
		spFindINode->m_spPrevSibling = NULL; 
		spFindINode->m_spNextSibling = NULL; 
		spINodeList[iDeletedINodeNum] = NULL; 
	}
}

///////현재 파일이나 디렉터리의 부모 디렉터리를 찾아주고 그 것의 inode nuber을 리턴해줌
int FindParentDirectory(SINode* spCurrentLocation) {
	SINode* spFindParentDirectory = spCurrentLocation;
	if (spFindParentDirectory->m_spParent != NULL) {	
		return spFindParentDirectory->m_spParent->m_iNumofINode;
	}
	else {
		while (spFindParentDirectory->m_spPrevSibling != NULL) {	
			spFindParentDirectory = spFindParentDirectory->m_spPrevSibling;
		}
		spFindParentDirectory = spFindParentDirectory->m_spParent;
		return spFindParentDirectory->m_iNumofINode;
	}
}

////////////////////
//동적할당 해제 함수
////////////////////

void FreeINode(SINode *spINodeList[MAX_NUM_OF_LIST]) {
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		InitializeInode(spINodeList[i]);
	}
	if (spINodeList)
		free(spINodeList);
}

void FreeDoubleLinkedList(SDoubleLinkedList* spDLL) {	
	free(spDLL);
}

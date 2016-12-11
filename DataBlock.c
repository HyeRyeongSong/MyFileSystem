#include "DataBlock.h"

void InitializeDataBlock(SDataBlock* spDataBlock) {
	for (int i = 0; i < DataBlockSize; ++i) {
		spDataBlock->m_cData[i] = 0;
	}
	spDataBlock->m_spNext = NULL;
	spDataBlock->iNumOfDataBlock = 0;
	spDataBlock->iSizeOfDataBlock = 0;
}

int SearchEmptyDataBlock(SSuperBlock* spSuperBlock) {
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


//���� ������ ���� ������ datablock �����͸� �־���(�� �Լ��� ������ �� �ǵ帲)
SDataBlock* CreateDataBlock(SSuperBlock* spSuperBlock) {
	SDataBlock* spNewDataBlock = (SDataBlock *)calloc(1, sizeof(SDataBlock));
	InitializeDataBlock(spNewDataBlock); //�ʱ�ȭ ���� ����
	spNewDataBlock->iNumOfDataBlock = SearchEmptyDataBlock(spSuperBlock);
	MaskDataBlock(spSuperBlock, spNewDataBlock->iNumOfDataBlock);	//superblock ����ŷ�� ������ ���ÿ� ����
	return spNewDataBlock;
}


//�ش� ���̳����� �����͸� ó������ ����Ű���� �̱۸�ũ�� ����Ʈ ����ġ
void InitSLLIterator(SINode* spCurrentINode, SSingleLinkedList *spSLL) {
	spCurrentINode->m_Direct = spSLL->m_spHead;
	spSLL->m_spIterator = spSLL->m_spHead;
}

//Single-Linke-List ������ �������ְ� �ʱ�ȭ
SSingleLinkedList* Create_S_LinkedList(SDataBlock* spDataBlock[MAX_NUM_OF_DATABLOCK]) {
	SSingleLinkedList* spNewList;
	spNewList = (SSingleLinkedList*)calloc(1, sizeof(SSingleLinkedList));
	spNewList->m_spHead = NULL;
	spNewList->m_spIterator = NULL;
	return spNewList;
}



//���ο� data�� �������� �̸� 128byte�� �ɰ��� filesystem�� datablock list�� �־��ְ� ��ũ�帮��Ʈ�� ��������
void PutData(SSingleLinkedList* spSLL, SINode* spCurrentINode, char* cData, SSuperBlock* spSuperBlock) {
	char cInputData[1344897] = { '\0' };
	int iFileSize = 0;	//�� �Է� �������� ũ��
	int iNumOfDataBlock = 0;	//���忡 �ʿ��� �����ͺ����� ����
	char cBuffer[128] = { '\0' };
	scanf("%s", cInputData);
	cInputData[1344596] = '\0';	//1344595����Ʈ �̻��� �����Ͱ� �������� �߸��� ����
	for (int i = 0; i < sizeof(cInputData); ++i) {	//�� �Է� ������ ũ�� ��ȯ
		if (cInputData[i] != '\0') {
			++iFileSize;
		}
	}
	spCurrentINode->m_uFileSize = iFileSize;	//inode�� ���� ������ �־���(�嵮 �ذ�)
	iNumOfDataBlock = (iFileSize / 128) + 1;

	spCurrentINode->m_Direct = CreateDataBlock(spSuperBlock);	//���ο� ������ ���� ����
	InitSLLIterator(spCurrentINode, spSLL);	//head�� iterator�ʱ�ȭ

	strncpy(cBuffer, cInputData, 128);	//128����Ʈ��ŭ cBuffer�� ī��
	strcpy(spSLL->m_spIterator->m_cData, cBuffer);	//directblock�� �־���
	--iNumOfDataBlock;

	if (iNumOfDataBlock > 0) {
		for (int i = 0; (i<102) || ((i < iNumOfDataBlock) && (iNumOfDataBlock<=102)); ++i) {
			spCurrentINode->m_sSingleIndirect[i] = CreateDataBlock(spSuperBlock);	//singleindirect �ϳ� ����

			//�̰Ŵ� �������ִ� �ڵ�
			if (i == 0) {
				spCurrentINode->m_Direct->m_spNext = spCurrentINode->m_sSingleIndirect[i];
			}
			else {
				spCurrentINode->m_sSingleIndirect[i-1]->m_spNext = spCurrentINode->m_sSingleIndirect[i];
			}

			//iterator�� �̵� �ڵ�
			spSLL->m_spIterator = spSLL->m_spIterator->m_spNext;

			for (int j = 0; j < iFileSize; ++j) {	//128����Ʈ�� cInputData�迭�� �ε����� ������ �о���
				cInputData[j] = cInputData[j + 128];
			}
			iFileSize -= 128;

			strncpy(cBuffer, cInputData, 128);	//128����Ʈ��ŭ cBuffer�� ī��
			strcpy(spSLL->m_spIterator->m_cData, cInputData); //SingleIndirect[i]�� �־���
		}
	}
	if (iNumOfDataBlock > 102) {
		iNumOfDataBlock -= 102;
		for (int i = 0; i < 102; ++i) {
			for (int k = 0; k < 102; ++k) {
				spCurrentINode->m_sDoubleIndirect[i][k] = CreateDataBlock(spSuperBlock);	//singleindirect �ϳ� ����

				//�̰Ŵ� �������ִ� �ڵ�
				if (k == 0) {
					spCurrentINode->m_sDoubleIndirect[i - 1][102]->m_spNext = spCurrentINode->m_sDoubleIndirect[i][0];
				}
				else {
					spCurrentINode->m_sDoubleIndirect[i][k - 1]->m_spNext = spCurrentINode->m_sDoubleIndirect[i][k];
				}


				//iterator�� �̵� �ڵ�
				spSLL->m_spIterator = spSLL->m_spIterator->m_spNext;

				for (int j = 0; j < iFileSize; ++j) {	//128����Ʈ�� cInputData�迭�� �ε����� ������ �о���
					cInputData[j] = cInputData[j + 128];
				}
				iFileSize -= 128;

				strcpy(cBuffer, cInputData);	//128����Ʈ��ŭ cBuffer�� ī��
				strncpy(spSLL->m_spIterator->m_cData, cInputData, 128); //SingleIndirect[i]�� �־���
			}
		}
	}
}

void EraseFile(SINode* spINodeList[MAX_NUM_OF_LIST], SDataBlock* spDataBlock[MAX_NUM_OF_DATABLOCK], SDoubleLinkedList *spDLL, SSingleLinkedList *spSLL, char* cName, SSuperBlock* spSuperBlock) {
	int iNumOfINode; //���� ������ inode number
	SDataBlock* spBuffer = NULL;
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		if (0 == strcmp(spINodeList[i]->m_cFileName, cName)) {
			iNumOfINode = i;
			break;
		}
	}
	InitSLLIterator(spINodeList[iNumOfINode], spSLL);
	while (spSLL->m_spIterator->m_spNext != NULL) {	//�ش� ���̳��忡 ������ ������ ������ �ϳ��� ������
		spBuffer = spSLL->m_spIterator->m_spNext;
		MaskDataBlockToZero(spSuperBlock, spSLL->m_spIterator->iNumOfDataBlock); //0���� ����ŷ
		InitializeDataBlock(spSLL->m_spIterator);
		spSLL->m_spIterator = spBuffer;
	}
	EraseINode(spDLL, cName, spINodeList);
	MaskInodeListToZero(spSuperBlock, iNumOfINode);
}
void FreeDataBlock(SDataBlock *spDataBlock[MAX_NUM_OF_DATABLOCK]) {
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		InitializeDataBlock(spDataBlock[i]);
	}
	free(spDataBlock);
}

void FreeSingleLinkedList(SSingleLinkedList* spSLL) {
	free(spSLL);
}

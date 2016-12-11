#include "MyFileSystem.h"

void InitializeFileSystem(SMyFileSystem* spMyFileSystem) {
	spMyFileSystem->m_uBootBlock = 0;
	InitializeSuperBlock(spMyFileSystem->m_sSuperBlock);
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		spMyFileSystem->m_spINodeList[i] = 0;
	}
	for (int i = 0; i < MAX_NUM_OF_DATABLOCK; ++i) {
		spMyFileSystem->m_spDataBlock[i] = 0;
	}
}

SMyFileSystem* CreateFileSystem() {
	FILE *file;
	file = fopen("myfs","r");
	SMyFileSystem* spMyFileSystem = (SMyFileSystem*)calloc(1, sizeof(SMyFileSystem));

	if (file != NULL) {
		printf("ERROR\n");
		return 0;
	}
	else {
		fclose(file);
		file = fopen("myfs","a+");
		spMyFileSystem->m_sSuperBlock = (SSuperBlock*)calloc(1, sizeof(SSuperBlock));
		*spMyFileSystem->m_spINodeList = (SINode*)calloc(MAX_NUM_OF_LIST, sizeof(SINode));
		*spMyFileSystem->m_spDataBlock = (SDataBlock*)calloc(MAX_NUM_OF_DATABLOCK, sizeof(SDataBlock));
		CreateINode(spMyFileSystem->m_spINodeList,"root", Directory, spMyFileSystem->m_sSuperBlock);
		MaskInodeList(spMyFileSystem->m_sSuperBlock, 1);
		spMyFileSystem->spDLL = (SDoubleLinkedList*)calloc(1, sizeof(SDoubleLinkedList));
		spMyFileSystem->spSLL = (SSingleLinkedList*)calloc(1, sizeof(SSingleLinkedList));
		spMyFileSystem->spDLL = CreateDoubleLinkedList(spMyFileSystem->m_spINodeList);
		spMyFileSystem->spSLL = Create_S_LinkedList(spMyFileSystem->m_spDataBlock);
	}
	InitializeFileSystem(spMyFileSystem);
	fclose(file);
	return spMyFileSystem;
}

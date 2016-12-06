#include "MyFileSystem.h"

void InitializeFileSystem(SMyFileSystem* spMyFileSystem) {
	spMyFileSystem->m_uBootBlock = 0;
	InitializeSuperBlock(&spMyFileSystem->m_sSuperBlock);
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		spMyFileSystem->m_spINode[i] = 0;
	}
	for (int i = 0; i < MAX_NUM_OF_DATABLOCK; ++i) {
		spMyFileSystem->m_spDataBlock[i] = 0;
	}
}
SMyFileSystem* CreateFileSystem() {
	FILE *file;
	file = fopen("myfs.txt", "rb+");
	SMyFileSystem* spMyFileSystem = (SMyFileSystem*)calloc(1, sizeof(SMyFileSystem));
	InitializeFileSystem(spMyFileSystem);
	if(file != NULL){
		printf("ERROR\n");
		return 0;
	}
	else {
		SSuperBlock* spSSuperBlock;
		spSSuperBlock = (SSuperBlock*)calloc(1, sizeof(SSuperBlock));
		spMyFileSystem->m_spINode[1] = CreateINode("root", Directory);
		MaskInodeList(spSSuperBlock, 1);
	}
	return spMyFileSystem;
}
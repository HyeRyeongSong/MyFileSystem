#ifndef FINDPATH
#define FINDPATH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MyFileSystem.h"
typedef struct _MYFILESYSTEM SMyFileSystem;
SINode * SearchFileAddress(SMyFileSystem *spMyFileSystem, char *address, SINode *current_p);       //�ش� ���� ���� ����Ʈ �Լ�
void RenewAddress(char *current_add, char *address);
#endif

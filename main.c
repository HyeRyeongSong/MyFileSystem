#include<stdio.h>
#include "MyFileSystem.h"
int main() {
	char cDirectory = '/';
	char cBuffer[16];
	while (true) {
		printf("[%c }$ ", cDirectory);
		scanf("%[^ \n\t]", cBuffer);
		if (!(strcmp("mymkfs", cBuffer)))
			CreateFileSystem();
	}
	return 0;
}
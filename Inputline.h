#ifndef INPUTLINE
#define INPUTLINE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct{//명령 받는 구조체 선언
		char cCommand[20];
		char cOption[2];
		char *cp_Other[100];
}SCommandLine;

void Initialize(SCommandLine *);//구조체를 초기화 해주는 함수
void FreeCommandLine(SCommandLine *);//구조체에 할당된 메모리 해제
SCommandLine* Inputline(char *, SCommandLine *);//명령 입력 함수
#endif

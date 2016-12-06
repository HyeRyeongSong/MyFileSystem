#ifndef INODE
#define INODE

#define _CRT_SECURE_NO_WARNINGS //지우기

#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "Define.h"

typedef struct _INODE {
	char m_cFileName[4];	//파일 이름의 최대 길이가 4자
	unsigned m_uFileSize;	//data block이 각 128 byte씩 총 1024개 있으므로 최대 2^17까지의 수 표기
	unsigned m_uFileType : 1;	//디렉터리와 일반 파일 2가지 종류의 파일 구분
	unsigned m_uYear : 14;	//적어도 16,000(2^14)까지의 년도 표기
	unsigned m_uMonth : 4;	//12월(2^4)까지의 년도 표기
	unsigned m_uDay : 5;	//31일(2^5)까지의 일 표기
	unsigned m_uHour : 5;	//24시(2^5)까지의 시간 표기
	unsigned m_uMinute : 6;	//59분(2^6)까지의 분 표기
	unsigned m_uSecond : 6;	//59초(2^6)까지의 초 표기
	//data block이 총 1024개이므로 (0~2^10-1)의 수를 표현해야 함 -> 이를 충족하며 크기가 최소인 자료형 short을 사용
	short m_sDirect;		
	short m_sSingleIndirect;
	short m_sDoubleIndirect;
	struct _INODE* m_spNext; //Inode List가 Linked List형태

}SINode;

void InitializeInode(SINode* spINode);
SINode* CreateINode(const char* cFileName, bool bFileType);

#endif // !INODE

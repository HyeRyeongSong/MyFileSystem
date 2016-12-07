#include<stdio.h>
#include "MyFileSystem.h"
void MyTouch(SINode**, char*);
int main() {
	char cDirectory = '/';
	char cBuffer[16]; //입력된 명령어 저장
	char cFileName[16]; //입력된 파일 이름

	while (true) {
		printf("[%s }$ ", cDirectory);//"프롬프트 표기...?"
		scanf("%[^\n\t]", cBuffer);

		//mymkfs
		if (!(strcmp("mymkfs", cBuffer)))
			CreateFileSystem();

		//mytouch
		if (!(strcmp("mytouch"))) {//"오늘 코드 참고"
			fopen(cFileName, "rb"); //"함수로 대체"
		}
	}
	return 0;
}
void MyTouch(SINode** spINodeList, char* cFileName) {
	bool bExist = false;
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		if (spINodeList[i] != NULL) {
			//파일 수정 시간 변경
			if (0 == strcmp(spINodeList[i]->m_cFileName, cFileName)) {
				struct tm *t;
				time_t tTime = time(NULL);
				t = localtime(&tTime);
				spINodeList[i]->m_uYear = t->tm_year + 1900;
				spINodeList[i]->m_uMonth = t->tm_mon + 1;
				spINodeList[i]->m_uDay = t->tm_mday;
				spINodeList[i]->m_uHour = t->tm_hour;
				spINodeList[i]->m_uMinute = t->tm_min;
				spINodeList[i]->m_uSecond = t->tm_sec;
				bExist = true;
			}
			//0바이트 파일 생성
		}
	}
	if (bExist == false) {
		
		//inode 알려주는 함수
	}
}

#include<stdio.h>
#include "MyFileSystem.h"

int iCount = 0;

int FindCommandNumber(SCommandLine *spCommandLine);
void MyTouch(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, char* cFileName);
void MyMakeDirectory(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, char* cDirectoryName);
void Myls(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, SCommandLine* spCommandLine);
void ByeBye(SMyFileSystem* spMyFileSystem);
void MyPwd(char * current_add);
SINode * MyCd(char * current_add, char * address, SINode * Head, SMyFileSystem* spMyFileSystem);
void MyTree(SMyFileSystem* spMyFileSystem, char * address, SINode * current_p);
void MyShowFile(char * num1, char * num2, char * file, SINode * head, SMyFileSystem * smMyFileSystem);
void MyRemove(SMyFileSystem* spMyFileSystem, char* cName);
void MyRemoveDirectory(SMyFileSystem* spMyFileSystem, char* cName);
void MyShowINode(SMyFileSystem* spMyFileSystem, int iNumOfINode);
void MyShowBlock(SMyFileSystem* spMyFileSystem, int iNumOfDataBlock);
void MyState(SSuperBlock* spSuperBlock);
void MyMove(SMyFileSystem* spMyFileSystem, char* cName, SCommandLine* spCommandLine);

int main() {
	SCommandLine* spCommandLine;
	SDoubleLinkedList* spDLL;
	char cCurrentRoute[1024] = "/";
	char cBuffer[1024] = ""; //입력된 명령어 저장
	char cFileName[16]; //입력된 파일 이름

	SMyFileSystem* spMyFileSystem = (SMyFileSystem*)calloc(1, sizeof(SMyFileSystem));
	spCommandLine = (SCommandLine *)calloc(1, sizeof(SCommandLine));

	while (true) {
		Initialize(spCommandLine);

		printf("실행완료1\n");
		for(int i = 0; i < 1024; i++)
			cBuffer[i] = 0;
		printf("[%s ]$ ", cCurrentRoute);//프롬프트 표기
		scanf("%[^\n\t]", cBuffer);
		getchar();

		printf("%s\n",cBuffer);

		/*byebye*/
		if (cBuffer[0] == 'b' && cBuffer[1] == 'y' && cBuffer[2] == 'e') {
			FreeCommandLine(spCommandLine);
			ByeBye(spMyFileSystem);
		}

		if (cBuffer[0] != 'm' && cBuffer[1] != 'y'){
			system(cBuffer);/*command*/

			printf("실행완료2\n");

		}else {
			Inputline(cBuffer, spCommandLine);//임시입력버퍼의 명령문 분류

			if (!(strcmp("mymkfs", cBuffer))) {
				spMyFileSystem = CreateFileSystem(spDLL);	//파일시스템 생성
				continue;
			}

			switch (FindCommandNumber(spCommandLine)) {
				case myls:
					Myls(spMyFileSystem, spMyFileSystem->spDLL, spCommandLine);
					break;
				case mycat: break;
				case myshowfile:
							MyShowFile(spCommandLine->cp_Other[0], spCommandLine->cp_Other[1], spCommandLine->cp_Other[2], spDLL->m_spHead, spMyFileSystem);
							break;
				case mypwd:
							MyPwd(cCurrentRoute);
							break;
				case mycd:
							spDLL->m_spHead = MyCd(cCurrentRoute, spCommandLine->cp_Other[0], spDLL->m_spHead, spMyFileSystem);
							break;
				case mycp: break;
				case mycpto: break;
				case mycpfrom: break;
				case mymkdir:
							   MyMakeDirectory(spMyFileSystem, spMyFileSystem->spDLL, spCommandLine->cp_Other[0]);
							   break;
				case myrmdir:
							   MyRemoveDirectory(spMyFileSystem, spCommandLine->cp_Other[0]);
							   break;
				case myrm:
							   MyRemove(spMyFileSystem, spCommandLine->cp_Other[0]);
							   break;
				case mymv: 
								MyMove(spMyFileSystem, spCommandLine->cp_Other[0], spCommandLine->cp_Other[1]);
								break;
				case mytouch:
						   MyTouch(spMyFileSystem, spMyFileSystem->spDLL, spCommandLine->cp_Other[0]);
						   break;
				case myshowinode: 
							MyShowINode(spMyFileSystem, spCommandLine->cp_Other[0]);
							break;
				case myshowblock: 
							MyShowBlock(spMyFileSystem, spCommandLine->cp_Other[0]);
							break;
				case mystate: 
							MyState(spMyFileSystem->m_sSuperBlock);
				break;
				case mytree:
							  MyTree(spMyFileSystem, spCommandLine->cp_Other[0], spDLL->m_spHead);
							  break;
				default:
							  printf("ERROR\n");
			}
		}
	}
	return 0;
}

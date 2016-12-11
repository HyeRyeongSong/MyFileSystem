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

void MyTouch(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, char* cFileName) {
	bool bExist = false;
	int iNumOfINode;
	for (int i = 0; i < MAX_NUM_OF_LIST; ++i) {
		if (spMyFileSystem->m_spINodeList[i] != NULL) {
			/*파일 수정 시간 변경*/
			if (0 == strcmp(spMyFileSystem->m_spINodeList[i]->m_cFileName, cFileName)) {
				struct tm *t;
				time_t tTime = time(NULL);
				t = localtime(&tTime);
				spMyFileSystem->m_spINodeList[i]->m_uYear = t->tm_year + 1900;
				spMyFileSystem->m_spINodeList[i]->m_uMonth = t->tm_mon + 1;
				spMyFileSystem->m_spINodeList[i]->m_uDay = t->tm_mday;
				spMyFileSystem->m_spINodeList[i]->m_uHour = t->tm_hour;
				spMyFileSystem->m_spINodeList[i]->m_uMinute = t->tm_min;
				spMyFileSystem->m_spINodeList[i]->m_uSecond = t->tm_sec;
				bExist = true;
			}
			/*일치하는 파일 없을 경우 0바이트 파일 생성*/
		}
	}
	if (bExist == false) {
		iNumOfINode = InsertFile(spDLL, spMyFileSystem->m_spINodeList, spMyFileSystem->m_sSuperBlock, cFileName, File);
		spMyFileSystem->m_spINodeList[iNumOfINode]->m_uFileSize = 0;
		/*파일크기 함수 작성해야함*/
	}
}

/*void MyMakeDirectory(SDataBlock* spDataBlock, SINode** spINodeList, SSuperBlock* spSuperBlock,int iCurrentDirINodeNum, char cDirectoryName) { //iNum은 현재 내가 있는 디렉터리의 inode 넘버*/
void MyMakeDirectory(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, char* cDirectoryName) {
	int iNumOfINode = InsertFile(spDLL, spMyFileSystem->m_spINodeList, spMyFileSystem->m_sSuperBlock, cDirectoryName, Directory);
	spMyFileSystem->m_spINodeList[iNumOfINode]->m_uFileSize = sizeof(cDirectoryName);
	/*데이터 블록 링크드리스트 작성 후 수정하기*/
}

void Initialize(SCommandLine *spCommandLine) {

	iCount = 0;
	for (int i = 0; i < 20; i++)
		spCommandLine->cCommand[i] = '\0';

	for (int i = 0; i < 2; i++)
		spCommandLine->cOption[i] = '\0';

	for (int i = 0; i < 100; i++)
		spCommandLine->cp_Other[i] = NULL;

}

void FreeCommandLine(SCommandLine *spCommandLine) {

	for (int i = 0; i < 100; i++)
		if (spCommandLine->cp_Other[i] != NULL)
			free(spCommandLine->cp_Other[i]);

	Initialize(spCommandLine);

	free(spCommandLine);

}

SCommandLine* Inputline(char *cBuffer, SCommandLine *spCommandLine) {

	int iCnt = 0;
	int j = 0;
	char *cCheck;

	for (int i = 0; cBuffer[i] != '\0'; i++)
		if (cBuffer[i] == ' ')
			iCnt++;

	cCheck = strtok(cBuffer, " ");
	strcpy((spCommandLine->cCommand), cCheck);

	for (int i = 0; i < iCnt; i++) {
		cCheck = strtok(NULL, " ");
		if (cCheck[0] == '-') {
			cCheck = &cCheck[1];

			if (spCommandLine->cOption[0] != '\0')
				strcpy((spCommandLine->cOption + 1), cCheck);

			else
				strcpy((spCommandLine->cOption + 0), cCheck);
		}

		else {
			spCommandLine->cp_Other[j] = calloc(1024, sizeof(char));
			strcpy((spCommandLine->cp_Other[j]), cCheck);
			j++;
		}
	}

	for (int i = 0; spCommandLine->cp_Other[i] != '\0'; i++)
		iCount++;

	iCount = iCount - 1;

	return spCommandLine;

}

void Myls(SMyFileSystem* spMyFileSystem, SDoubleLinkedList* spDLL, SCommandLine* spCommandLine) {
	char cOption = 0;  //cOption의 값이 0이면 옵션 없음, 1이면 옵션 i, 2면 옵션 l, 3이면 옵션 i와l 모두
	int iCountNumOfFiles = 0;      //인자로 들어온 파일의 개수
	char cFileType; //파일 타입
	int iNumOfParentDirectory; //부모 디렉터리의 아이노드 넘버

	/*옵션 유무여부와 유일 경우 어떤 옵션들이 있는지 확인*/
	if (spCommandLine->cOption[0] == 'i') {
		if (spCommandLine->cOption[1] == 'l') {
			cOption = 3;
		}
		else {
			cOption = 1;
		}
	}
	else if (spCommandLine->cOption[0] == 'l') {
		if (spCommandLine->cOption[1] == 'i') {
			cOption = 3;
		}
		else {
			cOption = 2;
		}
	}

	/*인자로 들어온 파일의 개수 대입*/
	for (int i = 0; i < 100; ++i) {
		if (spCommandLine->cp_Other[i] != NULL) {
			++iCountNumOfFiles;
		}
	}

	spDLL->m_spIterator = spDLL->m_spHead;

	switch (cOption) {
		case 0:  //옵션이 없는 경우
			printf(".\n..\n"); //현재 디렉터리 상위 디렉터리 출력
			if (iCountNumOfFiles == 0) { //인자로 받은 파일이 없는 경우 "전체 파일 출력"
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL);//현재 디렉터리에서 아래로 이동
					printf("%s\n", spDLL->m_spIterator->m_cFileName);//첫 번째 하위 파일 이름 출력
					while (spDLL->m_spIterator->m_spNextSibling != NULL) {//오른쪽으로 이동하며 파일 이름 출력
						IterateListToRight(spDLL);
						printf("%s\n", spDLL->m_spIterator->m_cFileName);
					}
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator원상 복귀
					return;
				}
				else {
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls 호출을 위해 iterator 원상 복귀
					return; //하위 파일이 없으므로 다시 프롬프트로 돌아감
				}
			}

			else if (iCountNumOfFiles > 0) { //인자로 받은 파일이 있는 경우
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL); //현재 디렉터리에서 아래로 이동
				}
				for (int i = 0; i < iCountNumOfFiles; ++i) {   //총 iCountNumOfFiles의 이름을 출력함
					if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) {
						printf("%s\n", spDLL->m_spIterator->m_cFileName);
						spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌
					}
					else {
						while (spDLL->m_spIterator->m_spNextSibling != NULL) {   //현재 디렉터리 하위 파일들 모두 검사
							IterateListToRight(spDLL);
							if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) { //이름 비교 시 일치하면 출력
								printf("%s\n", spDLL->m_spIterator->m_cFileName);
								spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌
								break; //while문 밖으로 나감
							}
						}
					}
				}
				spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
				return;
			}
			break;

		case 1:   //옵션 i

			///현재 디렉터리와 상위디렉터리 출력
			printf("%10d  .\n", spDLL->m_spHead->m_iNumofINode);
			iNumOfParentDirectory = FindParentDirectory(spDLL->m_spHead);
			printf("%10d  ..\n", spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_iNumofINode);

			if (iCountNumOfFiles == 0) { //인자로 받은 파일이 없는 경우 "전체 파일 출력"
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL);//현재 디렉터리에서 아래로 이동
					printf("%d  %s\n", spDLL->m_spIterator->m_iNumofINode, spDLL->m_spIterator->m_cFileName);//첫 번째 하위 파일 이름 및 아이노드 넘버 출력
					while (spDLL->m_spIterator->m_spNextSibling != NULL) {//오른쪽으로 이동하며 파일 이름 및 아이노드 넘버 출력
						IterateListToRight(spDLL);
						printf("%d  %s\n", spDLL->m_spIterator->m_iNumofINode, spDLL->m_spIterator->m_cFileName);
					}
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
					return;
				}
				else {
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls 호출을 위해 iterator 원상 복귀
					return; //하위 파일이 없으므로 다시 프롬프트로 돌아감
				}
			}

			else if (iCountNumOfFiles > 0) { //인자로 받은 파일이 있는 경우
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL); //현재 디렉터리에서 아래로 이동
				}
				for (int i = 0; i < iCountNumOfFiles; ++i) {   //총 iCountNumOfFiles의 이름을 출력함
					if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) {
						printf("%d  %s\n", spDLL->m_spIterator->m_iNumofINode, spDLL->m_spIterator->m_cFileName);   //현재 디렉터리 바로 아래 파일과 일치할 경우
						spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌
					}
					else {
						while (spDLL->m_spIterator->m_spNextSibling != NULL) {   //현재 디렉터리 하위 파일들 모두 검사
							IterateListToRight(spDLL);
							if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) { //이름 비교 시 일치하면 출력
								printf("%d  %s\n", spDLL->m_spIterator->m_iNumofINode, spDLL->m_spIterator->m_cFileName);
								spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌
								break; //while문 밖으로 나감
							}
						}
					}
				}
				spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
				return;
			}
			break;

		case 2:   //옵션2
			//현재 디렉터리와 상위디렉터리 출력
			printf("d %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spHead->m_uFileSize, spDLL->m_spHead->m_uYear,
					spDLL->m_spHead->m_uMonth, spDLL->m_spHead->m_uDay, spDLL->m_spHead->m_uHour, spDLL->m_spHead->m_uMinute,
					spDLL->m_spHead->m_uSecond, spDLL->m_spHead->m_cFileName);

			iNumOfParentDirectory = FindParentDirectory(spDLL->m_spHead);
			printf("d %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uFileSize, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uYear,
					spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uMonth, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uDay, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uHour, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uMinute,
					spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uSecond, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_cFileName);


			if (iCountNumOfFiles == 0) { //인자로 받은 파일이 없는 경우 "전체 파일 출력"
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL);//현재 디렉터리에서 아래로 이동

					/////FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
					if (spDLL->m_spIterator->m_uFileType == Directory) {
						cFileType = 'd';
					}
					else if (spDLL->m_spIterator->m_uFileType == File) {
						cFileType = '-';
					}
					///////////////////////////////////////////////////////////////

					printf("%c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
							spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
							spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//첫번째 하위 파일 상세정보 출력
					while (spDLL->m_spIterator->m_spNextSibling != NULL) {//오른쪽으로 이동하며 파일 상세정보 출력
						IterateListToRight(spDLL);

						/////FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
						if (spDLL->m_spIterator->m_uFileType == Directory) {
							cFileType = 'd';
						}
						else if (spDLL->m_spIterator->m_uFileType == File) {
							cFileType = '-';
						}
						///////////////////////////////////////////////////////////////

						printf("%c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
								spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
								spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//���� ���� �������� ����

					}
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
					return;
				}
				else {
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
					return; //하위 파일이 없으므로 다시 프롬프트로 돌아감
				}
			}

			else if (iCountNumOfFiles > 0) { //인자로 받은 파일이 있는 경우
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL); //현재 디렉터리에서 아래로 이동
				}
				for (int i = 0; i < iCountNumOfFiles; ++i) {   //총 iCountNumOfFiles의 이름을 출력함
					if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) {
						//현재 디렉터리 바로 아래 파일과 일치할 경우
						//FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
						if (spDLL->m_spIterator->m_uFileType == Directory) {
							cFileType = 'd';
						}
						else if (spDLL->m_spIterator->m_uFileType == File) {
							cFileType = '-';
						}
						///////////////////////////////////////////////////////////////

						printf("%c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
								spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
								spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//하위 파일 상세 정보 출력

						spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌

					}
					else {
						while (spDLL->m_spIterator->m_spNextSibling != NULL) {   //현재 디렉터리 하위 파일들 모두 검사
							IterateListToRight(spDLL);
							if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) { //이름 비교시 일치하면 출력
								/////FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
								if (spDLL->m_spIterator->m_uFileType == Directory) {
									cFileType = 'd';
								}
								else if (spDLL->m_spIterator->m_uFileType == File) {
									cFileType = '-';
								}
								///////////////////////////////////////////////////////////////

								printf("%c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
										spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
										spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//하위 파일 상세 정보 출력
								spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해 iterator를 맨 앞으로 바꿔줌
								break; //while문 밖으로 나감
							}
						}
					}
				}
				spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator 원상 복귀
				return;
			}
			break;

		case 3: //옵션이 i와 ㅣ일 때

			///현재 디렉터리와 상위디렉터리 출력
			printf("%10d  d %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spHead->m_iNumofINode, spDLL->m_spHead->m_uFileSize, spDLL->m_spHead->m_uYear,
					spDLL->m_spHead->m_uMonth, spDLL->m_spHead->m_uDay, spDLL->m_spHead->m_uHour, spDLL->m_spHead->m_uMinute,
					spDLL->m_spHead->m_uSecond, spDLL->m_spHead->m_cFileName);

			iNumOfParentDirectory = FindParentDirectory(spDLL->m_spHead);
			printf("%10d  d %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_iNumofINode, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uFileSize, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uYear,
					spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uMonth, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uDay, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uHour, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uMinute,
					spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_uSecond, spMyFileSystem->m_spINodeList[iNumOfParentDirectory]->m_cFileName);

			if (iCountNumOfFiles == 0) { //인자로 받은 파일이 없는 경우 "전체 파일 출력"
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL);//현재 디렉터리에서 아래로 이동

					/////FileType이 디렉터리면 'd' 파일이면  '-'를 출력하도록 해줌
					if (spDLL->m_spIterator->m_uFileType == Directory) {
						cFileType = 'd';
					}
					else if (spDLL->m_spIterator->m_uFileType == File) {
						cFileType = '-';
					}
					///////////////////////////////////////////////////////////////

					printf("%d %c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spIterator->m_iNumofINode, cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
							spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
							spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//첫번째 하위 파일 상세정보 출력
					while (spDLL->m_spIterator->m_spNextSibling != NULL) {//오른쪽으로 이동하며 파일 상세정보 출력
						IterateListToRight(spDLL);

						/////FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
						if (spDLL->m_spIterator->m_uFileType == Directory) {
							cFileType = 'd';
						}
						else if (spDLL->m_spIterator->m_uFileType == File) {
							cFileType = '-';
						}
						///////////////////////////////////////////////////////////////

						printf("%d %c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spIterator->m_iNumofINode, cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
								spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
								spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//하위 파일 상세정보 출력

					}
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls호출을 위해 iterator원상 복귀
					return;
				}
				else {
					spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls 호출을 위해 iterator 원상 복귀
					return; //하위 파일이 없으므로 다시 프롬프트로 돌아감
				}
			}

			else if (iCountNumOfFiles > 0) { //인자로 받은 파일이 있는 경우
				if (spDLL->m_spIterator->m_spChild != NULL) {
					IterateListToChild(spDLL); //현재 디렉터리에서 아래로 이동
				}
				for (int i = 0; i < iCountNumOfFiles; ++i) {   //총 iCountNumOfFiles의 이름을 출력함
					if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) {
						//현재 디렉터리 바로 아래 파일과 일치할 경우
						/////FileType이 디렉터리면 'd'  파일이면 '-'를 출력하도록 해줌
						if (spDLL->m_spIterator->m_uFileType == Directory) {
							cFileType = 'd';
						}
						else if (spDLL->m_spIterator->m_uFileType == File) {
							cFileType = '-';
						}
						///////////////////////////////////////////////////////////////

						printf("%d %c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spIterator->m_iNumofINode, cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
								spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
								spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//하위 파일 상세 정보 출력

						spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 다음 i를 위해  iterator를 맨 앞으로 바꿔줌

					}
					else {
						while (spDLL->m_spIterator->m_spNextSibling != NULL) {   //현재 디렉터리 하위 파일들 모두 검사
							IterateListToRight(spDLL);
							if (0 == strcmp(spCommandLine->cp_Other[i], spDLL->m_spIterator->m_cFileName)) { //이름 비교시 일치하면 출력
								/////FileType이 디렉터리면 'd' 파일이면 '-'를 출력하도록 해줌
								if (spDLL->m_spIterator->m_uFileType == Directory) {
									cFileType = 'd';
								}
								else if (spDLL->m_spIterator->m_uFileType == File) {
									cFileType = '-';
								}
								///////////////////////////////////////////////////////////////

								printf("%d %c %10d %4u/%2u/%2u %2u:%2u:%2u %s\n", spDLL->m_spIterator->m_iNumofINode, cFileType, spDLL->m_spIterator->m_uFileSize, spDLL->m_spIterator->m_uYear,
										spDLL->m_spIterator->m_uMonth, spDLL->m_spIterator->m_uDay, spDLL->m_spIterator->m_uHour, spDLL->m_spIterator->m_uMinute,
										spDLL->m_spIterator->m_uSecond, spDLL->m_spIterator->m_cFileName);//하위 파일 상세정보 출력
								spDLL->m_spIterator = spDLL->m_spHead->m_spChild; //for문의 i를 위해 iterator를 맨 앞으로 바꿔줌
								break; //while�� ������ ����
							}
						}
					}
				}
				spDLL->m_spIterator = spDLL->m_spHead;   //다음 ls 호출을 위해 iterator 원상 복귀
				return;
			}
			break;
	}
}

int FindCommandNumber(SCommandLine *spCommandLine) {

	//mymkfs
	if (!(strcmp("mymkfs", (spCommandLine->cCommand)))) {
		SMyFileSystem* spMyFileSystem = (SMyFileSystem*)calloc(1, sizeof(SMyFileSystem));
		spMyFileSystem = CreateFileSystem(spDLL);   
	}

	//myls
	if (!(strcmp("myls", (spCommandLine->cCommand))))
		return 3;

	//mycat
	if (!(strcmp("mycat", (spCommandLine->cCommand))))
		return 4;

	//myshowfile
	if (!(strcmp("myshowfile", (spCommandLine->cCommand))))
		return 5;

	//mypwd
	if (!(strcmp("mypwd", (spCommandLine->cCommand))))
		return 6;

	if (!(strcmp("mycd", (spCommandLine->cCommand))))
		return 7;

	//mycp
	if (!(strcmp("mycp", (spCommandLine->cCommand))))
		return 8;

	//mycpto
	if (!(strcmp("mycpto", (spCommandLine->cCommand))))
		return 9;

	//mycpfrom
	if (!(strcmp("mycpfrom", (spCommandLine->cCommand))))
		return 10;

	//mymkdir
	if (!(strcmp("mymkdir", (spCommandLine->cCommand))))
		return 11;

	//myrmdir
	if (!(strcmp("myrmdir", (spCommandLine->cCommand))))
		return 12;

	//myrm
	if (!(strcmp("myrm", (spCommandLine->cCommand))))
		return 13;

	//mymv
	if (!(strcmp("mymv", (spCommandLine->cCommand))))
		return 14;

	//mytouch
	if (!(strcmp("mytouch", (spCommandLine->cCommand))))
		return 15;
	//fopen(cFileName, "rb");

	//myshowinode
	if (!(strcmp("myshowinode", (spCommandLine->cCommand))))
		return 16;

	//myshowblock
	if (!(strcmp("myshowdatablock", (spCommandLine->cCommand))))
		return 17;

	//mystate
	if (!(strcmp("mystate", (spCommandLine->cCommand))))
		return 18;

	//mytree
	if (!(strcmp("mytree", (spCommandLine->cCommand))))
		return 19;

	//byebye
	if (!(strcmp("byebye", (spCommandLine->cCommand)))) {
		FreeCommandLine(spCommandLine);
		return 20;
	}
}

void ByeBye(SMyFileSystem * spMyFileSystem) {
	FILE *file;
	if ((file = fopen("myfs", "rb+")) == NULL){
		printf("ERROR\n");
		exit(1);
	}
	fwrite(spMyFileSystem, sizeof(SMyFileSystem), 1, file);
	fclose(file);
	free(spMyFileSystem->m_sSuperBlock);
	for (int i = 0; i < MAX_NUM_OF_LIST; i++)
		free(spMyFileSystem->m_spINodeList[i]);

	free(spMyFileSystem->m_spDataBlock[MAX_NUM_OF_DATABLOCK]);
	exit(1);
}

void MyPwd(char * current_add)
{
	printf("%s\n", current_add);
	return;
}

SINode * MyCd(char *current_add, char *address, SINode *Head, SMyFileSystem *spMyFileSystem)
{
	RenewAddress(current_add, address);
	return SearchFileAddress(spMyFileSystem, address,Head);
}

void MyTree(SMyFileSystem* spMyFileSystem, char * address, SINode * current_p)       // MyTree 명령 실행 함수
{
	SINode * parent_p, *inode_p;           // 부모를 가리킬 아이노드 포인터와 이동하면서 파일명을 출력할 아이노드 포인터
	int len = 0;            // 출력을 시작할 디렉터리부터의 깊이 카운트
	_Bool end = 0;          // 트리 구조가 다 출력되었는가 확인

	if (address == NULL) {        // 특정 경로가 들어왔는가? 안들어왔으면(NULL) 출력 시작 디렉터리는 루트
		parent_p = inode_p = spMyFileSystem->m_spINodeList[1];
		printf("/\n");
	}
	else {        // 특정 경로가 들어옴. 경로를 이용한 디렉터리 찾기 함수로 입력받은 디렉터리를 시작 디렉터리로 포인터 지정
		parent_p = inode_p = SearchFileAddress(spMyFileSystem, address, current_p);
		printf("%s\n", inode_p->m_cFileName);
	}

	while (1)        // 하위 파일들이 모두 출력될 때 까지 루프로 반복
	{
		while (1) {        // 현재 포인터하고 있는 디렉터리가 자식을 가지고 있는지 판별 후 이동-출력(이동할 경우 부모도 한 번 내려옴), 자식이 없을 때까지 하위로 내려감
			if (inode_p->m_spChild == NULL)
				break;
			else {
				parent_p = inode_p;
				inode_p = inode_p->m_spChild;
				len++;
				for (int i = 0; i < len; i++)
					printf("--");
				printf("* %s\n", inode_p->m_cFileName);
			}
		}

		while (1) {         // // 더이상 내려갈 하위 디렉터리가 없을 경우 형제를 따라 이동하며 파일명 출력, 더 연결된 형제가 없을 때까지 출력
			if (inode_p->m_spNextSibling == NULL)
				break;
			else {
				inode_p = inode_p->m_spNextSibling;
				for (int i = 0; i < len; i++)
					printf("--");
				printf("* %s\n", inode_p->m_cFileName);
			}
		}

		while (1) {          // 맨 밑 자식들을 모두 출력했을 경우 상위 디렉터리의 형제 파일로 이동하여 다시 자식들 탐색해야함. 아이노드 포인터를 부모로 이동
			inode_p = parent_p;
			len--;
			if (len == 0) {         // 시작 디렉터리로 돌아올 경우 출력하는 루프를 끝내기 위해 bool형에 신호
				end = 1;
				break;
			}
			else
				parent_p = inode_p->m_spParent;              // 부모 포인터를 상위로 이동
			if (inode_p->m_spNextSibling != NULL) {          // 형제가 존재할 경우에만 형제로 이동 형제가 없는 경우 형제를 찾을 때까지 상위로 이동
				inode_p = inode_p->m_spNextSibling;
				break;
			}
		}

		if (end)
			break;
	}
	return;
}

void MyShowFile(char * num1, char * num2, char * file, SINode * head, SMyFileSystem * smMyFileSystem)
{
	SINode * inode_p;                // 출력할 파일의 아이노드 주소
	SDataBlock * datablock_p;        // 출력할 파일의 시작 데이터 블록 주소
	int start, finish;               // 출력할 바이트의 시작과 끝
	int size;                        // 출력하지 않아 생략되는 출력 전 128바이트 개수
	int i;                           // 인덱스 번호 저장
	int cnt = 0;                     // 출력한 문자 카운트

	start = atoi(num1) - 1;                // 1바이트씩 빼서 저장(인덱스가 0부터 시작)
	finish = atoi(num2) - 1;
	inode_p = SearchFileAddress(smMyFileSystem, file, head);              // 출력할 파일의 아이노드 찾기
	datablock_p = inode_p->m_Direct;                                      // 출력할 파일의 데이터 블록 찾기

	size = start / 128;              // 출력 전 생략되는 128바이트의 개수
	for (int i = 0; i < size; i++)   // 개수만큼 다음 데이터 블록으로 이동
		datablock_p = datablock_p->m_spNext;

	start -= size * 128;             // 이동한 데이터블록만큼 바이트 빼주기
	finish -= size * 128;

	i = start;                       // 첫 인덱스 번호 저장

	while (1)
	{
		if (cnt == finish - start)   // 입력해야할 개수와 카운트가 같으면 끝내기
			break;

		putchar(datablock_p->m_cData[i]);   // 한 글자씩 출력

		i++;                                // 인덱스 +1
		cnt++;                              // 출력한 문자 +1

		if (i == 128)                       // // 한 데이터블록의 127까지 모두 출력했으면 다음 데이터 블록으로 이동
		{
			datablock_p = datablock_p->m_spNext;
			i = 0;
		}
	}
	return;
}

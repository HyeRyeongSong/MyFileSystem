#include "FindPath.h"

SINode * SearchFileAddress(SMyFileSystem* spMyFileSystem, char * address, SINode * current_p)       //�ش� ���� ���� ����Ʈ �Լ�
{
	SINode * inode_p;            // ���̳��� ������
	char * tmp_address;          // �Է¹��� �ּ� ���� ���ڿ� (strtok��, ���� �Ҵ��ؾ���)
	char * file;                 // ã�ƾ� �� �Ϲ� ���� �� �����͸� �ӽ� ���� ���ڿ�

	if(address == NULL)
	{
		return current_p;
	}

	tmp_address = (char *)calloc(1, sizeof(address));            // �Է¹��� �ּҸ�ŭ �����Ҵ�
	strcpy(tmp_address, address);                             // �ּ� ����

	if (tmp_address[0] == '/')        // �Է¹��� �ּҰ� ��Ʈ���� ������ ���� inode �����ʹ� ��Ʈ�� ����Ŵ
		inode_p = spMyFileSystem->m_spINodeList[1];
	else                              // �̿��� �Է¹��� �ּҴ� ���� �����͸��� ����Ŵ
		inode_p = current_p;

	file = strtok(tmp_address, "/");     // ó������ ã�� ���� �̸� ��������

	while (file)       // file�� NULL�� ����ų ������ �ݺ�(���̻� ã�� ������ ���� ���� ����)
	{
		inode_p = inode_p->m_spChild;           // ������ ã�� ���� ù° �ڽ� ���� ���̳��� ����Ŵ (�����͸� ������ ã�ƾ��� ������ ã�� ���� �غ�)

		if (strcmp(file, ".."))                 // �θ� �����͸��� �̵��ؾ��� ����: inode �����͸� �θ��� �̵�
		{
			while (inode_p->m_spPrevSibling != NULL)  //�θ��� ����Ű�� �ִ� ù° �ڽı��� �̵��� �� �θ��� �̵��ؾ���
				inode_p = inode_p->m_spPrevSibling;

			inode_p = inode_p->m_spParent;
		}
		else if (strcmp(file, "."))            // ���� �����͸��� �̵��ؾ��� ����: inode ������ ��ȭ����.
			;
		else {                                  // �ش� ���ϸ��� ���� inode�� �����͸� �̵��ؾ��� ����, ���� ������ũ�帮��Ʈ�� ���� ã��
			while (1)
			{
				if (strcmp(inode_p->m_cFileName, file) == 0)
					break;
				else
					inode_p = inode_p->m_spNextSibling;
			}
		}
		if(inode_p == NULL)
			break;
			
		file = strtok(NULL, "/");        // ���� ã�ƾ� �� ���ϸ� ��������
	}

	free(tmp_address);

	return inode_p;     //�Է� ���� �ּ��� ������ ������ ���̳��� �ּ� ����
}



/*���� ���� ���� ����*/
void RenewAddress(char * current_add, char * address)
{
	char * tmp_address;          // �Է¹��� �ּ� ���� ���ڿ� (strtok��, ���� �Ҵ��ؾ���)
	char * file;                 // ���� ���ο� �߰��� ���ϸ�
	int cnt = 0, cnt_m;          // current_add�� ���� ǥ�� (������ ���� ������ ����)

	tmp_address = (char *)calloc(1, sizeof(address));            // �Է¹��� ���θ�ŭ �����Ҵ�
	strcpy(tmp_address, address);                                // ���� ����

	if (tmp_address[0] == '/')         // ���� ���ΰ� ��Ʈ���� ������ ����
	{
		for (int i = 0; current_add[i] != '\0'; i++)       // �̹� �����ϰ� �ִ� ���� ���� ��Ʈ�� �ʱ�ȭ
			current_add[i] = 0;
		current_add[cnt] = '/';
		cnt++;
	}
	else
		cnt = strlen(current_add);       // �ʱ�ȭ���� �ʿ䰡 ���ٸ� ���� ���ο� ������ ���� ������ ��ȣ�� ����

	file = strtok(tmp_address, "/");       // ó�� ã�ƾ��� ���ϸ� �ޱ�

	while (file)
	{

		if (strcmp(file, "..") == 0)            //�θ� �����͸��� ����
		{
			if (cnt > 1)             // ���� ���ΰ� ��Ʈ�� �ƴ� ���� (���̻� �ö��� �θ� �����͸��� ���� X)
			{
				for (int i = cnt - 1; current_add[i] != '/'; i--)            // ���ο� �����ϰ� �ִ� �ֱ� (������) �����͸� �̸� ����
				{
					current_add[i] = 0;
					cnt_m++;
				}
				cnt -= cnt_m;     // ���� ���� ��ȣ ������(�����͸� �̸� ���̸�ŭ ����)
				cnt_m = 0;
			}
		}
		else if (strcmp(file, ".") == 0)        //���� �����͸��� ���� ��ȭ ����
			;
		else {
			if (current_add[cnt - 1] != '/') {          // ���ϸ��� �߰��Ǳ� ���� �� ���� ���м�'/' �߰�
				current_add[cnt] = '/';
				cnt++;
			}
			strcat(current_add, file);        // ���ϸ� �ٿ��ֱ�
			cnt += strlen(file);              // ���� ���� ��ȣ ������(�߰��� ���ϸ��� ���̸�ŭ ����)
		}

		file = strtok(NULL, "/");      //���� ã�� ���� �̸� �޾ƿ���

		if (cnt != 1 && current_add[cnt - 1] == '/')      // ���м�'/'�� �ߺ� ǥ�⸦ ����(��Ʈ ����)
		{
			current_add[cnt - 1] = 0;
			cnt--;
		}
	}
	printf("%s\n", current_add);
	return;
}

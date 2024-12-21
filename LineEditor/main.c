#include"LineEditor.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	//��ȡ��������ļ���
	char inputfile[256], outputfile[256];
	getInOutFile(inputfile, outputfile, sizeof(inputfile));

	//�����������
	int lineNum = 1;	//�к�
	int position = 0;	//��������
	long filePos = 0;	//�ļ�ָ��λ��
	int isEndOfFile = 0;//�Ƿ񵽴��ļ�ĩβ
	//��ȡ�ļ�������
	LineNode* head = NULL;
	head = readFileToLine(inputfile, head, &position, &lineNum, &filePos, &isEndOfFile);

	LineNode* page = head;

	displayHelp();
	while (1)
	{
		char command[20] = { 0 };	//�������������20���ַ�
		printf("Enter command: \n");
		fgets(command, sizeof(command), stdin);

		//��������
		int line = 0, line1 = 0, line2 = 0;
		switch (command[0])
		{
		//�в���
		case 'i':
		{
			char content[MAXSIZE] = { 0 };
			if (sscanf_s(command + 1, "%d", &line) == 1 && !checkLineOut(head, &lineNum, line))
			{
				printf("Enter words below:\n");
				fgets(content, sizeof(content), stdin);
				head = insertLine(head, &position, outputfile, line, &lineNum, content);
				page = head;
			}
			break;
		}
		//��ɾ��
		case 'd':
		{
			if (sscanf_s(command, "d%d %d", &line1, &line2) == 2 && !checkDeleteLineOut(head, &lineNum, line1) && !checkDeleteLineOut(head, &lineNum, line2))
			{
				head = deleteLines(head, line1, line2, &position, &lineNum);
			}
			else if (sscanf_s(command, "d%d", &line1) == 1 && !checkDeleteLineOut(head, &lineNum, line1))
			{
				head = deleteLine(head, line1, &position, &lineNum);
			}
			else
			{
				printf("Invalid delete command\n");
			}
			page = head;
			break;
		}
		//�����л�
		case 'n':
		{
			head = switchActiveZone(outputfile, head, &position, isEndOfFile);
			head = readFileToLine(inputfile, head, &position, &lineNum, &filePos, &isEndOfFile);
			page = head;
			break;
		}
		//��ҳ
		case 'p':
		{
			showLine(&page);
			break;
		}
		//��ʾ����
		case '?':
		{
			displayHelp();
			break;
		}
		default:
		{
			printf("Unknown command\n");
			displayHelp();
			break;
		}
		}
	}
	freeLinkList(head);
	freeLinkList(page);
	return 0;
}
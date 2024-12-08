#include"LineEditor.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	//��ȡ�����ļ�����ļ��ļ���
	char inputfile[256], outputfile[256];
	getInOutFile(inputfile, outputfile, sizeof(inputfile));

	//��ȡ�ļ����ڴ�
	int lineNum = 1;	//����к�
	int position = 0;	//��ǻ������λ��
	long filePos = 0;	//����ļ�ָ�룬��ȡλ��
	int isEndOfFile = 0;//����ļ�ĩβ
	//��ʼ���нڵ�
	LineNode* head = NULL;
	head = readFileToLine(inputfile, head, &position, &lineNum, &filePos, &isEndOfFile);

	LineNode* page = head;

	displayHelp();
	while (1)
	{
		char command[20] = { 0 };	//������һ���������20���ַ�
		printf("Enter command: \n");
		fgets(command, sizeof(command), stdin);

		//Ҫ������к�
		int line = 0, line1 = 0, line2 = 0;
		switch (command[0])
		{
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
		case 'd':
		{
			//���line1�Ϸ�line2���Ϸ���ֻɾ��line1һ��
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
		case 'n':
		{
			head = switchActiveZone(outputfile, head, &position, isEndOfFile);
			head = readFileToLine(inputfile, head, &position, &lineNum, &filePos, &isEndOfFile);
			page = head;
			break;
		}
		case 'p':
		{
			showLine(&page);
			break;
		}
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
	return 0;
}
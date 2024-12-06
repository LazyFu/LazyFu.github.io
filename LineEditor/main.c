#include<stdio.h>
#include<stdlib.h>
#include"LineEditor.h"

int main()
{
	//��ȡ�����ļ�����ļ��ļ���
	char inputfile[256], outputfile[256];
	getInOutFile(inputfile, outputfile);

	//��ȡ�ļ����ڴ�
	int* lineNum = 1;	//����к�
	int* position = 1;	//��ǻ������λ��
	long* filePos = 1;	//����ļ�ָ�룬��ȡλ��
	//��ʼ���нڵ�
	LineNode* head = createLineNode();
	head = readFileToLine(inputfile, head, *position, *lineNum, *filePos);

	LineNode* page = head;

	displayHelp();
	while (1)
	{
		char command[20] = { 0 };	//������һ���������20���ַ�
		printf("Enter command: \n");
		fgets(command, sizeof(command), stdin);

		int line = 0, line1 = 0, line2 = 0;
		switch (command[0])
		{
		case 'i':
		{
			char content[MAXSIZE] = { 0 };
			if (sscanf(command + 1, "%d", &line) != NULL)
			{
				fgets(content, sizeof(content), stdin);
				
			}

			break;
		}
		case 'd':

		case 'n':
		{
			head = switchActiveZone(outputfile, head, *position);
			head = readFileToLine(inputfile, head, *position, *lineNum, *filePos);
			page = head;
			showLine(&page);
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
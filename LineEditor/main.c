#include<stdio.h>
#include<stdlib.h>
#include"LineEditor.h"

int main()
{
	//获取输入文件输出文件文件名
	char inputfile[256], outputfile[256];
	getInOutFile(inputfile, outputfile);

	//读取文件到内存
	int* lineNum = 1;	//标记行号
	int* position = 1;	//标记活区最后位置
	long* filePos = 1;	//标记文件指针，读取位置
	//初始化行节点
	LineNode* head = createLineNode();
	head = readFileToLine(inputfile, head, *position, *lineNum, *filePos);

	LineNode* page = head;

	displayHelp();
	while (1)
	{
		char command[20] = { 0 };	//随便假如一行命令最多20个字符
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
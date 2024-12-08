#include"LineEditor.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	//获取输入文件输出文件文件名
	char inputfile[256], outputfile[256];
	getInOutFile(inputfile, outputfile, sizeof(inputfile));

	//读取文件到内存
	int lineNum = 1;	//标记行号
	int position = 0;	//标记活区最后位置
	long filePos = 0;	//标记文件指针，读取位置
	int isEndOfFile = 0;//标记文件末尾
	//初始化行节点
	LineNode* head = NULL;
	head = readFileToLine(inputfile, head, &position, &lineNum, &filePos, &isEndOfFile);

	LineNode* page = head;

	displayHelp();
	while (1)
	{
		char command[20] = { 0 };	//随便假如一行命令最多20个字符
		printf("Enter command: \n");
		fgets(command, sizeof(command), stdin);

		//要输入的行号
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
			//如果line1合法line2不合法，只删除line1一行
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
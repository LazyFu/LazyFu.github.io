#include"LineEditor.h"

//显示帮助
void displayHelp()
{
	printf("Available commands:\n");
	printf("  i<line_number><Enter><text><Enter> - Insert text after the specified line number.\n");
	printf("    Example: i10<Enter>New line text<Enter>\n");
	printf("  d<line_number1>[ <line_number2>]<Enter> - Delete the specified line or range of lines.\n");
	printf("    Example: d10<Enter>\n");
	printf("    Example: d10 14<Enter>\n");
	printf("  n<Enter> - Switch active zone. Write the current active zone to the output file and read the next part from the input file.\n");
	printf("  p<Enter> - Display the next page of the current active zone.\n");
	printf("  ?<Enter> - Show help.\n");
}

//获取输入输出文件名
void getInOutFile(char* input, char* output,int size)
{
	while(1)
	{
		printf("Enter the input file: ");
		if (fgets(input,size,stdin) == NULL)
		{
			perror("Failed to find file\n");
			continue;
		}
		input[strcspn(input, "\n")] = '\0';
		printf("Enter the output file: ");
		if (fgets(output,size,stdin) == NULL)
		{
			perror("Failed to create file\n");
			continue;
		}
		output[strcspn(output, "\n")] = '\0';
		if (strcmp(input, output) == 0)
		{
			perror("Can't use the same filename\n");
			continue;
		}
		//输入有效才跳出循环
		break;
	}
}

//初始化行节点链表
LineNode* createLineNode()
{
	LineNode* newline = (LineNode*)malloc(sizeof(LineNode));
	if (newline == NULL)
	{
		perror("Failed to allocate memory\n");
		exit(1);
	}
	else
	{
		newline->LineNumber = 0;
		newline->content = NULL;
		newline->next = NULL;
	}

	return newline;
}

//读取文件到活区
LineNode* readFileToLine(const char* infile, LineNode* head, int* position, int* lineNum, long* filePos, int* isEndOfFile)
{
	if (infile == NULL || position == NULL || lineNum == NULL || isEndOfFile == NULL)
	{
		perror("Invalid input parameters\n");
		return NULL;
	}
	FILE* file = fopen(infile, "r");
	if (file == NULL)
	{
		perror("Failed to open file\n");
		return NULL;
	}
	// 恢复文件指针位置
	if (filePos != NULL && *filePos > 0)
	{
		fseek(file, *filePos, SEEK_SET);
	}
	//找到链表最后一个空指针，往后追加
	LineNode* current = head;
	if (current != NULL)
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
	}
	char buffer[MAXSIZE];
	while (*position < (ACTIVEMAXLEN - 20) && fgets(buffer, MAXSIZE, file) != NULL)
	{
		LineNode* newline = createLineNode();
		newline->content = _strdup(buffer);
		newline->LineNumber = (*lineNum)++;

		if (head == NULL)
		{
			head = newline;
			current = head;
		}
		else
		{
			current->next = newline;
			current = newline;
		}
		(*position)++;
	}
	//检查是否到达文件末尾
	if (feof(file))
	{
		*isEndOfFile = 1;
	}
	//记录文件指针位置
	if (filePos != NULL)
	{
		*filePos = ftell(file);
	}
	fclose(file);
	printf("File has successfully read into the active zone\n");
	return head;
}

//每次插入输出一行
void outPutLine(const char* outfile, char* content)
{
	FILE* file = fopen(outfile, "a+");
	fprintf(file, "%s", content);
	fclose(file);
}

//检查行号是否在活区
int checkLineOut(LineNode* head, int* lineNum, int line)
{
	if (line<head->LineNumber - 1 || line>*lineNum)
	{
		printf("Incorrect line number,check your input\n");
		return 1;
	}
	return 0;
}

//检查delete的行数是否正确
int checkDeleteLineOut(LineNode* head, int* lineNum, int line)
{
	if (line<head->LineNumber || line>*lineNum)
	{
		return 1;
	}
	return 0;
}

//插入行
LineNode* insertLine(LineNode* head, int* position, const char* outfile, int line, int* lineNum, char content[])
{
	//这种情况不应该直接输出，而是添加到头指针之前，并且使用p显示时保证输出
	//插入活区之前
	/*if (head->LineNumber == line + 1)
	{
		outPutLine(outfile, content);
		LineNode* current = head;
		while (current != NULL)
		{
			current->LineNumber++;
			current = current->next;
		}
	}*/
	//插入后活区溢出
	if (*position + 1 > ACTIVEMAXLEN)
	{
		//插入到活区前一行，直接输出
		if (head->LineNumber == line + 1)
		{
			outPutLine(outfile, content);
			LineNode* current = head;
			while (current != NULL)
			{
				current->LineNumber++;
				current = current->next;
			}
		}
		else
		{
			LineNode* current = head;
			while (current != NULL && current->LineNumber < line)
			{
				current = current->next;
			}
			if (current != NULL)
			{
				LineNode* insert = createLineNode();
				insert->LineNumber = line + 1;
				insert->content = _strdup(content);
				addLineNode(current, insert);
				while (insert->next != NULL)
				{
					insert->next->LineNumber = insert->LineNumber + 1;
					insert = insert->next;
				}
				outPutLine(outfile, head->content);
				LineNode* next = head->next;
				freeLineNode(head);
				head = next;
			}
		}
	}
	//插入后活区未满
	else
	{
		if (head->LineNumber == line + 1)
		{
			LineNode* newhead = createLineNode();
			newhead->LineNumber = line + 1;
			newhead->content = _strdup(content);
			newhead->next = head;
			while (head != NULL)
			{
				head->LineNumber++;
				head = head->next;
			}
			head = newhead;
		}
		else
		{
			LineNode* current = head;
			while (current->LineNumber < line && current != NULL)
			{
				current = current->next;
			}
			LineNode* insert = createLineNode();
			insert->LineNumber = line + 1;
			insert->content = _strdup(content);
			addLineNode(current, insert);
			while (insert->next != NULL)
			{
				insert->next->LineNumber = insert->LineNumber + 1;
				insert = insert->next;
			}
		}
		(*position)++;
	}
	(*lineNum)++;
	return head;
}

LineNode* deleteLines(LineNode* head, int line1, int line2, int* position, int* lineNum)
{
	clearConsole();
	//不能循环删除，因为会每次改变行号
	/*for (int i = line1; i <= line2; i++)
	{
		head = deleteLine(head, i);
	}
	return head;*/
	if (head == NULL)
	{
		return NULL;
	}
	if (line1 >= line2)
	{
		printf("Check your input\n");
		return head;
	}
	//删除头结点
	while (head != NULL && head->LineNumber >= line1 && head->LineNumber <= line2)
	{
		LineNode* next = head->next;
		freeLineNode(head);
		head = next;
	}
	//其他情况
	LineNode* current = head;
	while (current != NULL && current->next != NULL)
	{
		if (current->next->LineNumber >= line1 && current->next->LineNumber <= line2)
		{
			deleteLineNode(current);
		}
		else
		{
			current = current->next;
		}
	}
	//更新剩余节点的行号
	current = head;
	while (current != NULL)
	{
		if (current->LineNumber > line2)
		{
			current->LineNumber -= (line2 - line1 + 1);
		}
		current = current->next;
	}
	(*position) -= (line2 - line1 + 1);
	(*lineNum) -= (line2 - line1 + 1);
	clearConsole();
	return head;
}

LineNode* deleteLine(LineNode* head, int line1, int* position, int* lineNum)
{
	clearConsole();
	if (head == NULL)
	{
		return NULL;
	}
	//删除头结点
	if (head->LineNumber == line1)
	{
		LineNode* next = head->next;
		freeLineNode(head);
		LineNode* new = next;
		while (new != NULL)
		{
			new->LineNumber--;
			new = new->next;
		}
		return next;
	}
	//其他情况
	LineNode* current = head;
	while (current->next != NULL && current->next->LineNumber != line1)
	{
		current = current->next;
	}
	if (current->next != NULL)
	{
		deleteLineNode(current);
		LineNode* new = current->next;
		while (new->next != NULL)
		{
			new->LineNumber--;
			new = new->next;
		}
	}
	(*position)--;
	(*lineNum)--;
	return head;
}

//活区切换
LineNode* switchActiveZone(const char* outfile, LineNode* head, int* position,int isEndOfFile)
{
	if (outfile == NULL)
	{
		perror("Output file is NULL\n");
		return head;
	}
	
	FILE* file = fopen(outfile, "a+");
	if (file == NULL)
	{
		perror("Failed to open output file\n");
		return head;
	}
	LineNode* current = head;
	LineNode* next = NULL;
	int count = 0;

	//活区保留五行其他输出
	while (current != NULL && (isEndOfFile || (current->LineNumber - head->LineNumber) < (*position - 5)))	//current到活区头部的行数
	{
		fprintf(file, "%s", current->content);
		next = current->next;
		freeLineNode(current);
		count++;
		current = next;
	}
	*position -= count;
	clearConsole();
	if (current == NULL)
	{
		printf("All content has been successfully written into the output file.\n\n");
		#ifdef _WIN32
		Sleep(2000);
		#else
		sleep(2);
		#endif
		exit(0);
	}
	else
	{
		printf("Active zone has successfully written into the output file");
	}
	fclose(file);
	return current;	//返回新的链表头结点
}

//两行中间插入一行
void addLineNode(LineNode* head, LineNode* nextLine)
{
	if (head->next == NULL)
	{
		head->next = nextLine;
	}
	else
	{
		nextLine->next = head->next;
		head->next = nextLine;
	}
}

//删除传入结点的下一个结点
void deleteLineNode(LineNode* head)
{
	if (head == NULL || head->next == NULL)
	{
		return;
	}
	LineNode* delete = head->next;
	head->next = delete->next;
	freeLineNode(delete);
}

/*指针的值：指针所在的内存地址
指针的地址：指针变量的地址，一个变量值
双重指针：指向指针地址的指针，指向的是一个变量，它允许你通过间接访问来修改指针的值，即修改指针所指向的内存地址。

#include <stdio.h>
void updatePointer(int** ptr) {
	static int value = 10;
	*ptr = &value; // 修改原始指针的值，使其指向新的地址
}
int main() {
	int* p = NULL;
	printf("Before: %p\n", (void*)p);
	updatePointer(&p); // 传递指针的地址
	printf("After: %p, Value: %d\n", (void*)p, *p);
	return 0;
}

在这个示例中：
	p 是一个指向 int 的指针，初始值为 NULL。
	updatePointer 函数接受一个双重指针 int** ptr，并通过它修改原始指针 p 的值，使其指向新的地址。
	在 main 函数中，调用 updatePointer 函数并传递 p 的地址（即 &p），从而修改 p 的值。
*/

//每次显示20行
void showLine(LineNode** page)
{
	clearConsole();
	int count = 0;
	LineNode* line = *page;
	while (line != NULL && count < 20)
	{
		count++;
		printf("%4d %s\n", line->LineNumber, line->content);
		line = line->next;
	}
	*page = line;

	if (line == NULL)
	{
		printf("All content in the active zone has been displayed.\n");
	}
	else
	{
		printf("Continue? Enter p\n");
	}
}

//清除终端显示
void clearConsole()
{
	//for windows
	system("cls");
	//for linux
	//system("clear");
}

//释放内存
void freeLineNode(LineNode* node)
{
	if (node != NULL)
	{
		free(node->content);
		free(node);
	}
}

void freeLinkList(LineNode* head)
{
	LineNode* current = head;
	while (current != NULL)
	{
		LineNode* next = current->next;
		freeLineNode(current);
		current = next;
	}
}
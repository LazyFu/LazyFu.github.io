#include"LineEditor.h"

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
void getInOutFile(char* input, char* output)
{
	while(1)
	{
		printf("Enter the input file: ");
		if (scanf("%s", input) == NULL)
		{
			perror("Failed to find file\n");
			continue;
		}
		printf("Enter the output file: ");
		if (scanf("%s", output) == NULL)
		{
			perror("Failed to create file\n");
			continue;
		}
		else if (strcmp(input, output) == 0);
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
LineNode* readFileToLine(const char* infile, LineNode* head, int* position, int* lineNum, long* filePos)
{
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
	if (position == NULL)
	{
		perror("Line number mark is NULL\n");
		fclose(file);
		return head;
	}
	while (position < (ACTIVEMAXLEN - 20) && fgets(buffer, MAXSIZE, file) != NULL)
	{
		LineNode* newline = createLineNode();
		newline->content = strdup(buffer);
		newline->LineNumber = lineNum++;

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

void outPutLine(FILE* outfile, char* content)
{
	FILE* file = fopen(outfile, "a+");
	fprintf(file, "%s", content);
}

void insertLine(LineNode* head, int* position, FILE* outfile,int line,char* content)
{
	printf("Enter words blow:\n");
	if (head->LineNumber == line + 1)
	{
		outPutLine(outfile, content);
	}
	else if (position + 1 > MAXSIZE)
	{

	}
	else if (position + 1 <= MAXSIZE)
	{

	}
	else
	{
		printf("Incorrect line number,check your input\n");
	}
}

LineNode* switchActiveZone(const char* outfile, LineNode* head, int* position)
{
	if (outfile == NULL)
	{
		perror("Output file is NULL\n");
		return;
	}
	
	FILE* file = fopen(outfile, 'a+');
	if (file == NULL)
	{
		perror("Failed to open output file\n");
		return;
	}
	LineNode* current = head;
	LineNode* next = NULL;
	int count = 0;
	while (current != NULL && (current->LineNumber - head->LineNumber) <= (position - 5))	//current到活区头部的行数
	{
		fprintf(file, "%s", current->content);
		next = current->next;
		freeLineNode(current);
		count++;
		current = next;
	}
	position -= count;
	fclose(file);
	return current;	//返回新的链表头结点
}

void addLineNode(LineNode* head, LineNode* nextLine)
{
	if (head == NULL)
	{
		head = nextLine;
	}
	else
	{
		LineNode* current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = nextLine;
	}
}

//每次显示20行
void showLine(LineNode** page)
{
	int count = 0;
	LineNode* line = *page;
	while (line != NULL && count <= 20)
	{
		count++;
		printf("%4d %s\n", line->LineNumber, line->content);
		line = line->next;
	}
	*page = line;
	printf("Continue? Enter p\n");
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
		freeLinkNode(current);
		current = next;
	}
}
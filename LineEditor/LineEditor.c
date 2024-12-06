#include"LineEditor.h"

/*ָ���ֵ��ָ�����ڵ��ڴ��ַ
ָ��ĵ�ַ��ָ������ĵ�ַ��һ������ֵ
˫��ָ�룺ָ��ָ���ַ��ָ�룬ָ�����һ����������������ͨ����ӷ������޸�ָ���ֵ�����޸�ָ����ָ����ڴ��ַ��

#include <stdio.h>
void updatePointer(int** ptr) {
	static int value = 10;
	*ptr = &value; // �޸�ԭʼָ���ֵ��ʹ��ָ���µĵ�ַ
}
int main() {
	int* p = NULL;
	printf("Before: %p\n", (void*)p);
	updatePointer(&p); // ����ָ��ĵ�ַ
	printf("After: %p, Value: %d\n", (void*)p, *p);
	return 0;
}

�����ʾ���У�
	p ��һ��ָ�� int ��ָ�룬��ʼֵΪ NULL��
	updatePointer ��������һ��˫��ָ�� int** ptr����ͨ�����޸�ԭʼָ�� p ��ֵ��ʹ��ָ���µĵ�ַ��
	�� main �����У����� updatePointer ���������� p �ĵ�ַ���� &p�����Ӷ��޸� p ��ֵ��
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

//��ȡ��������ļ���
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
		//������Ч������ѭ��
		break;
	}
}

//��ʼ���нڵ�����
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

//��ȡ�ļ�������
LineNode* readFileToLine(const char* infile, LineNode* head, int* position, int* lineNum, long* filePos)
{
	FILE* file = fopen(infile, "r");
	if (file == NULL)
	{
		perror("Failed to open file\n");
		return NULL;
	}

	// �ָ��ļ�ָ��λ��
	if (filePos != NULL && *filePos > 0)
	{
		fseek(file, *filePos, SEEK_SET);
	}

	//�ҵ��������һ����ָ�룬����׷��
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

	//��¼�ļ�ָ��λ��
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
	while (current != NULL && (current->LineNumber - head->LineNumber) <= (position - 5))	//current������ͷ��������
	{
		fprintf(file, "%s", current->content);
		next = current->next;
		freeLineNode(current);
		count++;
		current = next;
	}
	position -= count;
	fclose(file);
	return current;	//�����µ�����ͷ���
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

//ÿ����ʾ20��
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

//����ն���ʾ
void clearConsole()
{
	//for windows
	system("cls");
	//for linux
	//system("clear");
}

//�ͷ��ڴ�
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
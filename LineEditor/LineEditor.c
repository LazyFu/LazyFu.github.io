#include"LineEditor.h"

//��ʾ����
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
	//����Ƿ񵽴��ļ�ĩβ
	if (feof(file))
	{
		*isEndOfFile = 1;
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

//ÿ�β������һ��
void outPutLine(const char* outfile, char* content)
{
	FILE* file = fopen(outfile, "a+");
	fprintf(file, "%s", content);
	fclose(file);
}

//����к��Ƿ��ڻ���
int checkLineOut(LineNode* head, int* lineNum, int line)
{
	if (line<head->LineNumber - 1 || line>*lineNum)
	{
		printf("Incorrect line number,check your input\n");
		return 1;
	}
	return 0;
}

//���delete�������Ƿ���ȷ
int checkDeleteLineOut(LineNode* head, int* lineNum, int line)
{
	if (line<head->LineNumber || line>*lineNum)
	{
		return 1;
	}
	return 0;
}

//������
LineNode* insertLine(LineNode* head, int* position, const char* outfile, int line, int* lineNum, char content[])
{
	//���������Ӧ��ֱ�������������ӵ�ͷָ��֮ǰ������ʹ��p��ʾʱ��֤���
	//�������֮ǰ
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
	//�����������
	if (*position + 1 > ACTIVEMAXLEN)
	{
		//���뵽����ǰһ�У�ֱ�����
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
	//��������δ��
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
	//����ѭ��ɾ������Ϊ��ÿ�θı��к�
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
	//ɾ��ͷ���
	while (head != NULL && head->LineNumber >= line1 && head->LineNumber <= line2)
	{
		LineNode* next = head->next;
		freeLineNode(head);
		head = next;
	}
	//�������
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
	//����ʣ��ڵ���к�
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
	//ɾ��ͷ���
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
	//�������
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

//�����л�
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

	//�������������������
	while (current != NULL && (isEndOfFile || (current->LineNumber - head->LineNumber) < (*position - 5)))	//current������ͷ��������
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
	return current;	//�����µ�����ͷ���
}

//�����м����һ��
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

//ɾ�����������һ�����
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

//ÿ����ʾ20��
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
		freeLineNode(current);
		current = next;
	}
}
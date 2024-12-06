#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ACTIVEMAXLEN 100	//�������100��
#define MAXSIZE 320			//�ļ�ÿ�в�����320���ַ�

//�����нڵ㣬��̬����ÿ������
typedef struct LineNode
{
	int LineNumber;		//�кű�ʶ
	char* content;		//ÿ������
	struct LineNode* next;
}LineNode;

void displayHelp();
void getInOutFile(char* input, char* output);
LineNode* createLineNode();
LineNode* readFileToLine(const char* filename, LineNode* head, int* position, int* lineNum, long* filePos);
LineNode* switchActiveZone(const char* outfile, LineNode* head,int* position);
void addLineNode(LineNode* head, LineNode* nextLine);
void showLine(LineNode** page);
void clearConsole();
void freeLineNode(LineNode* node);
void freeLinkList(LineNode* head);

#endif // LINEEDITOR_H
#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ACTIVEMAXLEN 100	//活区最多100行
#define MAXSIZE 320			//文件每行不超过320个字符

//定义行节点，动态分配每行内容
typedef struct LineNode
{
	int LineNumber;		//行号标识
	char* content;		//每行内容
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
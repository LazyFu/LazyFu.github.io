#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//��������������
typedef struct HuffmanNode
{
    char data;  //�洢����
    int weight; //�洢Ȩ��
    struct HuffmanNode *left;
    struct HuffmanNode *right;
}HufNode, *HufTree;

//�������ȶ���
typedef struct PriorityQueue
{
    HufTree tree;   //�洢�����������
    struct PriorityQueue *next;
}PriorityQueue;

void showhelp();
HufTree CreateNode(char data, int weight);
void InsertHufNode(PriorityQueue **head, HufTree tree);
HufTree PriorityQueueOut(PriorityQueue **head);
HufTree CreateHuffmanTree();
void SaveHufTree(HufTree T,FILE *file);
HufTree LoadHufTree();
void buildCodeTable(HufTree T, char **codeTable, char *code, int depth);
void freeCodeTable(char **codeTable);
void Encode(HufTree T);
void Decode(HufTree T);
void PrintCode();
void PrintHufTree(HufTree T,int depth,FILE *file);

#endif // HUFFMAN_H
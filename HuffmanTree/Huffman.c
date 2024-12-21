#include "Huffman.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//��ʾ������Ϣ
void showhelp()
{
    printf("H:��ʾ������Ϣ\n");
    printf("I:�����ַ������������ַ����ݺ�Ȩ�أ�������������\n");
    printf("E:����\n");
    printf("D:����\n");
    printf("P:��ӡ�����ļ�\n");
    printf("T:��ӡ��������\n");
    printf("Q:�˳�����\n");
}

//���������������
HufTree CreateNode(char data, int weight)
{
    HufTree node=(HufTree)malloc(sizeof(HufNode));
    if(node==NULL)
    {
        perror("�ڴ����ʧ��\n");
        return NULL;
    }
    node->data=data;
    node->weight=weight;
    node->left=NULL;
    node->right=NULL;
    return node;
}

//�������������㵽���ȶ���
void InsertHufNode(PriorityQueue **head, HufTree tree)
{
    PriorityQueue *node=(PriorityQueue *)malloc(sizeof(PriorityQueue));
    node->tree=tree;
    node->next=NULL;
    //Ȩ��С���ڶ�ǰ
    if(*head==NULL||(*head)->tree->weight > tree->weight)
    {
        node->next=*head;
        *head=node;
    }
    else
    {
        PriorityQueue *current=*head;
        while(current->next!=NULL&&current->next->tree->weight <= tree->weight)
        {
            current=current->next;
        }
        node->next=current->next;
        current->next=node;
    }
}

//��С���ȳ��ӣ����ع�������Ҷ�ӽڵ�
HufTree PriorityQueueOut(PriorityQueue **head)
{
    PriorityQueue *node=*head;
    *head=(*head)->next;
    HufTree tree=node->tree;
    free(node);
    return tree;
}

//������������
HufTree CreateHuffmanTree()
{
    int n;
    char data;
    int weight;
    printf("�����ַ�������");
    scanf("%d",&n);
    printf("����ÿ���ַ������ݺ�Ȩ�أ�һ��һ���ַ���Ȩ�أ�\n");
    //�������ݲ������ȶ���
    PriorityQueue *PriorityQueue=NULL;
    for (int i = 0; i < n; i++)
    {
        scanf(" %c %d",&data,&weight);
        HufTree node=CreateNode(data,weight);
        InsertHufNode(&PriorityQueue,node);
    }
    //������������
    while(PriorityQueue!=NULL&&PriorityQueue->next!=NULL)
    {
        HufTree left=PriorityQueueOut(&PriorityQueue);
        HufTree right=PriorityQueueOut(&PriorityQueue);
        HufTree parent=CreateNode('\0',left->weight+right->weight);
        parent->left=left;
        parent->right=right;
        //����С�Ľڵ�ϲ���������ȶ���
        InsertHufNode(&PriorityQueue,parent);
    }
    return PriorityQueueOut(&PriorityQueue);
}

//��������������ļ�
void SaveHufTree(HufTree T,FILE *file)
{
    if(T!=NULL)
    {
        if(T->data!='\0')
        {
            fprintf(file,"%c %d\n",T->data,T->weight);
        }
        SaveHufTree(T->left,file);
        SaveHufTree(T->right,file);
    }
}

//���ļ��е����������
HufTree LoadHufTree()
{
    FILE *file=fopen("hfmtree.txt","r");
    if(file==NULL)
    {
        perror("�޷����ļ�hfmtree\n");
        return NULL;
    }
    char data;
    int weight;
    PriorityQueue *PriorityQueue=NULL;
    while(fscanf(file,"%c %d\n",&data,&weight)!=EOF)
    {
        HufTree node=CreateNode(data,weight);
        InsertHufNode(&PriorityQueue,node);
    }
    //������������
    while(PriorityQueue!=NULL&&PriorityQueue->next!=NULL)
    {
        HufTree left=PriorityQueueOut(&PriorityQueue);
        HufTree right=PriorityQueueOut(&PriorityQueue);
        HufTree parent=CreateNode('\0',left->weight+right->weight);
        parent->left=left;
        parent->right=right;
        //����С�Ľڵ�ϲ���������ȶ���
        InsertHufNode(&PriorityQueue,parent);
    }
    printf("��������������\n");
    fclose(file);
    return PriorityQueueOut(&PriorityQueue);
}

// ���������
void buildCodeTable(HufTree T, char **codeTable, char *code, int depth) 
{
    if (T->left == NULL && T->right == NULL) 
    {
        code[depth] = '\0';
        codeTable[(unsigned char)T->data] = strdup(code);
        return;
    }
    if (T->left != NULL) 
    {
        code[depth] = '0';
        buildCodeTable(T->left, codeTable, code, depth + 1);
    }
    if (T->right != NULL) 
    {
        code[depth] = '1';
        buildCodeTable(T->right, codeTable, code, depth + 1);
    }
}

// �ͷű����
void freeCodeTable(char **codeTable) 
{
    for (int i = 0; i < 256; i++) 
    {
        if (codeTable[i] != NULL) 
        {
            free(codeTable[i]);
        }
    }
}

//����
void Encode(HufTree T)
{
    FILE *file=fopen("ToBeTran.txt","r");
    if(file==NULL)
    {
        perror("�޷����ļ�ToBeTran\n");
        return;
    }
    FILE *out=fopen("CodeFile.txt","w");
    if(out==NULL)
    {
        perror("�޷������ļ�CodeFile\n");
        return;
    }
    // ���������
    char *codeTable[256] = {0};
    char code[256];
    buildCodeTable(T, codeTable, code, 0);
    // ��ȡ�ļ�������
    char data;
    while (fscanf(file, "%c", &data) != EOF) 
    {
        if (codeTable[(unsigned char)data] != NULL) 
        {
            fprintf(out, "%s", codeTable[(unsigned char)data]);
        }
    }
    printf("�������\n");
    fclose(file);
    fclose(out);
    // �ͷű����
    freeCodeTable(codeTable);
}

//����
void Decode(HufTree T)
{
    FILE *file=fopen("CodeFile.txt","r");
    if(file==NULL)
    {
        perror("�޷����ļ�CodeFile\n");
        perror("����Ƿ��Ѿ�����\n");
        return;
    }
    FILE *out=fopen("TextFile.txt","w");
    if(out==NULL)
    {
        perror("�޷������ļ�TextFile\n");
        return;
    }
    char bit;
    HufTree current=T;
    while(fscanf(file,"%c",&bit)!=EOF)
    {
        if(bit=='0')
        {
            current=current->left;
        }
        else if(bit=='1')
        {
            current=current->right;
        }
        if(current->data!='\0')
        {
            fprintf(out,"%c",current->data);
            current=T;
        }
    }
    printf("�������\n");
    fclose(file);
    fclose(out);
}

//��ӡ�����ļ�
void PrintCode()
{
    FILE *file=fopen("CodeFile.txt","r");
    if(file==NULL)
    {
        perror("�޷����ļ�CodeFile\n");
        return;
    }
    FILE *out=fopen("CodePrint.txt","w");
    char bit;
    int count=0;
    while(fscanf(file,"%c",&bit)!=EOF)
    {
        printf("%c",bit);
        count++;
        if(count%50==0)
        {
            printf("\n");
            fprintf(out,"\n");
        }
        fprintf(out,"%c",bit);
    }
    printf("\nCodeFile�����ɣ���д�뵽CodePrint��\n");
    fclose(file);
    fclose(out);
}

//��ӡ��������
void PrintHufTree(HufTree T,int depth,FILE *file)
{
    if(T!=NULL)
    {
        if(T->data!='\0')
        {
            for (int i = 0; i < depth; i++)
            {
                printf("\t");
                fprintf(file,"\t");
            }
            printf("%c %d\n",T->data,T->weight);
            fprintf(file,"%c %d\n",T->data,T->weight);
        }
        PrintHufTree(T->left,depth+1,file);
        PrintHufTree(T->right,depth+1,file);
    }
}
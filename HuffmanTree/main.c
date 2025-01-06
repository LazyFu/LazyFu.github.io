#include "Huffman.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    HufTree T = NULL;
    printf("����������������--------\n");
    showhelp();
    while (1)
    {
        char func[10] = { 0 };
        printf("������ѡ�");
        /*if(fgets(func,sizeof(func),stdin)!=NULL)
        {
            func[strcspn(func,"\n")]='\0';
            printf("��������ǣ�%s\n",func);
        }*/
        fgets(func, sizeof(func), stdin);
        switch (func[0])
        {
        case 'Q':
        {
            return 0;
        }
        case 'H':
        {
            showhelp();
            break;
        }
        case 'I':
        {
            FILE* file = fopen("hfmtree.txt", "w");
            if (file == NULL)
            {
                perror("�޷������ļ�hfmtree\n");
                return 0;
            }
            T = CreateHuffmanTree();
            printf("���������Ѵ���\n");
            //�������������浽�ļ�hfmtree
            SaveHufTree(T, file);
            printf("���������ѱ��浽�ļ�hfmtree\n");
            fclose(file);
            break;
        }
        case 'E':
        {
            if (T == NULL)
            {
                printf("û�����й������������ļ��е���...\n");
                T = LoadHufTree();
            }
            Encode(T);
            break;
        }
        case 'D':
        {
            Decode(T);
            break;
        }
        case 'P':
        {
            PrintCode();
            break;
        }
        case 'T':
        {
            if (T == NULL)
            {
                printf("û�����й������������ļ��е���...\n");
                T = LoadHufTree();
            }
            FILE* file = fopen("TreePrint.txt", "w");
            if (file == NULL)
            {
                perror("�޷������ļ�TreePrint\n");
                return 0;
            }
            int depth = 0;
            PrintHufTree(T, depth, file);
            fclose(file);
            break;
        }
        default:
        {
            showhelp();
            break;
        }
        }
    }
    return 0;
}
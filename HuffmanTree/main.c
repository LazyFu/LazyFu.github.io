#include "Huffman.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    HufTree T=NULL;
    printf("哈夫曼编码译码器--------\n");
    showhelp();
    while (1)
    {
        char func[2]={0};
        printf("请输入选项：");
        fgets(func, sizeof(func), stdin);
        func[1]='\0';
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
            FILE *file=fopen("hfmtree","w");
            if(file==NULL)
            {
                perror("无法创建文件hfmtree\n");
                return 0;
            }
            T=CreateHuffmanTree();
            //将哈夫曼树保存到文件hfmtree
            SaveHufTree(T,file);
            printf("哈夫曼树已保存到文件hfmtree\n");
            fclose(file);
            break;
        }
        case 'E':
        {
            if(T==NULL)
            {
                printf("没有现有哈夫曼树，从文件中导入...\n");
                T=LoadHufTree();
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
            if(T==NULL)
            {
                printf("没有现有哈夫曼树，从文件中导入...\n");
                T=LoadHufTree();
            }
            FILE *file=fopen("TreePrint","w");
            if(file==NULL)
            {
                perror("无法创建文件TreePrint\n");
                return 0;
            }
            PrintHufTree(T,0,file);
            fclose(file);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return 0;
}
//
// Created by 32874 on 2025/2/25.
//

#include <iostream>
#include <cstring>
#include "D:\c_cpp\HuffmanSLN\Header Files\Huffman.h"
using namespace std;
#define SIZE 256

//选择权值最小的两个节点
void Select(HuffmanTree pHT,int nSize,int &s1,int &s2){
    int minValue=0x7fffffff;
    for(int j=1;j<=nSize;j++){
        if(pHT[j].weight<minValue&&pHT[j].parent==0){
            minValue=pHT[j].weight;
            s1=j;
        }
    }
    minValue=0x7fffffff;
    for(int j=1;j<=nSize;j++){
        if(pHT[j].weight<minValue&&pHT[j].parent==0&&j!=s1){
            minValue=pHT[j].weight;
            s2=j;
        }
    }
}
//构建哈夫曼树
int MakeHuffmanTree(HuffmanTree &pHT,int *weight){
    int m=2*SIZE-1;
    for(int i=1;i<=SIZE;i++){
        pHT[i].weight=weight[i-1];
        pHT[i].parent=0;
        pHT[i].lchild=0;
        pHT[i].rchild=0;
    }
    for(int i=SIZE+1;i<=m;i++){
        pHT[i].weight=0;
        pHT[i].parent=0;
        pHT[i].lchild=0;
        pHT[i].rchild=0;
    }

    int s1,s2;
    for(int i=SIZE+1;i<=m;i++){
        Select(pHT,i-1,s1,s2);
        pHT[s1].parent=i;
        pHT[s2].parent=i;
        pHT[i].lchild=s1;
        pHT[i].rchild=s2;
        pHT[i].weight=pHT[s1].weight+pHT[s2].weight;
    }
    return 1;
}
//生成哈夫曼编码
int MakeHuffmanCode(HuffmanTree &pHT,HuffmanCode &pHC){
    char cd[256] = { '\0' };	//记录访问路径
    int cdlen = 0;				//记录当前路径长度

    for (int i = 1; i < 512; i++) {
        pHT[i].weight = 0;	//遍历Huffman树时用做节点的状态标志
    }

    int p = 511;			//根节点
    while (p != 0) {
        //向左
        if (pHT[p].weight == 0) {
            pHT[p].weight = 1;
            if (pHT[p].lchild != 0) {
                p = pHT[p].lchild;
                cd[cdlen++] = '0';
            }
                //登记叶子节点的字符的编码
            else if(pHT[p].rchild==0){
                pHC[p] = (char*)malloc((cdlen + 1) * sizeof(char));
                cd[cdlen] = '\0';
                strcpy(pHC[p], cd);//复制编码
            }
        }
            //向右
        else if (pHT[p].weight == 1) {
            pHT[p].weight = 2;
            //右孩子为叶子节点
            if (pHT[p].rchild != 0) {
                p = pHT[p].rchild;
                cd[cdlen++] = '1';
            }
        }
            //退回父节点，编码长度减一
        else {
            pHT[p].weight = 0;
            p = pHT[p].parent;
            cdlen--;
        }
    }
    return 0;
}


//逆向生成哈夫曼编码
int ReserveHuffmanCode(HuffmanTree &pHT,HuffmanCode &pHC){
    for (int i = 1; i <= SIZE; i++) { // 遍历所有叶子节点
        int current = i;
        int parent = pHT[current].parent;
        char code[256] = {0};
        int code_len = 0;
        // 从叶子节点回溯到根节点
        while (parent != 0) {
            if (pHT[parent].lchild == current) {
                code[code_len++] = '0';
            } else {
                code[code_len++] = '1';
            }
            current = parent;
            parent = pHT[current].parent;
        }

        // 反转编码字符串
        for (int j = 0; j < code_len / 2; j++) {
            char temp = code[j];
            code[j] = code[code_len - 1 - j];
            code[code_len - 1 - j] = temp;
        }

        // 分配内存并存储编码
        pHC[i] = new char[code_len + 1];
        strcpy(pHC[i], code);
        pHC[i][code_len] = '\0';
    }
    for(int i=1;i<=SIZE;i++){
        printf("0x%02X %s\n", i - 1, pHC[i]);
    }
    return 0;
}





void PrintHuffmanCodes(int root,HuffmanTree pHT, HuffmanCode pHC){
    if (root <= 1) return;

    if (pHT[root].lchild == 0 && pHT[root].rchild == 0)
    {
        printf("0x%02X %s\n", root - 1, pHC[root]);
    }
    if (pHT[root].lchild)//访问左孩子
    {
        PrintHuffmanCodes(pHT[root].lchild, pHT, pHC);
    }
    if (pHT[root].rchild)//访问右孩子
    {
        PrintHuffmanCodes(pHT[root].rchild, pHT, pHC);
    }
}

void PrintHuffmanTree(HuffmanTree pHT){
    cout<<"Byte\tWeight\tParent\tLchild\tRchild"<<endl;
    for(int i=1;i<512;i++){
            cout<<"pHT["<<i<<"]\t"<<pHT[i].weight<<"\t"<<pHT[i].parent<<"\t"<<pHT[i].lchild<<"\t"<<pHT[i].rchild<<endl;
    }
}








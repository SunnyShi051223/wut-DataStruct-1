//
// Created by 32874 on 2025/2/25.
//

#ifndef Huffman_h
#define Huffman_h
#include <cstdio>

struct HTNode{
    int weight;
    int parent,lchild,rchild;
};

typedef HTNode *HuffmanTree;
typedef char **HuffmanCode;

void PrintHuffmanCodes(int root,HuffmanTree pHT, HuffmanCode pHC);
void PrintHuffmanTree(HuffmanTree pHT);

void Select(HuffmanTree pHT,int nSize,int &s1,int &s2);

int MakeHuffmanTree(HuffmanTree &pHT,int *weight);
int MakeHuffmanCode(HuffmanTree &pHT,HuffmanCode &pHC);
int ReserveHuffmanCode(HuffmanTree &pHT,HuffmanCode &pHC);
#endif


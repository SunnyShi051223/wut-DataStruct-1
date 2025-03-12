//
// Created by 32874 on 2025/2/25.
//
#include "D:\c_cpp\HuffmanSLN\Header Files\Huffman.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include "D:\c_cpp\HuffmanSLN\Header Files\Compress.h"
#define SIZE 256
using namespace std;

int IniHed(const char *pFilename,HEAD & sHead){
    strcpy(sHead.type,"HUF");
    sHead.length=0;
    for(int i=0;i<SIZE;i++){
        sHead.weight[i]=0;
    }
    FILE  *in = fopen(pFilename,"rb");
    int ch;
    while((ch=getc(in))!=EOF){
        sHead.weight[ch]++;
        sHead.length++;
    }
    fclose(in);
    return 0;
}

int Compress(const char *pFilename){
    FILE *in=fopen(pFilename,"rb");
    if(in==NULL){
        printf("文件打开失败!\n");
    }
    int weight[256]={0};
    int  ch;
    while((ch=getc(in))!=EOF){
        weight[ch]++;
    }
    fclose(in);

    TestWeight(weight);

    HuffmanTree pHT=new HTNode[512];
    MakeHuffmanTree(pHT,weight);

    PrintHuffmanTree(pHT);


    HuffmanCode pHC = new char*[SIZE+1];
    MakeHuffmanCode(pHT,pHC);
    //ReserveHuffmanCode(pHT,pHC);

    PrintHuffmanCodes(511,pHT,pHC);


    int bufferSize=0;
    for(int i=0;i<SIZE;i++){
        bufferSize+=weight[i]*strlen(pHC[i+1]);
    }
    bufferSize=(bufferSize%8)?bufferSize/8+1:bufferSize/8;

    char *pBuffer=NULL;
    pBuffer = new char[bufferSize];
    memset(pBuffer,0,sizeof(char)*bufferSize);
    Encode(pFilename,pHC,pBuffer,bufferSize);
    if(!pBuffer){
        printf("内存分配失败！\n");
        return -1;
    }

    HEAD sHead;
    IniHed(pFilename,sHead);
    cout<<"文件大小："<<sHead.length<<"字节"<<endl;
    int ret=WriteFile(pFilename,sHead,pBuffer,bufferSize);
    cout<<"生成解压文件："<<pFilename<<".huf"<<endl;
    cout<<"压缩后文件大小："<<ret<<"字节"<<endl;
    cout<<"压缩率："<<(double)ret/sHead.length*100<<"%"<<endl;
    delete[] pHT;
    delete[] pHC;
    delete[] pBuffer;
    return 0;
}

//将字符串转换为字节
char Str2byte(const char *pStr){
    char b = 0x00;
    for(int i=0;i<8;i++){
        b=b<<1;
        if(pStr[i]=='1'){
            b=b|0x01;
        }
    }
    return b;
}

int Encode(const char *pFilename,const HuffmanCode pHC,char *pBuffer,const int bufferSize){
    FILE *in=fopen(pFilename,"rb");
    pBuffer = (char *)malloc(bufferSize*sizeof(char));
    if(pBuffer==NULL){
        printf("内存分配失败！\n");
    }
    char ac[SIZE]={0};
    int pos=0;
    int ch;
    while((ch=fgetc(in))!=EOF){
        strcat(ac,pHC[ch+1]);
        while(strlen(ac)>=8){
            pBuffer[pos++]=Str2byte(ac);
            for(int i=0;i<SIZE-8;i++){
                ac[i]=ac[i+8];
            }
        }
    }
    if(strlen(ac)>0){
        pBuffer[pos++]=Str2byte(ac);
    }
    fclose(in);
    return 0;
}
int WriteFile(const char *pFilename,const HEAD sHead,const char *pBuffer,const int bufferSize){
    char filename[256]={0};
    strcpy(filename,pFilename);
    strcat(filename,".huf");
    FILE *out=fopen(filename,"wb");
    fwrite(&sHead,sizeof(HEAD),1,out);
    fwrite(pBuffer,sizeof(char),bufferSize ,out);
    fclose(out);
    int len=sizeof(HEAD)+strlen(pFilename)+bufferSize+1;
    return len;
}




//扩展2，解压
int Decompress(const char *pFilename) {
    FILE *in = fopen(pFilename, "rb");
    if (!in) {
        printf("无法打开文件 %s\n", pFilename);
        return -1;
    }
    HEAD sHead;
    fread(&sHead, sizeof(HEAD), 1, in);
    if (strcmp(sHead.type, "HUF") != 0) {
        printf("无效的压缩文件\n");
        fclose(in);
        return -1;
    }

    // 计算压缩数据大小
    fseek(in, 0, SEEK_END);
    long fileSize = ftell(in);
    fseek(in, sizeof(HEAD), SEEK_SET);
    int compressedSize = fileSize - sizeof(HEAD);
    char *pBuffer = new char[compressedSize];
    fread(pBuffer, 1, compressedSize, in);
    fclose(in);

    HuffmanTree pHT = new HTNode[512];
    MakeHuffmanTree(pHT, sHead.weight);

    char outputFilename[256];
    strcpy(outputFilename, pFilename);
    // 如果文件名以.huf结尾，则去掉
    if (strlen(outputFilename) > 4 && strcmp(outputFilename + strlen(outputFilename) - 4, ".huf") == 0) {
        outputFilename[strlen(outputFilename) - 4] = '\0';
    }
    strcat(outputFilename, "_Decompressed");
    FILE *out = fopen(outputFilename, "wb");
    if (!out) {
        printf("无法创建输出文件\n");
        delete[] pBuffer;
        delete[] pHT;
        return -1;
    }

    int byteIndex = 0;
    int bitIndex = 7; // 从高位开始读取
    int decodedCount = 0;
    int root = 511; // 根节点索引
    int currentNode = root;

    while (decodedCount < sHead.length) {
        if (byteIndex >= compressedSize) break;
        unsigned char byte = pBuffer[byteIndex];
        int bit = (byte >> bitIndex) & 1;
        currentNode = (bit == 0) ? pHT[currentNode].lchild : pHT[currentNode].rchild;
        // 到达叶子节点
        if (pHT[currentNode].lchild == 0 && pHT[currentNode].rchild == 0) {
            fputc(currentNode - 1, out); // 写入字符
            decodedCount++;
            currentNode = root; // 重置到根节点
        }
        bitIndex--;
        if (bitIndex < 0) {
            byteIndex++;
            bitIndex = 7;
        }
    }

    cout<<"文件大小："<<fileSize<<endl;
    cout<<"解压后文件大小："<<decodedCount<<endl;
    cout<<"生成解压文件为："<<pFilename<<"_Decompressed"<<endl;

    fclose(out);
    delete[] pBuffer;
    delete[] pHT;
    return 0;
}

int TestWeight(int *weight){
    cout<<"Byte  Weight"<<endl;
    for(int i=0;i<SIZE;i++){
        printf("0x%02X\t %d\n",i,weight[i]);
    }
    return 0;
}
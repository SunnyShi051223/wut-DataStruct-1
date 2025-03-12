//
// Created by 32874 on 2025/2/25.
//
#ifndef Compress_h
#define Compress_h

struct HEAD{
    char type[4];
    int length;
    int weight[256];
};
int WriteFile(const char *pFilename,const HEAD sHead,const char *pBuffer,const int bufferSize);
int Compress(const char *pFilename);
char Str2byte(const char *pBinStr);
int Encode(const char *pFilename,const HuffmanCode pHC,char *pBuffer,const int bufferSize);
int IniHed(const char *pFilename,HEAD & sHead);

//解压
int Decompress(const char *pFilename);

int TestWeight(int *weight);
#endif

#include <iostream>
#include <cstring>
#include "D:\c_cpp\HuffmanSLN\Header Files\Huffman.h"
#include "D:\c_cpp\HuffmanSLN\Header Files\Compress.h"
using namespace std;
#define SIZE 256

int main() {
    cout<<"=========哈夫曼文件的压缩与解压========="<<endl;
    int choice;
    cout << "请选择操作：1.压缩 2.解压" << endl;
    cin >> choice;
    if (choice == 1) {
        cout << "=========Huffman文件压缩=========" << endl;
        cout << "请输入文件名：";
        char filename[256];
        cin >> filename;
        //D:\Pic.bmp
        if (Compress(filename) == 0) {
            cout << "压缩完成！" << endl;
        } else {
            cout << "压缩失败" << endl;
        }
    }
    else if( choice == 2) {
        cout << "=========Huffman文件解压=========" << endl;
        cout << "请输入文件名：";
        char filename[256];
        cin >> filename;
        //D:\Pic.bmp.huf
        if (Decompress(filename) == 0) {
            cout << "解压完成！" << endl;
        }
        else {
            cout << "解压失败" << endl;
        }
    }
    else {
        cout << "输入错误！程序已退出" << endl;
        return 0;
    }
    cout << "=========感谢使用=========" << endl;
    return 0;
}



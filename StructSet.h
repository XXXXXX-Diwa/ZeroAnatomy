#ifndef STRUCTSET_H_INCLUDED
#define STRUCTSET_H_INCLUDED
#include <iostream>
#include <fstream>
using namespace std;
class RelBgData{       //无需对齐
public:
    uint8_t roomWidth;
    uint8_t roomHeigh;
    uint8_t relCompressedData[0xF218];
    uint32_t length;
    RelBgData();
    ~RelBgData();
    void getRelCompressData(ifstream &inf);
};

class Lz77BgData{      //首位最好与4对齐
public:
    uint32_t bgSize;    //0 = 256x256, 1 = 512x256, 2 = 256x512 只用了一字节
    uint32_t decompressedLen;   //用了三字节
    uint8_t lz77CompressedTileTable[0x1000];
    uint32_t length;
    Lz77BgData();
    ~Lz77BgData();
    void getLz77CompressData(ifstream &inf);
};

struct SpriteData{  //最多24个 结尾FFFFFF 无需对齐 无效的也指向数据FFFFFF
    uint8_t ypos;
    uint8_t xpos;
    uint8_t data;   //Bits 0-3 = Sprite set number + 1 Bits 4-7 = Property
}spdDat[25];


#endif // STRUCTSET_H_INCLUDED

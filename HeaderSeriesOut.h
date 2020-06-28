#ifndef HEADERSERIESOUT_H
#define HEADERSERIESOUT_H

#include "main.h"

struct HeaderInfo{
    uint8_t tileSet;
    uint8_t BG0_Properties;
    uint8_t BG1_Properties;
    uint8_t BG2_Properties;
    uint8_t BG3_Properties;
    uint8_t unuse1[3];
    uint32_t BG0_DataPointer;
    uint32_t BG1_DataPointer;
    uint32_t BG2_DataPointer;
    uint32_t clipDataPointer;
    uint32_t BG3_DataPointer;
    uint8_t BG3_Scrolling;
    uint8_t transparency;
    uint16_t unuse2;
    uint32_t defaultSpriteDataPointer;
    uint8_t defaultSpriteSet;
    uint8_t firstSpriteSetEvent;
    uint16_t unuse3;
    uint32_t firstSpriteDataPointer;
    uint8_t firstSpriteSet;
    uint8_t secondSpriteEvent;
    uint16_t unuse4;
    uint32_t secondSpriteDataPointer;
    uint8_t secondSpriteSet;
    uint8_t mapXCoordinate;
    uint8_t mapYCoordinate;
    uint8_t effect;
    uint8_t effectYPosition;
    uint8_t unuse5;
    uint16_t musicTrackNumber;
};

struct RelBgData{
    uint8_t roomWidth;
    uint8_t roomHeigh;
    uint8_t relCompressedData[]
};

struct Lz77BgData{
    uint32_t bgSize; //0 = 256x256, 1 = 512x256, 2 = 256x512 只用了一字节
    uint32_t decompressedLen;   //用了三字节
    uint8_t lz77CompressedTileTable[];
};

struct SpriteData{  //最多24个 结尾FFFFFF
    uint8_t ypos;
    uint8_t xpos;
    uint8_t data;   //Bits 0-3 = Sprite set number + 1 Bits 4-7 = Property
};

class HeaderSeriesOut{
    public:
    string readRomRoute;
    string HeaderPath;
    uint32_t allAreaHeaderOffsets;      //所有区域的header数据地址的地址
    uint32_t ldrAreaHeaderOffset[4]={0x56480,0x56604,0x60960,0x6CD7C};
    vector<uint32_t>areaHeaderOffset;   //每个区域的header数据地址
    vector<vector<HeaderInfo> > headInfo;   //记录所有区域每个房间的Head数据
    vector<DataListPrint>headSeriesDLP;  //记录每个数据的位置和长度,以便最终打印

    HeaderSeriesOut(string name,string path);
    ~HeaderSeriesOut();
    static string areaName(uint32_t arean);
    void HeadAllDataOut();
};
#endif  //HEADERSERIESOUT_H

#ifndef SCROLLS_H_INCLUDED
#define SCROLLS_H_INCLUDED
#include "main.h"
/**
*每个房间最多16个卷轴,每个区卷轴结束的指针为0x34597C
*对应数据 FF FF FF FF FF FF FF FF FF FF
**/
struct ScrollData{
    uint8_t xPosBegin;
    uint8_t xPosEnd;
    uint8_t yPosBegin;
    uint8_t yPosEnd;
    uint8_t xPosOfBreakable;//0xFF:none
    uint8_t yPosOfBreakable;//0xFF:none
    uint8_t extendedBoundsDirection;//0:left,1:right,2:up,3:down,0xFF:none;
    uint8_t extendedBoundsValue;//0xFF:none;
};
struct ScrollFmt{
    uint8_t roomId;
    uint8_t numberOfScrolls;
    ScrollData SD[16];//最多容纳十六个
};
class Scrolls{
private:
    string readRomRoute;
    string scrollsPath;
public:
    uint32_t ldrAreaScrollPointerTable=0x5849C;
    uint8_t roomPerArea[7];
    uint32_t AreaScrollPointerTableOft;
    map<uint32_t,uint32_t>oftToLen;
    vector<uint32_t>areaScrollDataPointer;
    vector<vector<uint32_t> >ScrollsDataPointer;
    vector<DataListPrint>scrollsDLP;
    Scrolls(string name,string path,uint8_t numOfAreaRoom[]);
    ~Scrolls();
    void ScrollsDataOut();
    void MakeScrollsFolder();

};


#endif // SCROLLS_H_INCLUDED

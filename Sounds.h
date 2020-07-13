#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED
#include "main.h"
/**
*从D导出曲导入G,再从G导出曲,会发现鼓点并不全,还会多出一些无效的鼓点键位
*也许鼓点键数并不能全由Song数据探测(D曲),故本次采用鼓点键位全导出制防万一.
**/


class Sounds{
private:
    struct Header{
        uint8_t amountOfTracks;
        uint8_t unKnow;
        uint8_t songPriority;
        uint8_t echoFeedBack;
        uint32_t voiceGroupOft;
        uint32_t trackDataOft[16];
    };
    struct Track{
        uint32_t HeaderOft;
        uint8_t unuse1;
        uint8_t songGroup1;
        uint8_t unuse2;
        uint8_t songGroup2;
    };
    struct SampleHead{
        uint8_t unuse[3];//为零
        uint8_t loopFlag;
        uint32_t pitchAdj;//Pitch adjustement 频率
        uint32_t loopPoint;//循环相对于数据开始处的偏移
        uint32_t sampleSize;//循环的话,末尾会多一字节
    };
    struct Key{
        uint8_t type;
        uint8_t baseNote;
        uint8_t sweep;
        uint8_t pan;
        uint32_t difoft;
        uint32_t meydamoft;
    };
    string readRom;
    string curPath;
public:
    struct Info{
        uint32_t typeLen;//高字节类型 其余字节长度
        string describe;
    };
    uint32_t ldrTrackTableOft[19]={
        0x2B60,0x2BA8,0x2C0C,0x2F90,0x3788,0x3870,
        0x38D4,0x392C,0x3A64,0x3C1C,0x3C98,0x3D04,
        0x3E10,0x3EF8,0x3F18,0x3FEC,0x4048,0x42A8,0x430C
    };
//    样本地址 00
//    波形地址 01
//    伤害地址 02
//    vg地址   04
//    ms地址   08
//    dp地址   10
//    track    20 记录了到下一个定义或指针位置或结尾的长度
//    track    21 指针位置
//    header   40
//    table    80
    map<uint32_t,Info>pointer;//所有数据的指针
    uint32_t HeaderOft;
    vector<DataListPrint>songdlp;
    Sounds(string rom,string path);
    ~Sounds();
    void MakeSoundSeriesFolder();
    void SoundSeriesDataOut();
};

//struct Key{
//    //0:directSound
//    //1-2:squareWave1-2
//    //3:waveMemory
//    //4:noise
//    //8:Fixed Fred.(ds)/Note off with Noise(sw)
//    unsigned outputDevice:4;
//    //1:reversePlayback(directSound)
//    //4:multiSample
//    //8:drumPart
//    unsigned multiType:4;
//    unsigned baseNote:8;
//    unsigned sweepTime:4;//(squareWave)
//    unsigned sweepShift:4;//(squareWave)
//    unsigned forcedPan:8;//(directSound)
//
//};

#endif // SOUNDS_H_INCLUDED

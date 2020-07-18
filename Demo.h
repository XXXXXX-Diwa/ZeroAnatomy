#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED
#include "main.h"
/**363ce8记录了demo演出的顺序号 60c0c 60cec处ldr
*411510是按键号约定数据 75884处ldr 不考虑导出
*75fdfc记录了demo的状态数据地址,每个长0x280字节 753c8处ldr
*363be8记录了demo的按键数据地址,以地址+长度形式
*,长度为非字节,而是具体类型,似乎无法超过FD个
*每个demo连续两个,60c14处ldr
*第一个为按键的数据,每个4字节,第二个为按键的持续时间,每个两字节
*demo的事件修改在60c2a
**/

class Demo{
private:
    struct Keys{
        uint32_t keyOft;
        uint32_t keyLen;
        uint32_t keySusOft;
        uint32_t susLen;
    };
    string readRom;
    string curPath;
    uint32_t ldrDemoOrder[2]={0x60C0C,0x60CEC};
    uint32_t ldrDemoState=0x753C8;
    uint32_t ldrDemoKeys=0x60C14;
    uint8_t buffer[0x440];
    uint32_t orderOft;
    uint32_t stateOftTableOft;
    uint32_t keysOftTableOft;
    vector<uint32_t>stateOft;
    vector<Keys>keysOft;
public:
    vector<DataListPrint>demodlp;
    Demo(string rom,string path);
    ~Demo();
    void MakeDemoSeriesFolder();
    void DemoDataOut();
};


#endif // DEMO_H_INCLUDED

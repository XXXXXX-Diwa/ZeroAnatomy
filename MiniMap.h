#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "main.h"
/**
*δѹ����ͼƬһ��Ϊ400h�ֽ�����
*��ɫ��һ��Ϊ20h�ֽ�����
*ָ���Ҫ��4����,����str���ܶ�ȡ��ȷ������
**/
class MiniMap{
private:
    string readRom;
    string curPath;
public:
    uint32_t ldrDataPointerTableOft=0x6B174;
    uint32_t ldrGraphicsOft[5]={
        0x6A6B4,0x6C780,0x6C86C,0x6C948,0x6CA38};
    uint32_t ldrPaletteOft=0x6A6A4;
    uint32_t graphicsLen=0x3000;
    uint32_t paletteLen=0xA0;
    uint8_t buffer[0x3000];
    map<uint32_t,vector<uint8_t> >oftToNo;
    vector<DataListPrint>mdlp;
    MiniMap(string rom,string path);
    ~MiniMap();

    void MapDataOut();

};

#endif // MINIMAP_H_INCLUDED

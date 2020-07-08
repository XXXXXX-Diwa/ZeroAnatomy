#ifndef SPRITESERIES_H_INCLUDED
#define SPRITESERIES_H_INCLUDED
#include "main.h"

class SpriteSeries{
public:
    string readRomRoute;
    string spriteSeriesPath;
    char buffer[0x1000];
    uint32_t ldrSpriteGraphicsOft[2]={0xE070,0xE0A8};
    uint32_t ldrSpritePaletteOft[2]={0xE07C,0xE0E8};
    uint32_t ldrPrispriteAIOft[4]={0xD01C,0xD104,0xD228,0xD31C};
    uint32_t ldrSecspriteAIOft[4]={0xCFCC,0xD0C4,0xD1C4,0xD2C8};
    uint32_t ldrSpriteSetOft=0xE060;
    uint8_t numberOfSecsprite=0x4D;
    uint8_t numberOfPrisprite=0xBE;//CEh,但是前10无效
    uint8_t numOfSpriteSet;
    vector<uint32_t>graphicsPointer;
    vector<uint32_t>palettePointer;
    vector<uint32_t>spriteSetPointer;
    vector<DataListPrint>spriteDLP;
    SpriteSeries(string name,string path,uint8_t spritesetNum);
    ~SpriteSeries();
    void SpriteSeriesDataOut();
    void MakeSpriteSeriesFolders();
};


#endif // SPRITESERIES_H_INCLUDED

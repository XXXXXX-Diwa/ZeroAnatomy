#ifndef GENERALDATAOUT_H_INCLUDED
#define GENERALDATAOUT_H_INCLUDED
#include "main.h"
/**因为大小又不会变化,导出似乎没有多大意义,而且实际的数量要多得多(又无处可查),暂略**/
class GeneralDataOut{
public:
    string readRomRoute;
    string genericDataPath;
    uint32_t ldrGenericBgFstGraphicOft=0x563D8;//0x1000 bit
    uint32_t ldrGenericBgSecGraphicOft=0x5633C;//0x1000 bit 母舰式
    uint32_t ldrGenericBgFstPaletteOft=0x563E0;//0x60 bit 指针需要减2
    uint32_t ldrGenericBgSecPaletteOft=0x56344;//0x60 bit 指针需要减2 母舰式
    uint32_t ldrGenericSpriteGraphicOft=0x53C80;//0x3800 bit
    uint32_t ldrGenericSpritePaletteOft=0xCDA8;//0xC0 bit
    uint32_t ldrClipCollisionTypeOft=0x56310;//0xB0 bit
    uint32_t ldrClipCollisionTypeOft2=0x56320;//0x1A0 bit
    uint32_t ldrClipBehaviorTypeOft=0x56314;//0x160 bit
    uint32_t ldrClipBehaviorTypeOft2=0x56328;//0x680 bit
    vector<DataListPrint>genericDLP;

    GeneralDataOut(string name,string path);
    ~GeneralDataOut();


};



#endif // GENERALDATAOUT_H_INCLUDED

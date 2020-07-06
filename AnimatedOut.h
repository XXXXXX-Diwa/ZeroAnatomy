#ifndef ANIMATEDOUT_H_INCLUDED
#define ANIMATEDOUT_H_INCLUDED
#include "main.h"

class AnimatedOut{
public:
    struct AnimatedGpcOrPal{
        uint8_t animatedType;
        uint8_t framesPerState;
        uint8_t numberOfStates;
        uint8_t unused;
        uint32_t gpcOrPalDataPointer;
    };
	string readRomRoute;
	string animatedPath;
    uint8_t tileSetData[0x3000];
    uint32_t ldrTileSetOft=0x5E200;
    uint32_t ldrGraphicsOft=0x5E1F8;
    uint32_t ldrPaletteOft[2]={0x5E320,0X5E484};
    vector<DataListPrint>animatedDLP;
    vector<AnimatedGpcOrPal>agpc;
    vector<AnimatedGpcOrPal>apal;
    void MakeAnimatedOutFolders();
    void AnimatedAllDataOut();
    AnimatedOut(string name,string path);
    ~AnimatedOut();
};


#endif // ANIMATEDOUT_H_INCLUDED

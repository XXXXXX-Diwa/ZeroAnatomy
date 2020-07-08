#ifndef PROJINFO_H_INCLUDED
#define PROJINFO_H_INCLUDED
#include "main.h"
class ProjInfo{
public:
    uint8_t roomsPerArea[7];
    uint8_t numOfTilesets;
    uint8_t numOfAnimaTilesets;
    uint8_t numOfAnimaGfx;
    uint8_t numOfAnimaPalettes;
    uint8_t numOfSpritesets;
    ProjInfo(string name);
    ~ProjInfo();
//    static string RemoveExtension(string s);
};


#endif // PROJINFO_H_INCLUDED

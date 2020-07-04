#ifndef UNCOMPRESS_H_INCLUDED
#define UNCOMPRESS_H_INCLUDED
#include "main.h"

class PaletteData{
public:
    uint8_t data[0x200];
    PaletteData();
    ~PaletteData();
    void getPaletteData(ifstream &inf,uint32_t len);
};

class TileTableData{
public:
    uint32_t length;
    uint16_t data[0x1000];
    TileTableData();
    ~TileTableData();
    void getTileTableData(ifstream &inf);
};
#endif // UNCOMPRESS_H_INCLUDED

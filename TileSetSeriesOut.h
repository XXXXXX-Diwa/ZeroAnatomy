#ifndef TILESETSERIESOUT_H_INCLUDED
#define TILESETSERIESOUT_H_INCLUDED
#include "main.h"
#include "DataException.h"

struct TileSetInfo{
    uint32_t graphicsPointer;
    uint32_t palettePointer;
    uint32_t bg3_graphicsPointer;
    uint32_t tileTablePointer;
    uint8_t animatedTileSetNumber;
    uint8_t animatedPaletteNumber;
    uint16_t unuse;
};

class TileSetSeriesOut{
public :
    string readRomRoute;
    string tileSetPath;
    uint32_t tileSetOffset;
    uint32_t ldrTileSetOffset=0x56250;
    vector<DataListPrint>tileSetSeriesDLP;
    vector<TileSetInfo> tilesets;
    TileSetSeriesOut(string name,string path);
    ~TileSetSeriesOut();
    void MakeTileSetSeriesFolders();
    void TileSetAllDataOut();

};


#endif // TILESETSERIESOUT_H_INCLUDED

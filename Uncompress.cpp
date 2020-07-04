#include "Uncompress.h"

PaletteData::PaletteData(){}
PaletteData::~PaletteData(){}
void PaletteData::getPaletteData(ifstream &inf,uint32_t len)
{
    inf.read((char*)data,len);
}

TileTableData::TileTableData(){}
TileTableData::~TileTableData(){}
void TileTableData::getTileTableData(ifstream &inf){
    inf.read((char*)data,sizeof(data));
    for(uint32_t i=0;i<0x1000;++i){
        if(data[i]==0){
            length=((i+1)>>12)?i*2:(++i)*2;//0x999Ê±½ûÖ¹ÔÙ¼Ó
            break;
        }
    }
}

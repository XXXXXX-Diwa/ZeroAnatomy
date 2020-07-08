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
    length=inf.tellg();
    inf.read((char*)&head,2);
    head>>=8;
    if(head==0){
        map<uint32_t,uint8_t>::iterator it;
        it=originalTileTableHeight.find(length);
        if(it!=originalTileTableHeight.end()){
            head=it->second;
            length=head*128+2;
        }else{
            DataException::DataError
            ("没有高度标记的TileTable数据,同时也找不到默认的高度值!,地址: 0x"+to_string(length));
        }
    }else{
        length=head*128+2;
    }
    head=0x02|(head<<8);
    inf.read((char*)data,length);
}

SpriteSetData::SpriteSetData(){}
SpriteSetData::~SpriteSetData(){}
void SpriteSetData::getSpriteSetData(ifstream &inf){
    inf.read((char*)data,sizeof(data));
    uint8_t i=0;
    for(;i<0x10;++i){
        if(data[i]==0){
            break;
        }
    }
    length=(++i)*2;
}

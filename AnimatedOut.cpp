#include "AnimatedOut.h"

AnimatedOut::AnimatedOut(string name,string path)
            :readRomRoute(name),animatedPath(path){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    uint32_t byte4;
    inf.seekg(ldrTileSetOft,ios::beg);
    inf.read((char*)&byte4,4);
    if((byte4>>25)!=4){
        DataException::DataError("TileSet÷∏’Î“Ï≥£!");
    }
    inf.seekg(byte4,ios::beg);
    while(true){

    }

}
AnimatedOut::~AnimatedOut(){}

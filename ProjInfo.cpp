#include "ProjInfo.h"

ProjInfo::ProjInfo(string name){
    string s="";
    size_t pos=name.find_last_of('.');
    if(pos>0&&pos!=string::npos){
        s=name.substr(0,pos);
    }
    s+=".proj";
    ifstream inf(s,ios::in);
    if(inf.fail()){
        roomsPerArea[0]=0x2A;
        roomsPerArea[1]=0x2A;
        roomsPerArea[2]=0x39;
        roomsPerArea[3]=0x21;
        roomsPerArea[4]=0x14;
        roomsPerArea[5]=0x16;
        roomsPerArea[6]=0x73;
        numOfTilesets=0x4F;
        numOfAnimaTilesets=0x8;
        numOfAnimaGfx=0x26;
        numOfAnimaPalettes=0x12;
        numOfSpritesets=0x72;
    }else{
        string tes;
        for(int i=0;i<9;++i){
            getline(inf,s);//¶ÁÈ¡µÚ¾ÅÐÐ
        }
        tes=s.substr(13,15);
        roomsPerArea[0]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(16,18);
        roomsPerArea[1]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(19,21);
        roomsPerArea[2]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(22,24);
        roomsPerArea[3]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(25,27);
        roomsPerArea[4]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(28,30);
        roomsPerArea[5]=strtol(tes.c_str(),NULL,16);
        tes=s.substr(31);
        roomsPerArea[6]=strtol(tes.c_str(),NULL,16);
        getline(inf,s);
        tes=s.substr(14);
        numOfTilesets=strtol(tes.c_str(),NULL,16);
        getline(inf,s);
        tes=s.substr(18);
        numOfAnimaTilesets=strtol(tes.c_str(),NULL,16);
        getline(inf,s);
        tes=s.substr(13);
        numOfAnimaGfx=strtol(tes.c_str(),NULL,16);
        getline(inf,s);
        tes=s.substr(18);
        numOfAnimaPalettes=strtol(tes.c_str(),NULL,16);
        getline(inf,s);
        tes=s.substr(16);
        numOfSpritesets=strtol(tes.c_str(),NULL,16);
    }
}
ProjInfo::~ProjInfo(){

}

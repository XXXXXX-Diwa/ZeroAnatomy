#include "AnimatedOut.h"
#include "HeaderSeriesOut.h"
bool agpcsort(const AnimatedOut::AnimatedGpcOrPal &a1,const AnimatedOut::AnimatedGpcOrPal &a2){
    return a1.gpcOrPalDataPointer<a2.gpcOrPalDataPointer;
}

bool agpcunique(const AnimatedOut::AnimatedGpcOrPal &a1,const AnimatedOut::AnimatedGpcOrPal &a2){
    return a1.gpcOrPalDataPointer==a2.gpcOrPalDataPointer;
}

AnimatedOut::AnimatedOut(string name,string path,
            uint8_t tilesetNum,uint8_t gfxNum,uint8_t palNum)
            :readRomRoute(name),animatedPath(path),
            numOfTileSet(tilesetNum),numOfGfx(gfxNum),numOfPal(palNum){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    uint32_t byte4,bit32;
    inf.seekg(ldrGraphicsOft,ios::beg);
    inf.read((char*)&byte4,4);
    if((byte4>>25)!=4){
        DataException::DataError("TileSet指针异常!");
    }
    byte4^=0x8000000;
    AnimatedGpcOrPal agop;
    DataListPrint tedlp;
    inf.seekg(byte4,ios::beg);
    for(uint8_t i=0;i<numOfGfx;++i){
        inf.read((char*)&agop,sizeof(agop));
        if(agop.animatedType!=0&&
           agop.animatedType!=1&&
           agop.animatedType!=4){
            bit32=inf.tellg();
            bit32-=sizeof(agop);
            DataException::DataError("AnimatedGfx数据异常!",bit32);
        }
        if(agop.unused!=0||
           (agop.gpcOrPalDataPointer>>25)!=4){
            bit32=inf.tellg();
            bit32-=sizeof(agop);
            DataException::DataError("AnimatedGfx数据异常!",bit32);
        }
        agpc.push_back(agop);
    }
    tedlp.explain="AnimatedGraphicsTable";
    tedlp.offset=byte4;
    tedlp.len=agpc.size()*8;
    animatedDLP.push_back(tedlp);

    inf.seekg(ldrPaletteOft[0],ios::beg);
    inf.read((char*)&byte4,4);
    if((byte4>>25)!=4){
        bit32=inf.tellg();
        bit32-=4;
        DataException::DataError("TileSet指针异常!",bit32);
    }
    byte4^=0x8000000;
    inf.seekg(byte4,ios::beg);
    for(uint8_t i=0;i<numOfPal;++i){
        inf.read((char*)&agop,sizeof(agop));
        if(agop.animatedType!=0&&
           agop.animatedType!=1&&
           agop.animatedType!=2){
            bit32=inf.tellg();
            bit32-=-sizeof(agop);
            DataException::DataError("AnimatedPal数据异常!",bit32);
        }
        if(agop.unused!=0||
           (agop.gpcOrPalDataPointer>>25)!=4){
            bit32=inf.tellg();
            bit32-=-sizeof(agop);
            DataException::DataError("AnimatedPal数据异常!",bit32);
        }
        apal.push_back(agop);
    }
    tedlp.explain="AnimatedPaletteTable";
    tedlp.offset=byte4;
    tedlp.len=apal.size()*8;
    animatedDLP.push_back(tedlp);

    inf.close();
}

void AnimatedOut::MakeAnimatedOutFolders(){
    File::makefolder(animatedPath+"GpcData");
    File::makefolder(animatedPath+"PalData");
}

void AnimatedOut::AnimatedAllDataOut(){
    string tes=animatedPath+"AnimatedGerenalData.asm";
    uint32_t byte4,bit32;
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)<<(int)(ldrTileSetOft^0x8000000)
    <<"\n\t.word AnimatedTileSetTable\n"<<".org 0x"
    <<setw(7)<<(int)(ldrGraphicsOft^0x8000000)
    <<"\n\t.word AnimatedGraphicsTable\n"<<".org 0x"
    <<setw(7)<<(int)(ldrPaletteOft[0]^0x8000000)
    <<"\n\t.word AnimatedPaletteTable\n"<<".org 0x"
    <<setw(7)<<(int)(ldrPaletteOft[1]^0x8000000)
    <<"\n\t.word AnimatedPaletteTable\n"<<endl;
    ouf.close();

    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }

    tes=animatedPath+"AnimatedTileSetData.bin";
    ouf.open(tes,ios::out|ios::binary);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }

    inf.seekg(ldrTileSetOft,ios::beg);
    DataListPrint tedlp;
    inf.read((char*)&byte4,4);
    if((byte4>>25)!=4){
        DataException::DataError("TileSet指针异常!");
    }
    byte4^=0x8000000;
    inf.seekg(byte4,ios::beg);
    bit32=numOfTileSet*0x30;
    inf.read((char*)tileSetData,bit32);
    uint32_t i=2;
    for(;i<=bit32;i+=3){
        if(tileSetData[i]!=0||tileSetData[i-1]!=0){
            bit32=inf.tellg();
            bit32-=i;
            DataException::DataError("AnimatedTileSet数据异常!",bit32);
        }
    }
    i/=0x30;
    i*=0x30;
    tedlp.explain="AnimatedTileSetTable";
    tedlp.offset=byte4;
    tedlp.len=i;
    animatedDLP.push_back(tedlp);
    ouf.write((char*)tileSetData,tedlp.len);
    ouf.close();

    tes=animatedPath+"AnimatedGpcDataTable.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\nAnimatedTileSetTable:"
    <<"\n\t.import \"AnimatedSeries\\AnimatedTileSetData.bin\""
    <<"\nAnimatedGraphicsTable:"<<endl;
    map<string,uint32_t>desToOft;
    ofstream oub;
    tes=animatedPath+"GpcData\\";
    string testr;
    for(uint32_t i=0;i<agpc.size();++i){
        tedlp.explain="AnimatedGraphics"+HeaderSeriesOut::numToHexStr(i,2);
        tedlp.offset=agpc[i].gpcOrPalDataPointer^0x8000000;
        tedlp.len=agpc[i].numberOfStates*0x80;
        if(tedlp.len==0){
            tedlp.len=0x80;
        }
        animatedDLP.push_back(tedlp);
        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<";"<<tedlp.explain
        <<"\n\t.db 0x"<<setw(2)<<(int)agpc[i].animatedType
        <<",0x"<<setw(2)<<(int)agpc[i].framesPerState
        <<",0x"<<setw(2)<<(int)agpc[i].numberOfStates
        <<",0x00\n\t.dw "<<tedlp.explain<<endl;
        desToOft.insert(pair<string,uint32_t>(tedlp.explain,tedlp.offset));
        testr=tes+HeaderSeriesOut::numToHexStr(tedlp.offset,7)+".gpc";
        oub.open(testr,ios::out|ios::binary);
        if(oub.fail()){
            DataException::FileError(testr,0);
        }
        inf.seekg(tedlp.offset,ios::beg);
        inf.read((char*)tileSetData,tedlp.len);
        oub.write((char*)tileSetData,tedlp.len);
        oub.close();
    }
    ouf.close();
    testr=animatedPath+"AnimatedGpcPointerDef.asm";
    ouf.open(testr,ios::out);
    if(ouf.fail()){
        DataException::FileError(testr,0);
    }
    map<string,uint32_t>::iterator it;
    tes="\"AnimatedSeries\\GpcData\\";
    for(it=desToOft.begin();it!=desToOft.end();++it){
        ouf<<".align\n"<<it->first<<":\n\t.import "
        <<tes<<setiosflags(ios::uppercase)<<setfill('0')
        <<setw(7)<<it->second<<".gpc\""<<endl;
    }
    ouf.close();
    desToOft.clear();

    tes=animatedPath+"AnimatedPalDataTable.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\nAnimatedPaletteTable:"<<endl;
    tes=animatedPath+"PalData\\";
    for(uint32_t i=0;i<apal.size();++i){
        tedlp.explain="AnimatedPalette"+HeaderSeriesOut::numToHexStr(i,2);
        tedlp.offset=apal[i].gpcOrPalDataPointer^0x8000000;
        tedlp.len=apal[i].numberOfStates*0x20;
        animatedDLP.push_back(tedlp);
        desToOft.insert(pair<string,uint32_t>(tedlp.explain,tedlp.offset));
        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<";"<<tedlp.explain
        <<"\n\t.db 0x"<<setw(2)<<(int)apal[i].animatedType
        <<",0x"<<setw(2)<<(int)apal[i].framesPerState
        <<",0x"<<setw(2)<<(int)apal[i].numberOfStates
        <<",0x00\n\t.dw "<<tedlp.explain<<endl;
        testr=tes+HeaderSeriesOut::numToHexStr(tedlp.offset,7)+".pal";
        oub.open(testr,ios::out|ios::binary);
        if(oub.fail()){
            DataException::FileError(testr,0);
        }
        inf.seekg(tedlp.offset,ios::beg);
        inf.read((char*)tileSetData,tedlp.len);
        oub.write((char*)tileSetData,tedlp.len);
        oub.close();
    }
    ouf.close();
    testr=animatedPath+"AnimatedPalPointerDef.asm";
    ouf.open(testr,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    tes="\"AnimatedSeries\\PalData\\";
    for(it=desToOft.begin();it!=desToOft.end();++it){
        ouf<<".align\n"<<it->first<<":\n\t.import "
        <<tes<<setiosflags(ios::uppercase)<<setfill('0')
        <<setw(7)<<it->second<<".pal\""<<endl;
    }
    ouf.close();
    inf.close();
}
AnimatedOut::~AnimatedOut(){}

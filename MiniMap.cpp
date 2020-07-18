#include "MiniMap.h"
#include "HeaderSeriesOut.h"
#include "Decompress.h"

MiniMap::MiniMap(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    string tes;
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrDataPointerTableOft,ios::beg);
    uint32_t bit32;
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("迷你地图的数据指针表地址非指针!");
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    DataListPrint tedlp;
    tedlp.explain="MiniMapDataPointerTable";
    tedlp.offset=bit32;
    tedlp.len=44;
    mdlp.push_back(tedlp);
    map<uint32_t,vector<uint8_t> >::iterator it;
    for(uint8_t i=0;i<11;++i){
        tes="MiniMap"+HeaderSeriesOut::numToHexStr(i,2)+"Data";
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            DataException::DataError("迷你地图的数据地址非指针!");
        }
        bit32^=0x8000000;
        it=oftToNo.find(bit32);
        if(it!=oftToNo.end()){
            it->second.push_back(i);
        }else{
            vector<uint8_t>tev(1,i);
            oftToNo.insert(pair<uint32_t,vector<uint8_t> >(bit32,tev));
        }
    }
    inf.close();
}

MiniMap::~MiniMap(){}
void MiniMap::MakeMiniMapSeriesFolder(){
    File::makefolder(curPath+"MapData");
}

void MiniMap::MapDataOut(){
    ifstream inf;
    ofstream ouf,oudef;
    uint32_t bit32,byte4;
    DataListPrint tedlp;
    File::OpenFile(inf,readRom,true);
    //生产主文件
    string tes=curPath+"MiniMapGeneralData.asm";
    File::MakeFile(ouf,tes,false);
    tes="\n\t.word MiniMapGraphicsPointer\n.org 0x";
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)
    <<(int)(ldrDataPointerTableOft|0x8000000)
    <<"\n\t.word MiniMapDataPointerTable\n.org 0x"
    <<setw(7)<<(int)(ldrPaletteOft|0x8000000)
    <<"\n\t.word MiniMapPalettePointer\n.org 0x"
    <<setw(7)<<(int)(ldrGraphicsOft[0]|0x8000000)
    <<tes<<setw(7)<<(int)(ldrGraphicsOft[1]|0x8000000)
    <<tes<<setw(7)<<(int)(ldrGraphicsOft[2]|0x8000000)
    <<tes<<setw(7)<<(int)(ldrGraphicsOft[3]|0x8000000)
    <<tes<<setw(7)<<(int)(ldrGraphicsOft[4]|0x8000000)
    <<"\n\t.word MiniMapGraphicsPointer"<<endl;
    ouf.close();
    //生产数据指针表文件(数据信息已经在构造函数中收取)
    tes=curPath+"MiniMapDataPointerTable.asm";
    File::MakeFile(ouf,tes,false);
    ouf<<".align\nMiniMapDataPointerTable:\n\t"
    <<".word MiniMap00DataPointer\n\t"
    <<".word MiniMap01DataPointer\n\t"
    <<".word MiniMap02DataPointer\n\t"
    <<".word MiniMap03DataPointer\n\t"
    <<".word MiniMap04DataPointer\n\t"
    <<".word MiniMap05DataPointer\n\t"
    <<".word MiniMap06DataPointer\n\t"
    <<".word MiniMap07DataPointer\n\t"
    <<".word MiniMap08DataPointer\n\t"
    <<".word MiniMap09DataPointer\n\t"
    <<".word MiniMap0ADataPointer"<<endl;
    ouf.close();
    /**导出所有的数据文件并打印导入文件**/
    tes=curPath+"MiniMapDataDef.asm";
    File::MakeFile(oudef,tes,false);
    //首先是图片导出
    inf.seekg(ldrGraphicsOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("迷你地图的图片地址非指针!");
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    string testr=HeaderSeriesOut::numToHexStr(bit32,7);
    tes=curPath+testr+".raw";
    File::MakeFile(ouf,tes,true);
    inf.read((char*)buffer,graphicsLen);
    ouf.write((char*)buffer,graphicsLen);
    ouf.close();
    tedlp.explain="MiniMapGraphics";
    tedlp.offset=bit32;
    tedlp.len=graphicsLen;
    mdlp.push_back(tedlp);
    oudef<<".align\nMiniMapGraphicsPointer:\n\t.import "
    <<"\"MiniMapSeries\\"<<testr<<".raw\""<<endl;
    //其次是调色板导出
    inf.seekg(ldrPaletteOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("迷你地图的调色板地址非指针!");
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    testr=HeaderSeriesOut::numToHexStr(bit32,7);
    tes=curPath+testr+".raw";
    File::MakeFile(ouf,tes,true);
    inf.read((char*)buffer,paletteLen);
    ouf.write((char*)buffer,paletteLen);
    ouf.close();
    tedlp.explain="MiniMapPalette";
    tedlp.offset=bit32;
    tedlp.len=paletteLen;
    mdlp.push_back(tedlp);
    oudef<<".align\nMiniMapPalettePointer:\n\t.import "
    <<"\"MiniMapSeries\\"<<testr<<".raw\""<<endl;
    //最后是每个地图数据导出(lz77压缩,每个解压长度为800);
    unique_ptr<Lz77BgData>lz7(new Lz77BgData());//借用bg的lz7压缩
    map<uint32_t,vector<uint8_t> >::iterator it=oftToNo.begin();
    for(;it!=oftToNo.end();++it){
        byte4=it->second.front();
        tedlp.explain="MiniMap"+HeaderSeriesOut::numToHexStr(byte4,2)+"Data";
        tedlp.offset=it->first;
        testr=HeaderSeriesOut::numToHexStr(tedlp.offset,7);
        tes=curPath+"MapData\\"+testr+".lz7";
        File::MakeFile(ouf,tes,true);
        inf.seekg(tedlp.offset,ios::beg);
        lz7->getLz77CompressData(inf,true);
        tedlp.len=lz7->length+4;
        mdlp.push_back(tedlp);
        ouf.write((char*)&lz7->decompressedLen,4);
        ouf.write((char*)&lz7->lz77CompressedTileTable,lz7->length);
        ouf.close();
        byte4=it->second.size();
        for(uint8_t i=1;i<byte4;++i){
            oudef<<".align\nMiniMap"+HeaderSeriesOut::numToHexStr(it->second[i],2)
            <<"DataPointer:"<<endl;
        }
        oudef<<".align\n"<<tedlp.explain
        <<"Pointer:\n\t.import \"MiniMapSeries\\MapData\\"<<testr<<".lz7\""<<endl;
    }
    lz7.reset();
    ouf.close();
    oudef.close();
    inf.close();
}

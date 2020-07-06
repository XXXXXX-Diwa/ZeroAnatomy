#include "TileSetSeriesOut.h"
#include "HeaderSeriesOut.h"
#include "Decompress.h"
#include "Uncompress.h"

TileSetSeriesOut::TileSetSeriesOut(string name,string path)
                    :readRomRoute(name),tileSetPath(path){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }

    string tes=tileSetPath+"TileSetCustomData.asm";
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    uint32_t bit32;
    inf.seekg(ldrTileSetOffset,ios::beg);       //找到TileSet ldr处读取指针
    inf.read((char*)&bit32,4);
    if(bit32>>25!=4){
        DataException::DataError("错误的ROM,TileSet指针异常!");
    }
    tileSetOffset=bit32^0x8000000;
    inf.seekg(tileSetOffset,ios::beg);
    TileSetInfo tetsi;
    DataListPrint tedlp;
    ouf<<".include \"TileSetSeries\\TileSetPointerDef.asm\"\n"
    <<"TileSetTable:"<<endl;
    uint32_t i=0;
    for(;i<=0xFF;++i){
        bit32=inf.tellg();
        inf.read((char*)&tetsi,sizeof(tetsi));
        if((tetsi.graphicsPointer>>25)!=4||
           (tetsi.palettePointer>>25)!=4||
           (tetsi.tileTablePointer>>25)!=4||
           (tetsi.bg3_graphicsPointer>>25)!=4||
            tetsi.unuse!=0){
                break;
            }
        tilesets.push_back(tetsi);
//        tedlp.len=0x14;
//        tedlp.offset=bit32;
        tes="TileSet"+HeaderSeriesOut::numToHexStr(i,2);
//        tedlp.explain=tes+"_Info";
//        tileSetSeriesDLP.push_back(tedlp);
        ouf<<";"<<tes<<"_Info\n\t.word "
        <<tes<<"_Level_Graphic_Pointer\n\t.word "
        <<tes<<"_Palette_Pointer\n\t.word "
        <<tes<<"_BG3_Graphic_Pointer\n\t.word "
        <<tes<<"_Tile_Table_Pointer\n\t.db 0x"
        <<hex<<setiosflags(ios::uppercase)
        <<setfill('0')<<setw(2)
        <<(int)tetsi.animatedTileSetNumber<<",0x"
        <<setw(2)<<(int)tetsi.animatedPaletteNumber
        <<",0x00,0x00\n"<<endl;
    }
    tedlp.explain="TileSetTable";           //原本每个都记录变为全部记录在一个地方
    tedlp.offset=tileSetOffset;
    tedlp.len=(++i)*0x14;
    tileSetSeriesDLP.push_back(tedlp);

    inf.close();
    ouf.close();
}
TileSetSeriesOut::~TileSetSeriesOut(){

}
void TileSetSeriesOut::MakeTileSetSeriesFolders(){
    File::makefolder(tileSetPath+"GpcData");
    File::makefolder(tileSetPath+"PalData");
    File::makefolder(tileSetPath+"TabData");
}
void TileSetSeriesOut::TileSetAllDataOut(){
    string tes=tileSetPath+"TileSetGerenalData.asm";
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".org 0x"<<HeaderSeriesOut::numToHexStr(ldrTileSetOffset^0x8000000,7)
    <<"\n\t.word TileSetTable"<<endl;
    ouf.close();

    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }

    uint32_t bit32;
    string testr[4]=
    {"_Level_Graphic_Pointer","_Palette_Pointer",
    "_BG3_Graphic_Pointer","_Tile_Table_Pointer"};
    string tepath[3]=
    {
        "TileSetSeries\\GpcData\\",
        "TileSetSeries\\PalData\\",
        "TileSetSeries\\TabData\\"
    };
    //打印数据指针定义
    tes=tileSetPath+"TileSetPointerDef.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    map<uint32_t,uint8_t>pointerType;
    map<string,uint32_t>desTopointer;
    map<uint32_t,uint32_t>pointerToLen;
    uint32_t teoft[4];
    string tess[4];
    for(uint32_t i=0;i<tilesets.size();++i){//普通的for循环比迭代器快速
        tes="TileSet"+HeaderSeriesOut::numToHexStr(i,2);
        teoft[0]=tilesets[i].graphicsPointer^0x8000000;
        teoft[1]=tilesets[i].palettePointer^0x8000000;
        teoft[2]=tilesets[i].bg3_graphicsPointer^0x8000000;
        teoft[3]=tilesets[i].tileTablePointer^0x8000000;

        pointerType.insert(pair<uint32_t,uint8_t>(teoft[0],0));
        pointerType.insert(pair<uint32_t,uint8_t>(teoft[1],1));
        pointerType.insert(pair<uint32_t,uint8_t>(teoft[2],0));
        pointerType.insert(pair<uint32_t,uint8_t>(teoft[3],2));

        tess[0]=tes+testr[0];
        tess[1]=tes+testr[1];
        tess[2]=tes+testr[2];
        tess[3]=tes+testr[3];

        desTopointer.insert(pair<string,uint32_t>(tess[0],teoft[0]));
        desTopointer.insert(pair<string,uint32_t>(tess[1],teoft[1]));
        desTopointer.insert(pair<string,uint32_t>(tess[2],teoft[2]));
        desTopointer.insert(pair<string,uint32_t>(tess[3],teoft[3]));

        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<".align\n"<<tess[0]<<":\n\t.import \""
        <<tepath[0]<<setw(7)<<(int)teoft[0]
        <<".gpc\"\n.align\n"<<tess[1]<<":\n\t.import \""
        <<tepath[1]<<setw(7)<<(int)teoft[1]
        <<".pal\"\n.align\n"<<tess[2]<<":\n\t.import \""
        <<tepath[0]<<setw(7)<<(int)teoft[2]
        <<".gpc\"\n.align\n"<<tess[3]<<":\n\t.import \""
        <<tepath[2]<<setw(7)<<(int)teoft[3]<<".tab\""
        <<endl;
    }
    ouf.close();

    map<uint32_t,uint8_t>::iterator it;
    unique_ptr<Lz77BgData>lbd(new Lz77BgData());
    unique_ptr<PaletteData>ptd(new PaletteData());
    unique_ptr<TileTableData>ttd(new TileTableData());

    for(it=pointerType.begin();it!=pointerType.end();++it){
        bit32=it->first;
        switch(it->second){
        case 0:
            tes=tileSetPath+"GpcData\\"+HeaderSeriesOut::numToHexStr(bit32,7)+".gpc";
            break;
        case 1:
            tes=tileSetPath+"PalData\\"+HeaderSeriesOut::numToHexStr(bit32,7)+".pal";
            break;
        case 2:
            tes=tileSetPath+"TabData\\"+HeaderSeriesOut::numToHexStr(bit32,7)+".tab";
            break;
        default:
            DataException::DataError("数据类型参数错误!");
        }
        ouf.open(tes,ios::out|ios::binary);     //创建导出数据文件
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }

        inf.seekg(bit32,ios::beg);  //定位到每个指针的位置
        switch(it->second){
        case 0:
            lbd->getLz77CompressData(inf,true);
            ouf.write((char*)&lbd->decompressedLen,4);
            ouf.write((char*)lbd->lz77CompressedTileTable,lbd->length);
            pointerToLen.insert(pair<uint32_t,uint32_t>(bit32,lbd->length));
            break;
        case 1:
            ptd->getPaletteData(inf,0x1C0);
            ouf.write((char*)ptd->data,0x1C0);
            pointerToLen.insert(pair<uint32_t,uint32_t>(bit32,0x1C0));
            break;
        case 2:
            ttd->getTileTableData(inf);
            ouf.write((char*)&ttd->head,2);
            ouf.write((char*)ttd->data,ttd->length);
            pointerToLen.insert(pair<uint32_t,uint32_t>(bit32,ttd->length));
        }
        ouf.close();
    }
    lbd.reset();
    ptd.reset();
    ttd.reset();
    map<string,uint32_t>::iterator iter;
    DataListPrint tedlp;
    for(iter=desTopointer.begin();iter!=desTopointer.end();++iter){
        tedlp.explain=iter->first;
        tedlp.offset=iter->second;
        tedlp.len=pointerToLen[tedlp.offset];
        tileSetSeriesDLP.push_back(tedlp);
    }
    inf.close();
}

#include "SpriteSeries.h"
#include "Decompress.h"
#include "Uncompress.h"
#include "HeaderSeriesOut.h"
/**
*调用精灵的基础数据的地址太多了,基本都是AI使用
*而且AI的修改导致地址无法统一,故基础数据不考虑导出.
**/

SpriteSeries::SpriteSeries(string name,string path,uint8_t spritesetNum)
            :readRomRoute(name),spriteSeriesPath(path),numOfSpriteSet(spritesetNum){
    //提取图片和调色板
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    uint32_t bit32,byte4;
    DataListPrint tedlp;
    inf.seekg(ldrSpriteGraphicsOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("精灵图片数据Table指针不是地址!",ldrSpriteGraphicsOft[0]);
    }
    bit32^=0x8000000;
    tedlp.explain="SpriteGraphicsTable";
    tedlp.offset=bit32;
    tedlp.len=numberOfPrisprite*4;
    spriteDLP.push_back(tedlp);

    inf.seekg(bit32,ios::beg);
    for(uint32_t i=0;i<numberOfPrisprite;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            byte4=inf.tellg();
            byte4-=4;
            DataException::DataError("精灵图片地址Table中有非指针!",byte4);
        }
        bit32^=0x8000000;
        graphicsPointer.push_back(bit32);
    }

    inf.seekg(ldrSpritePaletteOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("精灵调色板数据起始指针不是地址!",ldrSpritePaletteOft[0]);
    }
    bit32^=0x8000000;
    tedlp.explain="SpritePaletteTable";
    tedlp.offset=bit32;
    tedlp.len=numberOfPrisprite*4;
    spriteDLP.push_back(tedlp);

    inf.seekg(bit32,ios::beg);
    for(uint32_t i=0;i<numberOfPrisprite;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            byte4=inf.tellg();
            byte4-=4;
            DataException::DataError("精灵调色板地址Table中有非指针!",byte4);
        }
        bit32^=0x8000000;
        palettePointer.push_back(bit32);
    }

    inf.seekg(ldrSpriteSetOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("精灵设置数据起始地址不是指针!",ldrSpriteSetOft);
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    tedlp.explain="SpriteSetTable";
    tedlp.offset=bit32;
    tedlp.len=numOfSpriteSet*4;
    spriteDLP.push_back(tedlp);
    for(uint8_t i=0;i<numOfSpriteSet;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            byte4=inf.tellg();
            byte4-=4;
            DataException::DataError("SpriteSetTable中有非指针!",byte4);
        }
        bit32^=0x8000000;
        spriteSetPointer.push_back(bit32);
    }
    inf.close();
}
SpriteSeries::~SpriteSeries(){
}

void SpriteSeries::MakeSpriteSeriesFolders(){
    File::makefolder(spriteSeriesPath+"GpcData");
    File::makefolder(spriteSeriesPath+"PalData");
    File::makefolder(spriteSeriesPath+"SetData");
}

void SpriteSeries::SpriteSeriesDataOut(){
    uint32_t bit32,byte4;
    DataListPrint tedlp;
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    string tes=spriteSeriesPath+"PrispriteAITable.bin";
    //主精灵AI指针Table导出(bin)
    ofstream oudef;
    ofstream ouf(tes,ios::out|ios::binary);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    inf.seekg(ldrPrispriteAIOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("主精灵AITable起始地址不是指针!",ldrPrispriteAIOft[0]);
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    byte4=(numberOfPrisprite+15)*4;
    tedlp.explain="PrispriteAITableData";
    tedlp.offset=bit32;
    tedlp.len=byte4;
    spriteDLP.push_back(tedlp);

    inf.read((char*)buffer,byte4);
    ouf.write((char*)buffer,byte4);
    ouf.close();
    //副精灵AI指针Table导出(bin)
    tes=spriteSeriesPath+"SecspriteAITable.bin";
    ouf.open(tes,ios::out|ios::binary);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    inf.seekg(ldrSecspriteAIOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("副精灵AITable起始地址不是指针!",ldrSecspriteAIOft[0]);
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    byte4=numberOfSecsprite*4;
    tedlp.explain="SecspriteAITableData";
    tedlp.offset=bit32;
    tedlp.len=byte4;
    spriteDLP.push_back(tedlp);

    inf.read((char*)buffer,byte4);
    ouf.write((char*)buffer,byte4);
    ouf.close();

    tes=spriteSeriesPath+"SpriteAITableData.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\nPrispriteAITableData:\n\t.import \"SpriteSeries\\PrispriteAITable.bin\"\
    \nSecspriteAITableData:\n\t.import \"SpriteSeries\\SecspriteAITable.bin\""<<endl;
    ouf.close();

    //统一收集所有数据,调色板的长度根据gfx的长度而定
    map<string,uint32_t>desToOft;
    map<uint32_t,uint32_t>gpcToPal;
    map<uint32_t,uint8_t>oftToType;
    map<uint32_t,uint32_t>oftToLen;

    tes=spriteSeriesPath+"SpriteGpcDataTable.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\n SpriteGpcDataTable:"<<endl;
    tes=spriteSeriesPath+"SpriteGpcPointerDef.asm";
    oudef.open(tes,ios::out);
    if(oudef.fail()){
        DataException::FileError(tes,0);
    }
    string testr="\"SpriteSeries\\GpcData\\";
    for(uint8_t i=0;i<numberOfPrisprite;++i){
        tes="Sprite"+HeaderSeriesOut::numToHexStr(i,2)+"_Graphics";
        desToOft.insert(pair<string,uint32_t>(tes,graphicsPointer[i]));
        //关联图片和调色板
        gpcToPal.insert(pair<uint32_t,uint32_t>(graphicsPointer[i],palettePointer[i]));
        oftToType.insert(pair<uint32_t,uint8_t>(graphicsPointer[i],0));
        oudef<<".align\n"<<tes<<":\n\t.import "<<testr
        <<HeaderSeriesOut::numToHexStr(graphicsPointer[i],7)<<".gpc\""<<endl;
        ouf<<"\t.word "<<tes<<endl;
    }
    ouf.close();
    oudef.close();

    tes=spriteSeriesPath+"SpritePalDataTable.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\n SpritePalDataTable:"<<endl;
    tes=spriteSeriesPath+"SpritePalPointerDef.asm";
    oudef.open(tes,ios::out);
    if(oudef.fail()){
        DataException::FileError(tes,0);
    }
    testr="\"SpriteSeries\\PalData\\";
    for(uint8_t i=0;i<numberOfPrisprite;++i){
        tes="Sprite"+HeaderSeriesOut::numToHexStr(i,2)+"_Palette";
        desToOft.insert(pair<string,uint32_t>(tes,palettePointer[i]));
        oftToType.insert(pair<uint32_t,uint8_t>(palettePointer[i],1));
        oudef<<".align\n"<<tes<<":\n\t.import "<<testr
        <<HeaderSeriesOut::numToHexStr(palettePointer[i],7)<<".pal\""<<endl;
        ouf<<"\t.word "<<tes<<endl;
    }
    ouf.close();
    oudef.close();

    tes=spriteSeriesPath+"SpriteSetDataTable.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\n SpriteSetDataTable:"<<endl;
    tes=spriteSeriesPath+"SpriteSetPointerDef.asm";
    oudef.open(tes,ios::out);
    if(oudef.fail()){
        DataException::FileError(tes,0);
    }
    testr="\"SpriteSeries\\SetData\\";
    for(uint8_t i=0;i<numOfSpriteSet;++i){
        tes="SpriteSet"+HeaderSeriesOut::numToHexStr(i,2);
        desToOft.insert(pair<string,uint32_t>(tes,spriteSetPointer[i]));
        oftToType.insert(pair<uint32_t,uint8_t>(spriteSetPointer[i],2));
        oudef<<".align\n"<<tes<<":\n\t.import "<<testr
        <<HeaderSeriesOut::numToHexStr(spriteSetPointer[i],7)<<".set\""<<endl;
        ouf<<"\t.word "<<tes<<endl;
    }
    ouf.close();
    oudef.close();

    //导出数据文件
    unique_ptr<Lz77BgData>ldb(new Lz77BgData());
    unique_ptr<PaletteData>ptd(new PaletteData());
    unique_ptr<SpriteSetData>ssd(new SpriteSetData());
    bool unPal=true;
    map<uint32_t,uint8_t>::iterator it;
    for(it=oftToType.begin();it!=oftToType.end();++it){
        unPal=true;
        switch(it->second){
        case 0:
            tes=spriteSeriesPath+"GpcData\\"+HeaderSeriesOut::numToHexStr(it->first,7)+".gpc";
            break;
        case 1:
            unPal=false;
            break;//调色板的长度可能未定,所以跳过
        case 2:
            tes=spriteSeriesPath+"SetData\\"+HeaderSeriesOut::numToHexStr(it->first,7)+".set";
            break;
        default:
            DataException::DataError("错误的数据类型!");
        }
        if(unPal){
            ouf.open(tes,ios::out|ios::binary);
            if(ouf.fail()){
                DataException::FileError(tes,0);
            }
        }
        inf.seekg(it->first,ios::beg);
        switch(it->second){
        case 0:
            ldb->getLz77CompressData(inf,true);
            ouf.write((char*)&ldb->decompressedLen,4);
            ouf.write((char*)ldb->lz77CompressedTileTable,ldb->length);
            oftToLen.insert(pair<uint32_t,uint32_t>(it->first,ldb->length+4));
            ouf.close();
            //对于已经知道了图片大小的,跟着导出调色板
            bit32=gpcToPal[it->first];
            byte4=ldb->definelen/0x40;//图片大小转化为调色板大小
            inf.seekg(bit32,ios::beg);
            tes=spriteSeriesPath+"PalData\\"+HeaderSeriesOut::numToHexStr(bit32,7)+".pal";
            ouf.open(tes,ios::out|ios::binary);
            if(ouf.fail()){
                DataException::FileError(tes,0);
            }
            ptd->getPaletteData(inf,byte4);
            ouf.write((char*)ptd->data,byte4);
            oftToLen.insert(pair<uint32_t,uint32_t>(bit32,byte4));
            break;
        case 1:
            break;
        case 2:
            ssd->getSpriteSetData(inf);
            ouf.write((char*)ssd->data,ssd->length);
            oftToLen.insert(pair<uint32_t,uint32_t>(it->first,ssd->length));
        }
        if(unPal){
            ouf.close();
        }
    }
    ldb.reset();
    ptd.reset();
    ssd.reset();

    tes=spriteSeriesPath+"SpriteGeneralData.asm";
    ouf.open(tes,ios::out);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)
    <<(int)(ldrSpriteGraphicsOft[0]|0x8000000)
    <<"\n\t.word SpriteGpcDataTable\n.org 0x"
    <<setw(7)<<(int)(ldrSpriteGraphicsOft[1]|0x8000000)
    <<"\n\t.word SpriteGpcDataTable\n.org 0x"
    <<setw(7)<<(int)(ldrSpritePaletteOft[0]|0x8000000)
    <<"\n\t.word SpritePalDataTable\n.org 0x"
    <<setw(7)<<(int)(ldrSpritePaletteOft[1]|0x8000000)
    <<"\n\t.word SpritePalDataTable\n.org 0x"
    <<setw(7)<<(int)(ldrSpriteSetOft|0x8000000)
    <<"\n\t.word SpriteSetDataTable\n.org 0x"
    <<setw(7)<<(int)(ldrPrispriteAIOft[0]|0x8000000)
    <<"\n\t.word PrispriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrPrispriteAIOft[1]|0x8000000)
    <<"\n\t.word PrispriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrPrispriteAIOft[2]|0x8000000)
    <<"\n\t.word PrispriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrPrispriteAIOft[3]|0x8000000)
    <<"\n\t.word PrispriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrSecspriteAIOft[0]|0x8000000)
    <<"\n\t.word SecspriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrSecspriteAIOft[1]|0x8000000)
    <<"\n\t.word SecspriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrSecspriteAIOft[2]|0x8000000)
    <<"\n\t.word SecspriteAITableData\n.org 0x"
    <<setw(7)<<(int)(ldrSecspriteAIOft[3]|0x8000000)
    <<"\n\t.word SecspriteAITableData"<<endl;
    ouf.close();

    map<string,uint32_t>::iterator its;
    for(its=desToOft.begin();its!=desToOft.end();++its){
        tedlp.offset=its->second;
        tedlp.explain=its->first;
        tedlp.len=oftToLen[its->second];
        spriteDLP.push_back(tedlp);
    }
}

#include "EndingImage.h"
#include "HeaderSeriesOut.h"
#include "Decompress.h"
EndingImage::EndingImage(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    uint32_t bit32;
    fusionPointer.reserve(65);
    zeroPointer.reserve(40);
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrFusionEndingImageTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("Fusion画廊数据Table地址不是指针!");
    }
    FusionEndingImageOftTable=bit32<<5>>5;
    inf.seekg(FusionEndingImageOftTable,ios::beg);
    for(uint32_t i=0;i<65;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            DataException::DataError("Fusion画廊数据地址有非指针!");
        }
        fusionPointer.push_back(bit32<<5>>5);
    }
    inf.seekg(ldrZeroEndingImageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("Zero画廊数据Table地址不是指针!");
    }
    ZeroEndingImageOftTable=bit32<<5>>5;
    inf.seekg(ZeroEndingImageOftTable,ios::beg);
    for(uint32_t i=0;i<40;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            DataException::DataError("Zero画廊数据地址有非指针!");
        }
        zeroPointer.push_back(bit32<<5>>5);
    }
    inf.close();
}
EndingImage::~EndingImage(){}
void EndingImage::MakeEndingImageFolder(){
    File::makefolder(curPath+"Fusion");
    File::makefolder(curPath+"Zero");
}
void EndingImage::EndingImageOut(){
    ifstream inf;
    ofstream ouf,obf,odf;
    DataListPrint tedlp;
    string tes=curPath+"EndingImageGerenal.asm";
    File::OpenFile(inf,readRom,true);
    //非更迁数据文件
    File::MakeFile(ouf,tes,false);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)<<(ldrFusionEndingImageTableOft|0x8000000)
    <<"\n\t.word FusionEndingImageOftTable\n.org 0x"
    <<setw(7)<<(ldrZeroEndingImageTableOft[0]|0x8000000)
    <<"\n\t.word ZeroEndingImageOftTable\n.org 0x"
    <<setw(7)<<(ldrZeroEndingImageTableOft[1]|0x8000000)
    <<"\n\t.word ZeroEndingImageOftTable"<<endl;
    ouf.close();
    //Table文件
    tedlp.explain="FusionEndingImageOftTable";
    tedlp.len=260;
    tedlp.offset=FusionEndingImageOftTable;
    eidlp.reserve(107);
    eidlp.push_back(tedlp);
    tedlp.explain="ZeroEndingImageOftTable";
    tedlp.len=160;
    tedlp.offset=ZeroEndingImageOftTable;
    eidlp.push_back(tedlp);
    tes=curPath+"EndingImageDef.asm";
    tes=curPath+"EndingImageTable.asm";
    File::MakeFile(ouf,tes,false);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<"FusionEndingImageOftTable:";
    unique_ptr<Lz77BgData> eilz7(new Lz77BgData());

    for(uint32_t i=0;i<65;++i){
        tedlp.offset=fusionPointer[i];
        inf.seekg(tedlp.offset,ios::beg);
        tes=HeaderSeriesOut::numToHexStr(tedlp.offset,7);
        if((i+1)%5==0){
            tedlp.explain="Palette_"+tes;
            tedlp.len=0x200;
            eidlp.push_back(tedlp);
            odf<<".align\n"<<tedlp.explain
            <<"\n\t.import \"EndingImageSeries\\Fusion\\"
            <<tedlp.explain<<"\""<<endl;
            ouf<<"\n\t.word "<<tedlp.explain;
            tes=curPath+"Fusion\\"+tedlp.explain;
            File::MakeFile(obf,tes,true);
            inf.read((char*)buffer,0x200);
            obf.write((char*)buffer,0x200);
            obf.close();

        }else{
            tedlp.explain="Lz77Image_"+tes;
            odf<<".align\n"<<tedlp.explain
            <<"\n\t.import \"EndingImageSeries\\Fusion\\"
            <<tedlp.explain<<"\""<<endl;
            ouf<<"\n\t.word "<<tedlp.explain;
            tes=curPath+"Fusion\\"+tedlp.explain;
            File::MakeFile(obf,tes,true);
            eilz7->getLz77CompressData(inf,true);
            cout<<hex<<eilz7->length;
            cin.get();
            obf.write((char*)eilz7->decompressedLen,4);
            obf.write((char*)eilz7->lz77CompressedTileTable,eilz7->length);
            tedlp.len=eilz7->length+4;
            eidlp.push_back(tedlp);
            obf.close();
            cin.get();
        }
    }
    cin.get();
    ouf<<"ZeroEndingImageOftTable:";
    for(uint32_t i=0;i<40;++i){
        tedlp.offset=zeroPointer[i];
        inf.seekg(tedlp.offset,ios::beg);
        tes=HeaderSeriesOut::numToHexStr(tedlp.offset,7);
        if(i>=31){
            tedlp.explain="Palette_"+tes;
            tedlp.len=0x200;
            eidlp.push_back(tedlp);
            odf<<".align\n"<<tedlp.explain
            <<"\n\t.import \"EndingImageSeries\\Zero\\"
            <<tedlp.explain<<"\""<<endl;
            ouf<<"\n\t.word "<<tedlp.explain;
            tes=curPath+"Zero\\"+tedlp.explain;
            File::MakeFile(obf,tes,true);
            inf.read((char*)buffer,0x200);
            obf.write((char*)buffer,0x200);
            obf.close();
        }else{
            tedlp.explain="Lz77Image_"+tes;
            odf<<".align\n"<<tedlp.explain
            <<"\n\t.import \"EndingImageSeries\\Zero\\"
            <<tedlp.explain<<"\""<<endl;
            ouf<<"\n\t.word "<<tedlp.explain;
            tes=curPath+"Zero\\"+tedlp.explain;
            File::MakeFile(obf,tes,true);
            eilz7->getLz77CompressData(inf,true);
            obf.write((char*)eilz7->decompressedLen,4);
            obf.write((char*)eilz7->lz77CompressedTileTable,eilz7->length);
            tedlp.len=eilz7->length+4;
            eidlp.push_back(tedlp);
            obf.close();
        }
    }
    inf.close();
    odf.close();
    eilz7.reset();
}

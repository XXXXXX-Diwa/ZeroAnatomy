#include "Scrolls.h"
#include "HeaderSeriesOut.h"

Scrolls::Scrolls(string name,string path,uint8_t numOfAreaRoom[])
        :readRomRoute(name),scrollsPath(path),roomPerArea{
        numOfAreaRoom[0],numOfAreaRoom[1],numOfAreaRoom[2],
        numOfAreaRoom[3],numOfAreaRoom[4],numOfAreaRoom[5],
        numOfAreaRoom[6]}{
    ifstream inf;
    File::OpenFile(inf,readRomRoute,true);

    inf.seekg(ldrAreaScrollPointerTable,ios::beg);
    uint32_t bit32,byte4;
    uint8_t bit;
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("区卷轴数据指针表地址非指针!");
    }
    bit32^=0x8000000;
    AreaScrollPointerTableOft=bit32;
    for(uint8_t i=0;i<7;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("区卷轴数据指针地址非指针!");
        }
        byte4^=0x8000000;
        areaScrollDataPointer.push_back(byte4);//记录每个区的scroll数据指针集地址
        uint32_t j=0,word,dw;
        vector<uint32_t>tePointers;
        for(;j<=numOfAreaRoom[i];++j){
            inf.seekg(byte4+j*4,ios::beg);//读取每个指针
            inf.read((char*)&word,4);
            if((word>>25)!=4){
                word=inf.tellg();
                word-=4;
                DataException::DataError("卷轴数据地址非指针!",word);
            }
            word^=0x8000000;
            tePointers.push_back(word);//记录每个卷轴指针
            inf.seekg(word,ios::beg);
            inf.read((char*)&dw,4);
            bit=dw>>8;

            if(bit>0x10&&bit!=0xFF){
                dw=inf.tellg();
                dw-=4;
                DataException::DataError("单个房间的卷轴数量超过了16",dw);
            }
            if(dw==0xFFFFFFFF){
                oftToLen.insert(pair<uint32_t,uint32_t>(word,10));
                break;
            }else{
                oftToLen.insert(pair<uint32_t,uint32_t>(word,bit*8+2));
            }
        }
        ScrollsDataPointer.push_back(tePointers);
    }
    inf.close();
}
Scrolls::~Scrolls(){}
void Scrolls::MakeScrollsFolder(){
    File::makefolder(scrollsPath+"SolData");
}
void Scrolls::ScrollsDataOut(){
    ifstream inf;
    File::OpenFile(inf,readRomRoute,true);

    string tes=scrollsPath+"ScrollGeneralData.asm";
    ofstream ouf,oudef,oudefine;
    File::MakeFile(ouf,tes,false);

    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)
    <<(int)(ldrAreaScrollPointerTable|0x8000000)
    <<"\n\t.word AreaScrollPointerTable"<<endl;
    ouf.close();

    tes=scrollsPath+"AreaScrollPointerTable.asm";
    File::MakeFile(ouf,tes,false);

    DataListPrint tedlp;
    tedlp.explain="AreaScrollPointerTable";
    tedlp.offset=AreaScrollPointerTableOft;
    tedlp.len=28;
    scrollsDLP.push_back(tedlp);

    ouf<<".align\nAreaScrollPointerTable:\n\t.word Area00ScrollData"
    <<"\n\t.word Area01ScrollData\n\t.word Area02ScrollData"
    <<"\n\t.word Area03ScrollData\n\t.word Area04ScrollData"
    <<"\n\t.word Area05ScrollData\n\t.word Area06ScrollData"<<endl;
    ouf.close();

    tes=scrollsPath+"ScrollPointerDef.asm";
    File::MakeFile(oudef,tes,false);
    tes=scrollsPath+"ScrollDataDef.asm";
    File::MakeFile(oudefine,tes,false);
    oudefine<<".align"<<endl;

    ScrollFmt slf;
    uint32_t bit32;
    string filePath=scrollsPath+"SolData\\";
    string path="\"ScrollSeries\\SolData\\";
    string testr,endstr="EveryAreaScrollEndData";

    for(uint32_t i=0;i<7;++i){
        bit32=ScrollsDataPointer[i].size()-1;
        oudef<<".align\nArea"<<HeaderSeriesOut::numToHexStr(i,2)<<"ScrollData:"<<endl;
        for(uint32_t j=0;j<bit32;++j){
            tedlp.offset=ScrollsDataPointer[i][j];
            tedlp.len=oftToLen[tedlp.offset];
            inf.seekg(tedlp.offset,ios::beg);
            inf.read((char*)&slf,tedlp.len);
            testr=HeaderSeriesOut::numToHexStr(ScrollsDataPointer[i][j],7)+".sol";
            tes=filePath+testr;
            tedlp.explain=HeaderSeriesOut::areaName(i)+HeaderSeriesOut::numToHexStr(slf.roomId,2)+"_ScrollData";
            scrollsDLP.push_back(tedlp);

            File::MakeFile(ouf,tes,true);
            ouf.write((char*)&slf,tedlp.len);
            ouf.close();

            oudef<<"\t.word "<<tedlp.explain<<endl;
            oudefine<<tedlp.explain<<":\n\t.import "<<path<<testr<<"\""<<endl;
        }
        tedlp.explain=HeaderSeriesOut::areaName(i)+"ScrollPointerTable";
        tedlp.len=bit32*4;
        tedlp.offset=areaScrollDataPointer[i];
        scrollsDLP.push_back(tedlp);//记录每个区的卷轴指针地址 长度 描述
        oudef<<"\t.word "<<endstr<<endl;
    }
    //统一导出区坐标结尾数据
    bit32=ScrollsDataPointer[0].back();
    testr=HeaderSeriesOut::numToHexStr(bit32,7)+".sol";
    tes=filePath+testr;
    tedlp.explain=endstr;
    tedlp.len=0xA;
    tedlp.offset=bit32;
    scrollsDLP.push_back(tedlp);

    oudefine<<endstr<<":\n\t.import "<<path<<testr<<"\""<<endl;
    inf.seekg(bit32,ios::beg);
    inf.read((char*)&slf,0xA);

    File::MakeFile(ouf,tes,true);
    ouf.write((char*)&slf,0xA);
    ouf.close();
    oudef.close();
    oudefine.close();
    inf.close();

}

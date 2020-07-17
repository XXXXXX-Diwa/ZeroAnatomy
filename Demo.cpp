#include "Demo.h"
#include "HeaderSeriesOut.h"

Demo::Demo(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    uint32_t bit32;
    stateOft.reserve(16);
    keysOft.reserve(16);//提前扩容
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrDemoOrder[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("Demo序号表的地址非指针!");
    }
    orderOft=bit32<<5>>5;
    inf.seekg(ldrDemoState,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("Demo状态表地址非指针!");
    }
    stateOftTableOft=bit32<<5>>5;
    inf.seekg(stateOftTableOft,ios::beg);
    inf.read((char*)buffer,0x40);
    for(int i=0;i<0x40;i+=4){
        bit32=*(uint32_t*)&buffer[i];
        if((bit32>>25)!=4){
            bit32=inf.tellg();
            bit32-=4;
            DataException::DataError("Demo状态地址非指针!",bit32);
        }
        bit32^=0x8000000;
        stateOft.push_back(bit32);
    }
    inf.seekg(ldrDemoKeys,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("按键数据地址表非指针!");
    }
    keysOftTableOft=bit32<<5>>5;
    inf.seekg(keysOftTableOft,ios::beg);
    Keys keys[16];
    inf.read((char*)keys,sizeof(keys));
    for(int i=0;i<0x10;++i){
        if((keys[i].keyOft>>25)!=4||(keys[i].keySusOft>>25)!=4){
            bit32=inf.tellg();
            bit32-=0x100;
            bit32+=i*0x10;
            DataException::DataError("按键数据地址非指针!",bit32);
        //说是最多0xFD个不同的按键和按键持续,但是还是有0x110长度数据存在...
        }else if((keys[i].keyLen>>16)!=0||(keys[i].susLen>>16)!=0){
            bit32=inf.tellg();
            bit32-=0x100;
            bit32+=i*0x10;
            DataException::DataError("按键长度数据错误!",bit32);
        }
        keys[i].keyOft^=0x8000000;
        keys[i].keySusOft^=0x8000000;
        keysOft.push_back(keys[i]);
    }
    inf.close();
}
Demo::~Demo(){}
void Demo::DemoDataOut(){
    string tes=curPath+"DemoGeneralData.asm";
    ifstream inf;
    ofstream ouf,odf;
    DataListPrint tedlp;
    File::OpenFile(inf,readRom,true);

    //导出非更迁类数据
    File::MakeFile(ouf,tes,false);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org 0x"<<setw(7)<<(ldrDemoOrder[0]|0x8000000)
    <<"\n\t.word DemoOrderData\n.org 0x"
    <<setw(7)<<(ldrDemoOrder[1]|0x8000000)
    <<"\n\t.word DemoOrderData\n.org 0x"
    <<setw(7)<<(ldrDemoState|0x8000000)
    <<"\n\t.word DemoStateOftTable\n.org 0x"
    <<setw(7)<<(ldrDemoKeys|0x8000000)
    <<"\n\t.word DemoKeysOftTable"<<endl;
    ouf.close();
    //导出更迁类数据
    tes=curPath+"DemoDataDef.asm";
    File::MakeFile(odf,tes,false);
    //首先是次序数据
    inf.seekg(orderOft,ios::beg);
    inf.read((char*)buffer,16);
    tes=curPath+"DemoOrderData.bin";
    File::MakeFile(ouf,tes,true);
    ouf.write((char*)buffer,16);
    tedlp.explain="DemoOrderData";
    tedlp.len=0x10;
    tedlp.offset=orderOft;
    demodlp.push_back(tedlp);
    ouf.close();

    odf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".align\nDemoOrderData:\n\t.import \"DemoSeries\\DemoOrderData.bin\""
    <<endl;

    //然后是状态地址表
    tes=curPath+"DemoStateOftTable.asm";
    File::MakeFile(ouf,tes,false);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".align\nDemoStateOftTable:";
    for(int i=0;i<0x10;++i){
        ouf<<"\n\t.word DemoStateData"<<setw(2)<<i;
    }
    tedlp.explain="DemoStateOftTable";
    tedlp.len=0x40;
    tedlp.offset=stateOftTableOft;
    demodlp.push_back(tedlp);
    ouf.close();

    //状态数据写出
    for(uint32_t i=0;i<stateOft.size();++i){
        tedlp.explain="DemoStateData"+HeaderSeriesOut::numToHexStr(i,2);
        tes=curPath+tedlp.explain+".bin";
        File::MakeFile(ouf,tes,true);
        inf.seekg(stateOft[i],ios::beg);
        inf.read((char*)buffer,0x280);
        ouf.write((char*)buffer,0x280);
        ouf.close();
        tedlp.len=0x280;
        tedlp.offset=stateOft[i];
        demodlp.push_back(tedlp);

        odf<<".align\n"<<tedlp.explain
        <<":\n\t.import \"DemoSeries\\"
        <<tedlp.explain<<".bin\""<<endl;
    }

    //按键数据表
    tes=curPath+"DemoKeysOftTable.asm";
    File::MakeFile(ouf,tes,false);
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".align\nDemoKeysOftTable:";
    for(int i=0;i<16;++i){
        ouf<<"\n\t.word DemoKeysData"<<setw(2)<<i
        <<",0x"<<setw(8)<<keysOft[i].keyLen
        <<"\n\t.word DemoKeysSusData"<<setw(2)<<i
        <<",0x"<<setw(8)<<keysOft[i].susLen;
    }
    ouf.close();

    //按键数据导出
    for(uint32_t i=0;i<keysOft.size();++i){
        tedlp.explain="DemoKeysData"+HeaderSeriesOut::numToHexStr(i,2);
        tes=curPath+tedlp.explain+".bin";

        File::MakeFile(ouf,tes,true);
        tedlp.len=keysOft[i].keyLen*4;
        tedlp.offset=keysOft[i].keyOft;
        demodlp.push_back(tedlp);

        inf.seekg(tedlp.offset,ios::beg);
        inf.read((char*)buffer,tedlp.len);
        ouf.write((char*)buffer,tedlp.len);
        ouf.close();

        odf<<".align\n"<<tedlp.explain
        <<":\n\t.import \"DemoSeries\\"
        <<tedlp.explain<<".bin\""<<endl;

        tedlp.explain="DemoKeysSusData"+HeaderSeriesOut::numToHexStr(i,2);
        tes=curPath+tedlp.explain+".bin";

        File::MakeFile(ouf,tes,true);
        tedlp.len=keysOft[i].susLen*2;
        tedlp.offset=keysOft[i].keySusOft;
        demodlp.push_back(tedlp);

        inf.seekg(tedlp.offset,ios::beg);
        inf.read((char*)buffer,tedlp.len);
        ouf.write((char*)buffer,tedlp.len);
        ouf.close();

        odf<<".align\n"<<tedlp.explain
        <<":\n\t.import \"DemoSeries\\"
        <<tedlp.explain<<".bin\""<<endl;
    }
    inf.close();
    odf.close();
}

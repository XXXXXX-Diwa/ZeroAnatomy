#include "HeaderSeriesOut.h"
/**
*Header���ݷ�Ϊ7������,ÿ�������ÿ�����䶼��һ��Header����60�ֽڳ�
*ÿ��������3��BG��һ��ʵ������Ҫ��ӡ,3��������������Ҫ��ӡ(��������)
**/
HeaderSeriesOut::HeaderSeriesOut(string name,string path):readRomRoute(name),HeaderPath(path){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
//        throw "�޷���ȡ�ļ�: \""+readRomRoute+"\"!";
        File::FileFail(readRomRoute);
    }
    uint32_t bit32,byte4;
    inf.seekg(ldrAreaHeaderOffset[0],ios::beg);
    inf.read((char*)&bit32,4);
    if(bit32>>25!=4){
        throw string("�����ROM,��ַ�쳣!");
    }
    for(int i=1;i<4;++i){
        inf.seekg(ldrAreaHeaderOffset[i],ios::beg);
        inf.read((char*)&byte4,4);
        if(byte4!=bit32){
            throw string("�����ROM,��ַ����ͬ!");
        }
    }
    allAreaHeaderOffsets=bit32^0x8000000;
    inf.seekg(allAreaHeaderOffsets);
    for(int i=0;i<7;++i){
        inf.read((char*)&bit32,4);
        if(bit32>>25!=4){
            bit32=inf.tellg();
            bit32-=4;
            throw "���������ͷ���ݵ�ַ!"+to_string(bit32);
        }
        areaHeaderOffset.push_back(bit32^0x8000000);
    }
    for(uint32_t i=0;i<areaHeaderOffset.size();++i){
        bit32=0xFFFFFFFF;
        for(uint32_t j=0;j<areaHeaderOffset.size();++j){
            if(j==i){
                continue;
            }
            bit32=(bit32>(byte4=areaHeaderOffset[j]-areaHeaderOffset[i]))?byte4:bit32;
        }
        inf.seekg(areaHeaderOffset[i],ios::beg);
        vector<HeaderInfo> teHeadInfo;
        HeaderInfo header;
        DataListPrint teDLP;
        stringstream ss;
        for(uint16_t j=0;j<0x10000;++j){
            byte4=inf.tellg();

            inf.read((char*)&header,sizeof(header));
            if(header.BG0_DataPointer>>25!=4||
                header.BG1_DataPointer>>25!=4||
                header.BG2_DataPointer>>25!=4||
                header.clipDataPointer>>25!=4||
                header.defaultSpriteDataPointer>>25!=4||
                header.firstSpriteDataPointer>>25!=4||
                header.secondSpriteDataPointer>>25!=4||
                header.mapXCoordinate>0x2F||
                header.mapYCoordinate>0x2F||
                header.effect>0xB||
                header.BG3_Scrolling>0xA||
                bit32==byte4-areaHeaderOffset[i]){
                if(teHeadInfo.size()==0){
                    throw string("��ͷ���ݷ���������Ϊ0!");
                }
                headInfo.push_back(teHeadInfo);
                break;
            }
            teHeadInfo.push_back(header);
            teDLP.offset=byte4;
            teDLP.len=0X3C;
            ss<<hex<<setiosflags(ios::uppercase)<<j;
            ss>>teDLP.explain;
            teDLP.explain=HeaderSeriesOut::areaName(i)+" room "+teDLP.explain+" HeaderData";
            headSeriesDLP.push_back(teDLP);//ÿ�ɹ�һ������¼���嵥������
        }
    }
    inf.close();
}

string HeaderSeriesOut::areaName(uint32_t arean)
{
    switch(arean){
        case 0:
        return string("Brinstar");
        case 1:
        return string("Kraid");
        case 2:
        return string("Norfair");
        case 3:
        return string("Ridley");
        case 4:
        return string("Tourian");
        case 5:
        return string("Crateria");
        case 6:
        return string("Chozodia");
        default:
            return string("");
    }

}
HeaderSeriesOut::~HeaderSeriesOut(){}

void HeaderSeriesOut::HeadAllDataOut(){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        File::FileFail(readRomRoute);
    }
    string tes=HeaderPath+"HeadGerenalData.asm";
    //�������ݵ�ַ
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        File::FileFail(tes);
    }
    for(int i=0;i<4;++i){
        ouf<<".org 0x" <<hex<<setiosflags(ios::uppercase)
        <<(ldrAreaHeaderOffset[i]^0x8000000)<<endl;
        ouf<<"\t.word allAreaHeaderOffsets\t\t\t;"<<hex
        <<setiosflags(ios::uppercase)<<allAreaHeaderOffsets<<endl;
    }
    ouf.close();
    ouf.clear();

    tes=HeaderPath+"HeadCustomData.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        File::FileFail(tes);
    }
    tes=HeaderPath+"HeadPointerDef.asm";
    ofstream odef(tes,ios::out);
    if(odef.fail()){
        File::FileFail(tes);
    }
    //header����ѭ������
    for(uint32_t i=0;i<headInfo.size();++i){
        tes=HeaderPath+areaName(i);
        File::makefolder(tes);//����ÿ���������ļ���
        ouf<<"\t;"<<areaName(i)<<"_HeaderData"<<endl;
        odef<<"\t;"<<areaName(i)<<"_DefineLabel"<<endl;
        for(uint32_t j=0;j<headInfo[i].size();++j){
            ouf<<areaName(i)<<"_room_"<<hex
            <<setiosflags(ios::uppercase)<<setfill('0')<<j
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].tileSet
            <<",0x"<<setw(2)<<(int)headInfo[i][j].BG0_Properties
            <<",0x"<<setw(2)<<(int)headInfo[i][j].BG1_Properties
            <<",0x"<<setw(2)<<(int)headInfo[i][j].BG2_Properties
            <<",0x"<<setw(2)<<(int)headInfo[i][j].BG3_Properties
            <<",0x"<<setw(2)<<(int)headInfo[i][j].unuse1[0]
            <<",0x"<<setw(2)<<(int)headInfo[i][j].unuse1[1]
            <<",0x"<<setw(2)<<(int)headInfo[i][j].unuse1[2]
            <<"\t;tileSet,BG 0-3_Properties,unuse1 0-2"<<endl;
        }
    }
}


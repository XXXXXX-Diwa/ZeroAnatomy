#include "HeaderSeriesOut.h"
#include "Decompress.h"

/**
*Header数据分为7个区域,每个区域的每个房间都有一个Header数据60字节长
*每个房间有3层BG和一个实体数据要打印,3个精灵设置数据要打印(都不定长)
**/
HeaderSeriesOut::HeaderSeriesOut(string name,string path):readRomRoute(name),HeaderPath(path){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    uint32_t bit32,byte4;
    inf.seekg(ldrAreaHeaderOffset[0],ios::beg);
    inf.read((char*)&bit32,4);
    if(bit32>>25!=4){
        DataException::DataError("错误的ROM,地址异常!");
    }
    for(int i=1;i<4;++i){
        inf.seekg(ldrAreaHeaderOffset[i],ios::beg);
        inf.read((char*)&byte4,4);
        if(byte4!=bit32){
            DataException::DataError("错误的ROM,地址不相同!");
        }
    }
    allAreaHeaderOffsets=bit32^0x8000000;
    inf.seekg(allAreaHeaderOffsets);
    for(int i=0;i<7;++i){
        inf.read((char*)&bit32,4);
        if(bit32>>25!=4){
            bit32=inf.tellg();
            bit32-=4;
            DataException::DataError("错误的区域头数据地址!",bit32);
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
                    DataException::DataError("区头数据房间数不该为0!");
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
            headSeriesDLP.push_back(teDLP);//每成功一个都记录在清单数据中
        }
    }
    inf.close();
}

string HeaderSeriesOut::areaName(uint8_t arean)
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
            return string("错误的区号!");
    }

}
string HeaderSeriesOut::compressType(uint8_t property){
    return (property&0x40)?"Lz7":"Rel";
}

string HeaderSeriesOut::numToHexStr(const uint32_t Num,const uint8_t width){
    if(width<1){
        DataException::DataError("数组转十六进制字符串的宽度参数不应小于1!");
    }
    string s;
    stringstream ss;
    ss<<hex<<setw(width)<<setfill('0')
    <<setiosflags(ios::uppercase)<<Num;
    ss>>s;
    return s;
}
string HeaderSeriesOut::dataType(uint8_t type){
    switch(type){
    case 0:
        return "BG0";
    case 1:
        return "BG1";
    case 2:
        return "BG2";
    case 3:
        return "BG3";
    case 4:
        return "Clip";
    default:
        return "";
    }
}

uint8_t HeaderSeriesOut::dataOutType(uint8_t property){
    return (property&0x40)?1:0;
}

string HeaderSeriesOut::bgDataDescription   //格式:区域名+房间号+_数据类型+_压缩类型
(const uint8_t type,const uint8_t property){
    return "_"+HeaderSeriesOut::dataType(type)+"_"
    +HeaderSeriesOut::compressType(property);
}
void HeaderSeriesOut::MakeHeaderSeriesFolders(){
    File::makefolder(HeaderPath+"RelData");
    File::makefolder(HeaderPath+"Lz77Data");
    File::makefolder(HeaderPath+"SpriteData");

}
HeaderSeriesOut::~HeaderSeriesOut(){}

void HeaderSeriesOut::HeadAllDataOut(){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    string tes=HeaderPath+"HeadGerenalData.asm";        //ldr 所有区头地址数据首地址的地址处
    //公共数据地址
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    for(int i=0;i<4;++i){
        ouf<<".org 0x" <<hex<<setiosflags(ios::uppercase)
        <<(ldrAreaHeaderOffset[i]^0x8000000)<<endl;
        ouf<<"\t.word allAreaHeaderOffsets\t\t\t;"<<hex
        <<setiosflags(ios::uppercase)<<allAreaHeaderOffsets<<endl;
    }
    ouf.close();
    ouf.clear();

    tes=HeaderPath+"HeadCustomData.asm";    //所有头数据内容
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
//    tes=HeaderPath+"HeadPointerDef.asm";
//    ofstream odef(tes,ios::out);
//    if(odef.fail()){
//        DataException::FileError(tes,0);
//    }
    //header数据循环遍历
    map<string,uint32_t>desToOft;  //描述->地址
    map<uint32_t,uint8_t>oftToType;//地址->压缩类型(0:rel,1:lz77,2:spritedata);
    map<uint32_t,uint32_t>oftToLen;//地址->长度 不用去重就去重了
    for(uint32_t i=0;i<headInfo.size();++i){
//        tes=HeaderPath+areaName(i);
//        File::makefolder(tes);//创建每个区数据文件夹
        uint8_t teu8[4];//临时接受数
        uint32_t teoft[5];//临时接受地址
        string testr[5];//临时接受字符串
        ouf<<"\t;"<<areaName(i)<<"_HeaderData"<<endl;
//        odef<<"\t;"<<areaName(i)<<"_DefineLabel"<<endl;
        for(uint32_t j=0;j<headInfo[i].size();++j){
            //当前区名+房间号
            tes=areaName(i)+HeaderSeriesOut::numToHexStr(j,2);
            //获取每个BG的属性
            teu8[0]=headInfo[i][j].BG0_Properties;
            teu8[1]=headInfo[i][j].BG1_Properties;
            teu8[2]=headInfo[i][j].BG2_Properties;
            teu8[3]=headInfo[i][j].BG3_Properties;
            //写入head数据BG地址前的部分
            ouf<<";"<<areaName(i)<<"_room_"<<hex
            <<setiosflags(ios::uppercase)<<setfill('0')<<j
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].tileSet
            <<",0x"<<setw(2)<<(int)teu8[0]
            <<",0x"<<setw(2)<<(int)teu8[1]
            <<",0x"<<setw(2)<<(int)teu8[2]
            <<",0x"<<setw(2)<<(int)teu8[3]
            <<",0x00,0x00,0x00"
            <<"\t;tileSet,BG 0-3_Properties,unuse1 0-2"
            <<endl;
            //获取每个BG数据地址和clip数据地址的描述
            testr[0]=tes+HeaderSeriesOut::bgDataDescription(0,teu8[0]);
            testr[1]=tes+HeaderSeriesOut::bgDataDescription(1,teu8[1]);
            testr[2]=tes+HeaderSeriesOut::bgDataDescription(2,teu8[2]);
            testr[3]=tes+HeaderSeriesOut::bgDataDescription(3,teu8[3]);
            testr[4]=tes+HeaderSeriesOut::bgDataDescription(4,0);
            //获取每个BG数据地址和Clip数据地址
            teoft[0]=headInfo[i][j].BG0_DataPointer^0x8000000;
            teoft[1]=headInfo[i][j].BG1_DataPointer^0x8000000;
            teoft[2]=headInfo[i][j].BG2_DataPointer^0x8000000;
            teoft[3]=headInfo[i][j].BG3_DataPointer^0x8000000;
            teoft[4]=headInfo[i][j].clipDataPointer^0x8000000;
            //写入BG地址描述
            ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
            <<"\t.dw "<<testr[0]<<"\t\t\t;"<<setw(7)<<teoft[0]
            <<"\n\t.dw "<<testr[1]<<"\t\t\t;"<<setw(7)<<teoft[1]
            <<"\n\t.dw "<<testr[2]<<"\t\t\t;"<<setw(7)<<teoft[2]
            <<"\n\t.dw "<<testr[4]<<"\t\t\t;"<<setw(7)<<teoft[4]
            <<"\n\t.dw "<<testr[3]<<"\t\t\t;"<<setw(7)<<teoft[3]
            <<endl;
            //描述->地址map记录
            desToOft.insert(pair<string,uint32_t>(testr[0],teoft[0]));
            desToOft.insert(pair<string,uint32_t>(testr[1],teoft[1]));
            desToOft.insert(pair<string,uint32_t>(testr[2],teoft[2]));
            desToOft.insert(pair<string,uint32_t>(testr[3],teoft[3]));
            desToOft.insert(pair<string,uint32_t>(testr[4],teoft[4]));
            //地址->压缩类型记录
            oftToType.insert(pair<uint32_t,uint8_t>
                             (teoft[0],HeaderSeriesOut::dataOutType(teu8[0])));
            oftToType.insert(pair<uint32_t,uint8_t>
                             (teoft[1],HeaderSeriesOut::dataOutType(teu8[1])));
            oftToType.insert(pair<uint32_t,uint8_t>
                             (teoft[2],HeaderSeriesOut::dataOutType(teu8[2])));
            oftToType.insert(pair<uint32_t,uint8_t>
                             (teoft[3],HeaderSeriesOut::dataOutType(teu8[3])));
            oftToType.insert(pair<uint32_t,uint8_t>(teoft[4],0));//clip压缩固定为rel
            //地址->数据长度记录(初始长度全为0)
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[1],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[2],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[3],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[4],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[5],0));
            //写入BG等地址与精灵数据中间的部分
            ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
            <<"\t.db 0x"<<setw(2)<<(int)headInfo[i][j].BG3_Scrolling
            <<",0x"<<setw(2)<<(int)headInfo[i][j].transparency
            <<",0x00,0x00"<<"\t;"<<"BG3_Scrolling,transparency,unuse2 0-1"<<endl;
            //组合每个精灵数据地址的描述
            testr[0]=tes+"_spriteData_default";
            testr[1]=tes+"_spriteData_first";
            testr[2]=tes+"_spriteData_second";
            //获取每个精灵数据地址
            teoft[0]=headInfo[i][j].defaultSpriteDataPointer^0x8000000;
            teoft[1]=headInfo[i][j].firstSpriteDataPointer^0x8000000;
            teoft[2]=headInfo[i][j].secondSpriteDataPointer^0x8000000;
            //描述->地址map记录 sprite篇
            desToOft.insert(pair<string,uint32_t>(testr[0],teoft[0]));
            desToOft.insert(pair<string,uint32_t>(testr[1],teoft[1]));
            desToOft.insert(pair<string,uint32_t>(testr[2],teoft[2]));
            //地址->压缩类型记录 sprite篇
            oftToType.insert(pair<uint32_t,uint8_t>(teoft[0],2));
            oftToType.insert(pair<uint32_t,uint8_t>(teoft[1],2));
            oftToType.insert(pair<uint32_t,uint8_t>(teoft[2],2));
            //地址->数据长度记录(初始长度全为0) sprite篇
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[1],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[2],0));
            oftToLen.insert(map<uint32_t,uint32_t>::value_type(teoft[3],0));
            //写入剩余的Header数据
            ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
            <<"\t.dw "<<testr[0]
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].defaultSpriteSet
            <<",0x"<<setw(2)<<(int)headInfo[i][j].firstSpriteSetEvent
            <<",0x00,0x00\t;defaultSpriteSet,firstSpriteSetEvent,unuse3 0-1"
            <<"\n\t.dw "<<testr[1]
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].firstSpriteSet
            <<",0x"<<setw(2)<<(int)headInfo[i][j].secondSpriteEvent
            <<",0x00,0x00\t;firstSpriteSet,secondSpriteEvent,unuse4 0-1"
            <<"\n\t.dw "<<testr[2]
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].secondSpriteSet
            <<",0x"<<setw(2)<<(int)headInfo[i][j].mapXCoordinate
            <<",0x"<<setw(2)<<(int)headInfo[i][j].mapYCoordinate
            <<"\t\t;secondSpriteSet,mapXCoordinate,mapYCoorDinate"
            <<"\n\t.db 0x"<<setw(2)<<(int)headInfo[i][j].effect
            <<",0x"<<setw(2)<<(int)headInfo[i][j].effectYPosition
            <<",0x00\t\t;effect,effectYPosition,unuse5"
            <<"\n\t.dh 0x"<<setw(4)<<(int)headInfo[i][j].musicTrackNumber
            <<"\t\t\t\t;musicTrackNumber"<<endl;
        }
    }
    ouf.close();
    ouf.clear();
    map<uint32_t,uint8_t>::iterator it;
    unique_ptr<RelBgData>rbd(new RelBgData());
    unique_ptr<Lz77BgData>lbd(new Lz77BgData());
    unique_ptr<SpriteData>spd(new SpriteData());
    for(it=oftToType.begin();it!=oftToType.end();++it){
        switch(it->second){
        case 0:
            tes=HeaderPath+"RelData\\"+HeaderSeriesOut::numToHexStr(it->first,7)+".rel";
            break;
        case 1:
            tes=HeaderPath+"Lz77Data\\"+HeaderSeriesOut::numToHexStr(it->first,7)+".lz7";
            break;
        case 2:
            tes=HeaderPath+"SpriteData\\"+HeaderSeriesOut::numToHexStr(it->first,7)+".spd";
            break;
        default:
            DataException::DataError("压缩类型参数错误!");
        }
        ouf.open(tes,ios::out|ios::binary);     //制造每个地址的数据文件
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }
        inf.seekg(it->first,ios::beg);          //定位到数据处
        switch(it->second){
        case 0:
            rbd->getRelCompressData(inf);
            ouf.put((char)rbd->roomWidth);
            ouf.put((char)rbd->roomHeigh);
            ouf.write((char*)rbd->relCompressedData,rbd->length);
            break;
        case 1:
            lbd->getLz77CompressData(inf);
            ouf.write((char*)&lbd->bgSize,4);
            ouf.write((char*)&lbd->decompressedLen,4);
            ouf.write((char*)lbd->lz77CompressedTileTable,lbd->length);
            break;
        default:
            spd->getSpriteData(inf);
            ouf.write((char*)spd->data,spd->length);
        }
        ouf.close();
    }
    rbd.reset();
    lbd.reset();
    spd.reset();
}


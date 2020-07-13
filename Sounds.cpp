#include "Sounds.h"
#include "HeaderSeriesOut.h"

Sounds::Sounds(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    uint32_t bit32,byte4,offset;
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrTrackTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("ldr的TrackTable地址不是指针!");
    }
    /**trackTable部分**/
    Info info;
    offset=bit32^0x8000000;
    inf.seekg(offset,ios::beg);//定位到trackTable处
    bit32=0;
    Track track[128];
    bool twicebreak=false;
    vector<uint32_t>headofts;//用来记录header地址
    while(!inf.eof()){//计算有多少个有效
        byte4=0;
        inf.read((char*)&track,sizeof(track));
        for(uint8_t i=0;i<128;++i){
            if((track[i].HeaderOft>>25)!=4||
               track[i].unuse1!=0||track[i].unuse2!=0){
                twicebreak=true;
                break;

            }
            headofts.push_back(track[i].HeaderOft^0x8000000);//记录header的地址
            ++byte4;
        }
        if(twicebreak){
            bit32=bit32*128+byte4;
            break;
        }
        ++bit32;
    }
    cout<<"table总数为: "<<hex<<bit32;
    cin.get();
    info.describe="TrackTable";//记录描述
    info.typeLen=0x80000000|bit32;
    pointer.insert(pair<uint32_t,Info>(offset,info));//记录Tracktable地址,类型和数量
    /**trackHeader部分**/
    Header header;
    byte4=headofts.size();
    vector<uint32_t>trackofts;//用来记录每个音轨的地址
    vector<uint32_t>vgofts;//用来记录voicegroup的地址
    for(uint32_t i=0;i<byte4;++i){
        offset=headofts[i];
        inf.seekg(offset,ios::beg);
        inf.read((char*)&header,sizeof(header));
        if(header.amountOfTracks==0||header.amountOfTracks>16){
            if(pointer.find(offset)!=pointer.end()){
                info.describe="InvaildTrackHeader";
                info.typeLen=0x40000004;//无效的只提供4byte无效数据
                pointer.insert(pair<uint32_t,Info>(offset,info));
            }
            continue;
        }
        if((header.voiceGroupOft>>25)!=4){
            bit32=inf.tellg();
            bit32=bit32-sizeof(header)+4;
            DataException::DataError("voiceGroup地址不是指针!",bit32);
        }
        vgofts.push_back(header.voiceGroupOft^0x8000000);//记录vg的地址
        for(uint8_t j=0;j<header.amountOfTracks;++j){
            if((header.trackDataOft[j]>>25)!=4){
                bit32=inf.tellg();
                bit32=bit32-sizeof(header)+8+j*4;
                DataException::DataError("trackData地址不是指针!",bit32);
            }
            trackofts.push_back(header.trackDataOft[j]^0x8000000);//记录音轨的地址
        }
        info.describe="TrackHeader_"+HeaderSeriesOut::numToHexStr(offset,7);
        info.typeLen=0x40000000|(header.amountOfTracks*4+8);
        pointer.insert(pair<uint32_t,Info>(offset,info));//记录header的信息
    }
    sort(trackofts.begin(),trackofts.end());//排序并去重
    trackofts.erase(unique(trackofts.begin(),trackofts.end()),trackofts.end());
    /**track部分**/
    uint8_t bit8;
    string tes;
    //记录track中指针的地址和指针本身,值为类型 0:指针所处地址 1:指针指向地址 2:结尾地址
    map<uint32_t,uint8_t>oftToB;
    map<uint32_t,uint8_t>::iterator it;
    for(uint32_t i=0;i<trackofts.size();++i){
        byte4=trackofts[i];
        inf.seekg(byte4,ios::beg);
        tes="Track"+HeaderSeriesOut::numToHexStr(i,6);
        oftToB.insert(pair<uint32_t,uint8_t>(byte4,1));//track开始的位置
        while(true){
            bit8=inf.get();
            if(bit8>0xB0){
                bit32=inf.tellg();
                if(bit8==0xB2||bit8==0xB3){

                    inf.read((char*)&offset,4);
                    if((offset>>25)!=4){
                        DataException::DataError("track数据中的地址非指针!",bit32);
                    }
                    offset^=0x8000000;
                    if(offset<trackofts[i]){
                        DataException::DataError("track数据中的地址比track开始地址还小!",bit32);
                    }
                    oftToB.insert(pair<uint32_t,uint8_t>(bit32,0));//指针所在位置
                    oftToB.insert(pair<uint32_t,uint8_t>(offset,1));//指针指向位置
                }else if(bit8==0xB3){
                    oftToB.insert(pair<uint32_t,uint8_t>(bit32,2));//结束所在位置
                    break;
                }
            }
        }
        twicebreak=false;
        for(it=oftToB.begin();it!=oftToB.end();){
            bit32=it->first;//记录坐标
            switch(it->second){
            case 0://指针所在处
                if((++it)==oftToB.end()){
                    twicebreak=true;
                }
                info.describe="";//指针所在处并不能推导出指向何处,写出的时候才能知晓
                break;
            case 1://指针指向的位置
                if((++it)==oftToB.end()){
                    twicebreak=true;
                }
                info.describe=tes+"_"+HeaderSeriesOut::numToHexStr(bit32,7);
                break;
            default://总不会经历
                break;
            }
            info.typeLen=0x21000000|(it->first-bit32);
            pointer.insert(pair<uint32_t,Info>(bit32,info));
            if(twicebreak){
                break;
            }
        }
    }
    sort(vgofts.begin(),vgofts.end());//排序和去重
    vgofts.erase(unique(vgofts.begin(),vgofts.end()),vgofts.end());
    /**voiceGroup部分**/
    Key keys[128];
    uint8_t damage[128];
    oftToB.clear();//清除元素 用来记录multi和drum的地址和键数
    for(uint32_t i=0;i<vgofts.size();++i){//样本记录
        inf.seekg(vgofts[i],ios::beg);
        inf.read((char*)&keys,sizeof(keys));
        for(uint8_t j=0;j<128;++j){
            bit8=keys[j].type&0xE7;//去掉18;
            switch(bit8){
            case 0:
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
//                    bit32=vgofts[i]+j*12;
//                    DataException::DataError("directSound样本地址非指针!",bit32);
                    break;
                }
                bit32^=0x8000000;
                if(pointer.find(bit32)==pointer.end()){
                    break;
                }
                info.describe="Sample_"+HeaderSeriesOut::numToHexStr(bit32,7);//没什么好描述的!
                info.typeLen=0;//类型和长度都为0 长度要导出时具体算
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                bit32^=0x8000000;
                if((bit32>>25)!=4){
                    bit32=vgofts[i]+j*12;
                    DataException::DataError("waveMemory数据地址非指针!",bit32);
                }
                bit32^=0x8000000;
                if(pointer.find(bit32)==pointer.end()){
                    break;
                }
                info.describe="WaveMemory_"+HeaderSeriesOut::numToHexStr(bit32,7);//同样没什么好描述的!
                info.typeLen=0x1000000;//长度没什么好记载的,波形固定为16字节长度
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 4:
                break;
            case 0x40:
                bit32=keys[j].meydamoft;
                if((bit32>>25)!=4){
                    bit32=vgofts[i]+j*12;
                    DataException::DataError("damage数据地址非指针!",bit32);
                }
                bit32^=0x8000000;
                inf.seekg(bit32,ios::beg);//读取damage数据
                inf.read((char*)damage,128);
                bit8=0;//归零
                for(uint8_t k=0;k<128;++k){
                    if(damage[k]>bit8){
                        bit8=damage[k];
                    }
                }
                if(pointer.find(bit32)==pointer.end()){
                    break;
                }
                info.describe="Damage_"+HeaderSeriesOut::numToHexStr(bit32,7);//范围没什么好描述的
                info.typeLen=0x2000000;//范围固定为128字节,只记类型即可
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    bit32=vgofts[i]+j*12;
                    DataException::DataError("multisample地址非指针!",bit32);
                }
                bit32^=0x8000000;
                oftToB.insert(pair<uint32_t,uint8_t>(bit32,bit8+1));
                break;
            case 0x80:
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    bit32=vgofts[i]+j*12;
                    DataException::DataError("multisample地址非指针!",bit32);
                }
                bit32^=0x8000000;
                oftToB.insert(pair<uint32_t,uint8_t>(bit32,129));//记录多1区分multi和drum
                break;
            default:
                bit32=vgofts[i]+j*12;
                DataException::DataError("有效和无效键位之外的类型~",bit32);
                break;
            }
        }
    }
    for(it=oftToB.begin();it!=oftToB.end();++it){//样本记录
        bit32=it->first;
        inf.seekg(bit32,ios::beg);
        inf.read((char*)keys,sizeof(keys));
        it->second;
        for(uint8_t j=0;j<it->second;++j){
            bit8=keys[j].type&0xE7;
            switch(keys[j].type){
            case 0:
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    break;
                }
                bit32^=0x8000000;
                if(pointer.find(bit32)==pointer.end()){
                    break;
                }
                info.describe="Sample_"+HeaderSeriesOut::numToHexStr(bit32,7);//没什么好描述的!
                info.typeLen=0;//类型和长度都为0 长度要导出时具体算
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                bit32^=0x8000000;
                if((bit32>>25)!=4){
                    bit32=it->first+j*12;
                    DataException::DataError("waveMemory数据地址非指针!",bit32);
                }
                bit32^=0x8000000;
                if(pointer.find(bit32)==pointer.end()){
                    break;
                }
                info.describe="Wavememory_"+HeaderSeriesOut::numToHexStr(bit32,7);//同样没什么好描述的!
                info.typeLen=0x1000000;//长度没什么好记载的,波形固定为16字节长度
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 4:
                break;
            case 40:
                break;
            case 80:
                break;
            default:
                bit32=it->first+j*12;
                DataException::DataError("有效和无效键位之外的类型~",bit32);
                break;
            }
        }
    }
    map<uint32_t,uint32_t>tevglen;//用来计算多样本键集的长度
    for(uint32_t i=0;i<vgofts.size();++i){
        tevglen.insert(pair<uint32_t,uint32_t>(vgofts[i],0x80));
    }
    pair<map<uint32_t,uint32_t>::iterator,bool> insok;
    for(it=oftToB.begin();it!=oftToB.end();++it){
        if(it->second==0x81){
            insok=tevglen.insert(pair<uint32_t,uint32_t>(it->first,0x81));
        }else{
            insok=tevglen.insert(pair<uint32_t,uint32_t>(it->first,0x80000000|it->second));
        }
        if(!insok.second){
            if(it->second==0x81){
                DataException::DataError("drum类型的指针竟然和voiceGroup的地址重合!",it->first);
            }else{
                DataException::DataError("multi类型的指针竟然和voiceGroup的地址重合!",it->first);
            }
        }
    }
    map<uint32_t,uint32_t>::iterator itlen;
    for(itlen=tevglen.begin();itlen!=tevglen.end();){
        offset=itlen->first;
        byte4=itlen->second;
        ++itlen;
        if(byte4>0xFF){
            byte4=byte4<<1>>1;
            bit32=byte4;
            byte4*=12;
            byte4+=offset;
            if(byte4>itlen->first){
                byte4=itlen->first-offset;
                bit32=byte4/12;
            }
            bit32|=0x8000000;;
            info.describe="MultiSample_"+HeaderSeriesOut::numToHexStr(offset,7);
        }else if(byte4>0x80){
            --byte4;
            bit32=byte4;
            byte4*=12;
            byte4+=offset;
            if(byte4>itlen->first){
                byte4=itlen->first-offset;
                bit32=byte4/12;
            }
            bit32|=0x10000000;
            info.describe="DrumPart_"+HeaderSeriesOut::numToHexStr(offset,7);
        }else{
            bit32=byte4;
            byte4*=12;
            byte4+=offset;
            if(byte4>itlen->first){
                byte4=itlen->first-offset;
                bit32=byte4/12;
            }
            bit32|=0x4000000;
            info.describe="VoiceGroup_"+HeaderSeriesOut::numToHexStr(offset,7);
        }
        info.typeLen=bit32;
        pointer.insert(pair<uint32_t,Info>(offset,info));
        if(itlen==tevglen.end()){
            byte4=itlen->second;
            if(byte4>0xFF){
                byte4=byte4<<1>>1;
                info.typeLen=0x8000000|byte4;
                info.describe="MultiSample_"+HeaderSeriesOut::numToHexStr(offset,7);
            }else if(byte4>0x80){
                info.typeLen=0x1000007F;
                info.describe="DrumPart_"+HeaderSeriesOut::numToHexStr(offset,7);
            }else{
                info.typeLen=0x400007F;
                info.describe="VoiceGroup_"+HeaderSeriesOut::numToHexStr(offset,7);
            }
            pointer.insert(pair<uint32_t,Info>(itlen->first,info));
        }
    }
}
Sounds::~Sounds(){}

void Sounds::MakeSoundSeriesFolder(){
    File::makefolder(curPath+"Samples");
    File::makefolder(curPath+"WaveMemory");
    File::makefolder(curPath+"Damage");
}
void Sounds::SoundSeriesDataOut(){
    ifstream inf;
    ofstream ouf,voicef,trackf,def;
    string tes=curPath+"TrackGerenalData.asm";
    File::MakeFile(ouf,tes,false);
    for(uint8_t i=0;i<19;++i){
        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<".org 0x"<<(int)(ldrTrackTableOft[i]|0x8000000)
        <<"\n\t.word TrackTable"<<endl;
    }
    ouf.close();
    tes=curPath+"TrackTableData.asm";
    File::MakeFile(ouf,tes,false);

}

#include "Sounds.h"
#include "HeaderSeriesOut.h"

Sounds::Sounds(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    uint32_t bit32,byte4,offset;
    DataListPrint tedlp;
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrTrackTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("ldr的TrackTable地址不是指针!");
    }

    RunTimes *durt=RunTimes::SharedRunTime();

    /**trackTable部分**/
    Info info;
    offset=bit32^0x8000000;
    tedlp.offset=offset;
    inf.seekg(offset,ios::beg);//定位到trackTable处
    bit32=0;
    TrackTable table[128];
    bool twicebreak=false;
    vector<uint32_t>headofts;//用来记录header地址
    while(!inf.eof()){//计算有多少个有效
        byte4=0;
        inf.read((char*)&table,sizeof(table));
        for(uint8_t i=0;i<128;++i){
            if((table[i].HeaderOft>>25)!=4||
               table[i].unuse1!=0||table[i].unuse2!=0){
                twicebreak=true;
                break;

            }
            headofts.push_back(table[i].HeaderOft^0x8000000);//记录header的地址
            ++byte4;
        }
        if(twicebreak){
            bit32=bit32*128+byte4;
            break;
        }
        ++bit32;
    }
    info.describe="TrackTable";//记录描述
    info.typeLen=0x80000000|bit32;
    pointer.insert(pair<uint32_t,Info>(offset,info));//记录Tracktable地址,类型和数量
    tedlp.explain=info.describe;
    tedlp.len=bit32*8;
    songdlp.push_back(tedlp);

    durt->RunFinish("Sounds:trackTable部分");

    /**trackHeader部分**/
    durt=RunTimes::SharedRunTime();

    TrackHeader header;
    byte4=headofts.size();//头部的数量
    vector<uint32_t>trackofts;//用来记录每个音轨的地址
    vector<uint32_t>vgofts;//用来记录voicegroup的地址
    for(uint32_t i=0;i<byte4;++i){
        offset=headofts[i];
        tedlp.offset=offset;
        inf.seekg(offset,ios::beg);
        inf.read((char*)&header,sizeof(header));
        if(header.amountOfTracks==0||header.amountOfTracks>16){
            if(pointer.find(offset)==pointer.end()){
                info.describe="InvaildTrackHeader";
                info.typeLen=0x40000004;//无效的只提供4byte无效数据
                pointer.insert(pair<uint32_t,Info>(offset,info));
                tedlp.explain="InvaildTrackHeader";
                tedlp.len=4;
                songdlp.push_back(tedlp);
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
        tedlp.explain=info.describe;
        tedlp.len=header.amountOfTracks*4+8;
        info.typeLen=0x40000000|tedlp.len;
        songdlp.push_back(tedlp);
        pointer.insert(pair<uint32_t,Info>(offset,info));//记录header的信息
    }
    durt->RunFinish("Sounds:trackHeader部分");

    /**track部分**/
    durt=RunTimes::SharedRunTime();

    sort(trackofts.begin(),trackofts.end());//排序并去重
    trackofts.erase(unique(trackofts.begin(),trackofts.end()),trackofts.end());

    uint8_t bit8;
    //记录track中指针的地址和指针本身,值为类型 0:指针所处地址 1:指针指向地址 2:结尾地址
    map<uint32_t,uint32_t>oftToB;
    map<uint32_t,uint32_t>::iterator it;

    for(uint32_t i=0;i<trackofts.size();++i){
        byte4=trackofts[i];
        inf.seekg(byte4,ios::beg);
        tedlp.explain="Track_"+HeaderSeriesOut::numToHexStr(byte4,7);
        tedlp.offset=byte4;
        oftToB.insert(pair<uint32_t,uint32_t>(byte4,0));//track开始的位置
        bit32=0x1000;
        if((i+1)<trackofts.size()){
            bit32=trackofts[i+1]-trackofts[i];
            if(bit32>0x800){
                bit32=0x1000;
            }
        }
        inf.read((char*)buffer,bit32);
        for(uint32_t j=0;j<bit32;){
            bit8=buffer[j];
            ++j;
            if(bit8>0xB0){
                if(bit8==0xB2||bit8==0xB3){
                    offset=*(uint32_t*)&buffer[j];
                    j+=4;
//                    cout<<"offset"<<hex<<offset<<endl;
                    if((offset>>25)!=4){
                        DataException::DataError("track数据中的地址非指针!",bit32);
                    }
                    offset^=0x8000000;
                    if(offset<byte4){
                        DataException::DataError("track数据中的地址比track开始地址还小!",bit32);
                    }
                    oftToB.insert(pair<uint32_t,uint32_t>(byte4+j-4,offset));//指针所在位置->指向位置
//                    cout<<"所在位置: "<<hex<<byte4+j-4<<endl;
                    if(oftToB.find(offset)==oftToB.end()){
                        oftToB.insert(pair<uint32_t,uint32_t>(offset,0));//指针指向位置
                    }
                }else if(bit8==0xB1){
                    oftToB.insert(pair<uint32_t,uint32_t>(trackofts[i]+j,0));//结束所在位置
                    tedlp.len=j;
                    songdlp.push_back(tedlp);
                    break;
                }
            }
        }
        it=oftToB.begin();
        while(true){
            bit32=it->first;//记录坐标
            byte4=it->second;
            if(byte4){
                ++it;
                info.describe="Track_"+HeaderSeriesOut::numToHexStr(byte4,7);
                info.typeLen=0x21000000|(it->first-bit32-4);
            }else{
                if((++it)==oftToB.end()){
                    break;
                }
                info.describe="Track_"+HeaderSeriesOut::numToHexStr(bit32,7);
                info.typeLen=0x20000000|(it->first-bit32);
            }
            pointer.insert(pair<uint32_t,Info>(bit32,info));
        }
        oftToB.clear();
    }

    durt->RunFinish("Sounds:track部分");

    /**voiceGroup部分**/
    durt=RunTimes::SharedRunTime();

    sort(vgofts.begin(),vgofts.end());//排序和去重
    vgofts.erase(unique(vgofts.begin(),vgofts.end()),vgofts.end());
    uint8_t damage[128];
    vector<uint8_t>tekeys;
    tekeys.reserve(128);
    oftToB.clear();//清除元素 用来记录VG和drum的地址和键数
    for(uint32_t i=0;i<vgofts.size();++i){//普通的VG复制
        oftToB.insert(pair<uint32_t,uint32_t>(vgofts[i],0x4000080));//默认128个键位
    }
    Key keys[128];
    for(uint32_t i=0;i<vgofts.size();++i){//样本记录
        inf.seekg(vgofts[i],ios::beg);
        inf.read((char*)&keys,sizeof(keys));
        twicebreak=false;
        for(uint8_t j=0;j<128;++j){
            bit8=keys[j].type&0xC7;//去掉18(E7);0x8d984处莫名有20开头,故20也去掉
//            cout<<hex<<"vg: 0x"<<(int)vgofts[i]<<" type="<<(int)keys[j].type;
//            cin.get();
            switch(bit8){
            case 0:
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    if(keys[j].difoft==0&&keys[j].meydamoft==0){
                        break;
                    }
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                info.describe="Sample_"+HeaderSeriesOut::numToHexStr(bit32,7);
                info.typeLen=0;//类型和长度都为0 长度要导出时具体算
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 1:
                if(keys[j].sweep>0x7F||keys[j].difoft>3||keys[j].pan!=0){
                    twicebreak=true;
                }
                break;
            case 2:
                if(keys[j].sweep>0x7F||keys[j].difoft!=0||keys[j].pan!=0){
                    twicebreak=true;
                }
                break;
            case 3:
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                if(pointer.find(bit32)!=pointer.end()){
                    break;
                }
                info.describe="WaveMemory_"+HeaderSeriesOut::numToHexStr(bit32,7);
                info.typeLen=0x1000000;//长度没什么好记载的,波形固定为16字节长度
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                break;
            case 4:
                if(keys[j].pan!=0||keys[j].sweep!=0||keys[j].difoft>1){
                    twicebreak=true;
                }
                break;
            case 0x40:
                bit32=oftToB[vgofts[i]];
                if(bit32>>28){//一旦当前检查的是drumpart则跳过该类键
                    break;
                }
                bit32=keys[j].meydamoft;
                if((bit32>>25)!=4){
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                inf.seekg(bit32,ios::beg);//读取damage数据
                inf.read((char*)damage,128);
                sort(damage,damage+127);
                bit8=unique(damage,damage+127)-damage;
                tekeys.clear();
                for(uint8_t k=0;k<bit8;++k){
                    if(damage[k]!=0xFF){
                        tekeys.push_back(damage[k]);
                    }
                }
                info.describe="Damage_"+HeaderSeriesOut::numToHexStr(bit32,7);
                info.typeLen=0x2000000;//范围固定为128字节,只记类型即可
                pointer.insert(pair<uint32_t,Info>(bit32,info));
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                if(multiKey.find(bit32)!=multiKey.end()){//检查是否之前已经插入过
                    tekeys.insert(tekeys.end(),multiKey[bit32].begin(),multiKey[bit32].end());
                    sort(tekeys.begin(),tekeys.end());
                    tekeys.erase(unique(tekeys.begin(),tekeys.end()),tekeys.end());
                    tekeys.swap(multiKey[bit32]);
                    break;
                }
                multiKey.insert(pair<uint32_t,vector<uint8_t> >(bit32,tekeys));
                break;
            case 0x80:
                bit32=oftToB[vgofts[i]];
                if(bit32>>28){//一旦当前检查的是drumpart则跳过该类键
                    break;
                }
                bit32=keys[j].difoft;
                if((bit32>>25)!=4){
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                if(oftToB.find(bit32)!=oftToB.end()){
                    if(!(oftToB[bit32]>>28)){
                        DataException::DataError("DrumPart和VoiceGroup坐标重合!",bit32);
                    }
                    break;
                }
                oftToB.insert(pair<uint32_t,uint32_t>(bit32,0x10000080));
                vgofts.push_back(bit32);
                break;
            default:
//                bit32=vgofts[i]+j*12;
//                cout<<hex<<"vg为: 0x"<<vgofts[i]<<" "<<oftToB[vgofts[i]]<<endl;
//                cout<<hex<<"地址为: 0x"<<bit32<<endl;
//                cout<<hex<<"数据为: ";
//                inf.seekg(bit32,ios::beg);
//                for(uint8_t i=0;i<12;++i){
//                    cout<<hex<<(int)inf.get()<<" ";
//                }
//                cout<<endl;

                twicebreak=true;
                break;
            }
            if(twicebreak){
                bit32=oftToB[vgofts[i]];
                bit32=bit32>>8<<8;
                bit32|=j;
                oftToB[vgofts[i]]=bit32;
                break;
            }
        }
    }
    map<uint32_t,vector<uint8_t> >::iterator mt=multiKey.begin();
    for(;mt!=multiKey.end();++mt){
        byte4=mt->second.size();
        offset=mt->first;
        bit32=mt->second.back();
        bit8=bit32;
        bit32=(bit32|0x8000000)+1;
        if(oftToB.find(offset)!=oftToB.end()){
            if((oftToB[offset]>>24)!=8){
                DataException::DataError("MultiSample与DrumPart或VoiceGroup开始坐标重合!",offset);
            }
            break;
        }
        oftToB.insert(pair<uint32_t,uint32_t>(offset,bit32));//把multi也插进去
        inf.seekg(offset,ios::beg);
        inf.read((char*)keys,(bit8+1)*12);
        for(uint8_t i=0;i<=bit8;++i){//记录使用的样本
//            inf.seekg(offset+mt->second[i]*12,ios::beg);
//            inf.read((char*)keys,12);
            bit8=keys[i].type&0xC7;//去掉18(E7);0x8d984处莫名有20开头,故20也去掉
            if(!bit8){//multi好像只有sample的类型,毕竟叫multisample
                bit32=keys[i].difoft;
                if((bit32>>25)!=4){
                    if(keys[i].difoft==0&&keys[i].meydamoft==0){
                        break;
                    }
                    twicebreak=true;
                    break;
                }
                bit32^=0x8000000;
                info.describe="Sample_"+HeaderSeriesOut::numToHexStr(bit32,7);
                info.typeLen=0;//类型和长度都为0 长度要导出时具体算
                pointer.insert(pair<uint32_t,Info>(bit32,info));
            }
        }
    }
    uint32_t pretail,nextail;//记录前一个尾巴和后一个尾巴
    for(it=oftToB.begin();it!=oftToB.end();){//整理前后数据的覆盖情况
        offset=it->first;//offset固定记录当前首坐标
        byte4=it->second;//读取长度和类型标记
        bit8=byte4>>24;
        if(bit8>0x8){//DP类型
            info.describe="DrumPart_"+HeaderSeriesOut::numToHexStr(offset,7);
        }else if(bit8<0x8){//VG类型
            info.describe="VoiceGroup_"+HeaderSeriesOut::numToHexStr(offset,7);
        }else{//MS类型
            info.describe="MultiSample_"+HeaderSeriesOut::numToHexStr(offset,7);
        }
        if((++it)==oftToB.end()){
            break;
        }
        pretail=offset+(byte4<<8>>8)*12;
        bit32=it->first;//bit32固定记录下一个的首坐标
        if(pretail>bit32){//检查前一个尾部范围是否超过了下一个的开头
            byte4=it->second<<8>>8;//读取下一个长度
            nextail=bit32+(byte4*12);//得到下一个尾巴
            if(pretail>nextail){//达到了下一个开头继续检查并超过了下一个结尾
                byte4=(pretail-bit32)/12;//得到前一个应该减去的长度
                bit32=oftToB[offset];//得到前一个类型和长度
                bit32-=byte4;
                info.typeLen=bit32;
                pointer.insert(pair<uint32_t,Info>(offset,info));//记录纠正过后的前一个

                byte4=(pretail-nextail)/12;//得到后一个应该增加的长度
                it->second+=byte4;
            }else{//没有到达结尾的情况
                byte4=(pretail-bit32)/12;//前一个结尾-下一个开头/12得到前一个应该减去的长度
                bit32=oftToB[offset];//得到前一个类型和长度
                bit32-=byte4;//修正长度
//                oftToB[offset]=bit32;//似乎多余了
                info.typeLen=bit32;
                pointer.insert(pair<uint32_t,Info>(offset,info));
            }
        }else{
            info.typeLen=oftToB[offset];//记录进坐标中
            pointer.insert(pair<uint32_t,Info>(offset,info));
        }
    }

    durt->RunFinish("Sounds:voiceGroup部分");

    inf.close();
}
Sounds::~Sounds(){}
void Sounds::PrintData(ofstream &ooo,uint8_t buffer[],const uint32_t length){
    int rsl=length>>4;
    uint32_t rec[4];
    uint32_t i=0;
    while((--rsl)>=0){
        rec[0]=*(uint32_t*)&buffer[i];
        rec[1]=*(uint32_t*)&buffer[i+4];
        rec[2]=*(uint32_t*)&buffer[i+8];
        rec[3]=*(uint32_t*)&buffer[i+12];
        ooo<<"\t.word 0x"<<setw(8)<<rec[0]
        <<",0x"<<setw(8)<<rec[1]
        <<",0x"<<setw(8)<<rec[2]
        <<",0x"<<setw(8)<<rec[3]<<endl;
        i+=16;
    }
    rsl=length%16;
    rsl>>=2;
    if(rsl){
        ooo<<"\t.word 0x";
    }
    while((--rsl)>=0){
        rec[0]=*(uint32_t*)&buffer[i];
        if(rsl>0){
            ooo<<setw(8)<<rec[0]<<",0x";
        }else{
            ooo<<setw(8)<<rec[0]<<endl;
        }
        i+=4;
    }
    rsl=length%4;
    if(rsl){
        ooo<<"\t.byte 0x";
    }
    while((--rsl)>=0){
        if(rsl>0){
            ooo<<setw(2)<<(int)buffer[i]<<",0x";
        }else{
            ooo<<setw(2)<<(int)buffer[i]<<endl;
        }
        ++i;
    }
}
void Sounds::MakeSoundSeriesFolder(){
    File::makefolder(curPath+"Samples");
}
void Sounds::SoundSeriesDataOut(){
    RunTimes *durt=RunTimes::SharedRunTime();
    ifstream inf;
    ofstream ouf,oth,otf,ovg,odg,owm,osd;
    string tes=curPath+"TrackGerenalData.asm";
    uint32_t bit32,byte4,offset=0;
    DataListPrint tedlp;
    File::MakeFile(ouf,tes,false);
    for(uint8_t i=0;i<19;++i){
        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<".org 0x"<<(int)(ldrTrackTableOft[i]|0x8000000)
        <<"\n\t.word TrackTable"<<endl;
    }
    ouf.close();
    //打开rom
    File::OpenFile(inf,readRom,true);
    //设定trackheader文件流
    tes=curPath+"TrackHeaderData.asm";
    File::MakeFile(oth,tes,false);//创建trackheaderdata文件
    TrackSimHeader header;//设置好空间
    oth<<hex<<setiosflags(ios::uppercase)<<setfill('0')<<".align\n";//设置好默认
    //设定track文件流
    tes=curPath+"TrackData.asm";
    File::MakeFile(otf,tes,false);
    otf<<hex<<setiosflags(ios::uppercase)<<setfill('0');
    //设定VGMSDP文件流
    tes=curPath+"VoiceGroupData.asm";
    File::MakeFile(ovg,tes,false);
    ovg<<hex<<setiosflags(ios::uppercase)<<setfill('0')<<".align\n";
    //设定Damage文件流
    tes=curPath+"DamageData.asm";
    File::MakeFile(odg,tes,false);
    odg<<hex<<setiosflags(ios::uppercase)<<setfill('0')<<".align\n";
    //设定WaveMemory文件流
    tes=curPath+"WaveMemoryData.asm";
    File::MakeFile(owm,tes,false);
    owm<<hex<<setiosflags(ios::uppercase)<<setfill('0')<<".align\n";
    //设定Sample文件流
    SampleHead shead;
    tes=curPath+"SamplesDef.asm";
    File::MakeFile(osd,tes,false);


    map<uint32_t,Info>::iterator it;
    for(it=pointer.begin();it!=pointer.end();++it){
        inf.seekg(it->first,ios::beg);//定位到开始处
        bit32=it->second.typeLen;
        byte4=bit32<<8>>8;//长度数据
        bit32>>=24;//类型数据
        switch(bit32){
        case 0://样本类型
            tedlp.offset=it->first;
            tedlp.explain=it->second.describe;
            tes=curPath+"Samples\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            inf.read((char*)&shead,16);
            byte4=shead.sampleSize+(shead.loogflag>>30);
            tedlp.len=byte4+16;
            songdlp.push_back(tedlp);

            ouf.write((char*)&shead,16);
            inf.read((char*)buffer,byte4);
            ouf.write((char*)buffer,byte4);
            osd<<".align\n"<<tedlp.explain
            <<":\n\t.import \"SoundSeries\\Samples\\"
            <<tedlp.explain<<"\""<<endl;
            ouf.close();
            break;
        case 1://波形样本类型
            tedlp.explain=it->second.describe;
            owm<<tedlp.explain<<":"<<endl;
            inf.read((char*)buffer,16);
            PrintData(owm,buffer,16);

            tedlp.len=16;
            tedlp.offset=it->first;
            songdlp.push_back(tedlp);
            break;
        case 2://damage样本类型
            tedlp.explain=it->second.describe;
            tedlp.len=128;
            tedlp.offset=it->first;
            songdlp.push_back(tedlp);
            odg<<tedlp.explain<<":"<<endl;
            inf.read((char*)buffer,128);
            PrintData(odg,buffer,128);
            break;
        case 4://vg琴键组
        case 8://ms琴键组
        case 0x10://dp琴键组
            {
                tedlp.explain=it->second.describe;
                ovg<<tedlp.explain<<":"<<endl;
                tedlp.len=byte4*12;
                tedlp.offset=it->first;
                songdlp.push_back(tedlp);
                inf.read((char*)skeys,byte4*12);
                for(uint32_t i=0;i<byte4;++i){
                    bit32=skeys[i].type<<24>>24;
                    bit32&=0xC7;
                    switch(bit32){
                    case 0:
                        ovg<<"\t.word 0x"<<setw(8)<<skeys[i].type
                        <<",Sample_"<<HeaderSeriesOut::numToHexStr(skeys[i].oft,7)
                        <<",0x"<<setw(8)<<skeys[i].adsr<<endl;
                        break;
                    case 1:
                    case 2:
                    case 4:
                        ovg<<"\t.word 0x"<<setw(8)<<skeys[i].type
                        <<",0x"<<setw(8)<<skeys[i].oft
                        <<",0x"<<setw(8)<<skeys[i].adsr<<endl;
                        break;
                    case 3:
                        ovg<<"\t.word 0x"<<setw(8)<<skeys[i].type
                        <<",WaveMemory_"<<HeaderSeriesOut::numToHexStr(skeys[i].oft,7)
                        <<",0x"<<setw(8)<<skeys[i].adsr<<endl;
                        break;
                    case 0x40:
                        ovg<<"\t.word 0x"<<setw(8)<<skeys[i].type
                        <<",MultiSample_"<<HeaderSeriesOut::numToHexStr(skeys[i].oft,7)
                        <<",Damage_"<<HeaderSeriesOut::numToHexStr(skeys[i].adsr,7)<<endl;
                        break;
                    case 0x80:
                        ovg<<"\t.word 0x"<<setw(8)<<skeys[i].type
                        <<",DrumPart_"<<HeaderSeriesOut::numToHexStr(skeys[i].oft,7)
                        <<",0x"<<setw(8)<<skeys[i].adsr<<endl;
                        break;
                    default:
                        DataException::DataError("存在不属于任何的类型的琴键!");
                        break;
                    }
                }
                break;
            }

        case 0x20://track定义
            otf<<it->second.describe<<":"<<endl;
            inf.read((char*)buffer,byte4);
            PrintData(otf,buffer,byte4);
            break;
        case 0x21://track定义指针
            otf<<"\t.word "<<it->second.describe<<endl;
            inf.seekg(4,ios::cur);
            inf.read((char*)buffer,byte4);
            PrintData(otf,buffer,byte4);
            break;
        case 0x40://track header
            {
                if(byte4<12){
                    oth<<"InvaildTrackHeader:\n\t.word 0x00000000"<<endl;
                    break;
                }
                inf.read((char*)&header,byte4);
                oth<<"TrackHeader_"<<HeaderSeriesOut::numToHexStr(it->first,7)
                <<"\n\t.word 0x"<<setw(8)<<header.amoutOfTracks
                <<"\n\t.word VoiceGroup_"
                <<HeaderSeriesOut::numToHexStr(header.voiceGroupOft,7)<<endl;
                byte4=header.amoutOfTracks<<24>>24;
                for(uint32_t i=0;i<byte4;++i){
                    oth<<"\t.word Track_"
                    <<HeaderSeriesOut::numToHexStr(header.trackDataOft[i],7)
                    <<endl;
                }
                break;
            }

        case 0x80://track table
            {
                tes=curPath+"TrackTableData.asm";
                File::MakeFile(ouf,tes,false);
                TrackSimTable table[byte4];
                inf.read((char*)table,sizeof(table));
                ouf<<".align\n"<<it->second.describe<<":"<<endl;
                ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0');
                for(uint32_t i=0;i<byte4;++i){
                    ouf<<"\t.word TrackHeader_"
                    <<HeaderSeriesOut::numToHexStr(table[i].HeaderOft,7)
                    <<",0x"<<setw(8)<<table[i].songGroup<<endl;
                }
                ouf.close();
                break;
            }
        default:
            DataException::DataError
            ("错误的数据类型!"+it->second.describe,it->second.typeLen);
        }
    }
    inf.close();
    osd.close();
    owm.close();
    odg.close();
    ovg.close();
    otf.close();
    oth.close();
    durt->RunFinish("Sounds全部数据导出");
}

#include "Decompress.h"

RelBgData::RelBgData(){
}
RelBgData::~RelBgData(){
//    delete[]RelBgData::relCompressedData;
}
/**
*首字节是次数是否是单字节flag 若为1,则为单字节,其余则为双字节

*第二个字节的第7位是重复flag,若有标记,则去掉第7位的第二位是第三位的重

*复次数
*		若无标记,则第二位代表了此后有多少个不重复的字节

*当第二个字节为00的时候就可能遇到切换与快要结束的flag

*具体为再一次碰到00(若已经切换到双字节模式则需要碰到00 00)就会结束

*第二字节首次遇到00那么下一个字节就是新首字节,会根据此字节切换单双

**/
void RelBgData:: getRelCompressData(ifstream &inf){
    RelBgData::roomWidth=inf.get();
    RelBgData::roomHeigh=inf.get();
    RelBgData::length=RelBgData::roomHeigh*RelBgData::roomWidth;
    inf.read((char*)RelBgData::relCompressedData,
             RelBgData::length);
    RelBgData::length<<=1;//乘以2
    bool swibit=false;
    uint8_t ut8,bit8,byte1;
    uint8_t *relDat=RelBgData::relCompressedData;//方便使用
    uint16_t bit16;
    uint32_t total=0,len=0;//一个记录解压累计长度一个记录读取的坐标
    ut8=relDat[len];
    while(total<=RelBgData::length){
        if(ut8==1){
            ++len;
            bit8=relDat[len];
            if(bit8==0){
                if(swibit){
                    break;
                }
                swibit=true;
                ++len;
                ut8=relDat[len];//此处可能转为2bit类型,只要该值不为1
					//2bit类型的第二字节和第三字节同时为0才会结束,故结尾为00 00
					//但若维持1bit类型,则第二个字节再次为0就会结束
            }else if((bit8&0x80)==0){
                total+=bit8;
                len+=bit8;
            }else{
                bit8^=0x80;
                if(bit8==0){
                    continue;
                }
                total+=bit8;
                ++len;
            }
        }else{
            ++len;
            bit8=relDat[len];
            ++len;
            byte1=relDat[len];
            bit16=(bit8<<8)|byte1;
            if(bit16==0){
                if(swibit){
                    break;
                }
                swibit=true;
            }else if((bit16&0x8000)==0){
                total+=bit16;
                len+=16;
            }else{
                bit16^=0x8000;
                if(bit16==0){
                    continue;
                }
                total+=bit16;
                ++len;
            }
        }
    }
    RelBgData::length=len;
}


Lz77BgData::Lz77BgData(){}
Lz77BgData::~Lz77BgData(){
//    delete[]Lz77BgData::lz77CompressedTileTable;
}
/**
*首先得到解压长度,然后检查读取的数据地址是否有0x8000000

*读取第一个字节,然后给予个变量=8,变量减一,检查是否为负,
*若为负,则再次读取第一个字节,然后给予变量=8 否则
*检查第一个字节是否没有7位
*没有的话读取第二个字节写入内存 总长度-1 检查总长度是否为0
*不为0的话第一个字节右移1h 然后继续变量-1,检查是否为负
*检查第一个字节是否没有7位...

*若第一个字节有7位,读取第二个字节,算术右移4再加3 (去掉低位并+3)(len)
*再次读取第二个字节去掉高位,然后左移8h,读取第三个字节,orr前面
*处理的第二字节,该值加1.(内存向前偏移值)
*读取当前内存的前方(内存向前偏移),写进当前的内存地址,并指针下移
*len递减,只要len不为0,都继续写

*len为零后,第一个字节左移1位继续循环,只要8次循环还没结束

*每次写数据循环8次,直到达到1000h结束
*首字节位数代表了从什么时候开始进行之前的内存复写.当然此行为代表循环一次

*内存不复写的部分只是单字节的读写循环.最多8次,然后再次读取第一个字节
**/

//数据块在rom中的首尾要与4对齐 起码观察是如此的(未确认)

void Lz77BgData::getLz77CompressData(ifstream &inf){
    inf.read((char*)&bgSize,4);
    inf.read((char*)&decompressedLen,4);
    inf.read((char*)Lz77BgData::lz77CompressedTileTable,
             Lz77BgData::decompressedLen>>8);
    uint16_t bit16;
    uint32_t definelen=Lz77BgData::decompressedLen>>8;//数据总长度
    uint32_t total=0,len=0;//解压累计长度和当前读取位置
    uint8_t bit8,byte1,ut8;
    uint8_t *lzDat=Lz77BgData::lz77CompressedTileTable;//方便使用
    while(total<=definelen){
        bit8=lzDat[len];    //第一个字节
        ++len;
        int i=8;
        while((--i)>=0){
            if((bit8&0x80)==0){
                byte1=lzDat[len];
                ++len;
                ++total;
            }else{
                ut8=lzDat[len];
                ++len;
                byte1=ut8;
                ut8>>=4;
                ut8+=3;
                byte1&=0xF;
                bit16=byte1;
                bit16<<=8;
                byte1=lzDat[len];
                ++len;
                bit16|=byte1;
                total+=ut8;
            }
            bit8<<=1;
            if(total>=definelen){
                break;
            }
        }
    }
    Lz77BgData::length=len;
}

SpriteData::SpriteData(){}
SpriteData::~SpriteData(){}
void SpriteData::getSpriteData(ifstream &inf){
    inf.read((char*)SpriteData::data,75);
    uint8_t *data=SpriteData::data;//方便使用
    for(uint8_t i=0;i<75;i+=3){
        if(data[i]==0xFF&&data[i+1]==0xFF&&data[i+2]==0xFF){
            SpriteData::length=i+3;
            break;
        }
    }
}

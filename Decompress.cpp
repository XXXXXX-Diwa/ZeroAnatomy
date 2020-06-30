#include "Decompress.h"

RelBgData::RelBgData(){
}
RelBgData::~RelBgData(){
//    delete[]RelBgData::relCompressedData;
}
/**
*���ֽ��Ǵ����Ƿ��ǵ��ֽ�flag ��Ϊ1,��Ϊ���ֽ�,������Ϊ˫�ֽ�

*�ڶ����ֽڵĵ�7λ���ظ�flag,���б��,��ȥ����7λ�ĵڶ�λ�ǵ���λ����

*������
*		���ޱ��,��ڶ�λ�����˴˺��ж��ٸ����ظ����ֽ�

*���ڶ����ֽ�Ϊ00��ʱ��Ϳ��������л����Ҫ������flag

*����Ϊ��һ������00(���Ѿ��л���˫�ֽ�ģʽ����Ҫ����00 00)�ͻ����

*�ڶ��ֽ��״�����00��ô��һ���ֽھ��������ֽ�,����ݴ��ֽ��л���˫

**/
void RelBgData:: getRelCompressData(ifstream &inf){
    RelBgData::roomWidth=inf.get();
    RelBgData::roomHeigh=inf.get();
    RelBgData::length=RelBgData::roomHeigh*RelBgData::roomWidth;
    inf.read((char*)RelBgData::relCompressedData,
             RelBgData::length);
    RelBgData::length<<=1;//����2
    bool swibit=false;
    uint8_t ut8,bit8,byte1;
    uint8_t *relDat=RelBgData::relCompressedData;//����ʹ��
    uint16_t bit16;
    uint32_t total=0,len=0;//һ����¼��ѹ�ۼƳ���һ����¼��ȡ������
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
                ut8=relDat[len];//�˴�����תΪ2bit����,ֻҪ��ֵ��Ϊ1
					//2bit���͵ĵڶ��ֽں͵����ֽ�ͬʱΪ0�Ż����,�ʽ�βΪ00 00
					//����ά��1bit����,��ڶ����ֽ��ٴ�Ϊ0�ͻ����
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
*���ȵõ���ѹ����,Ȼ�����ȡ�����ݵ�ַ�Ƿ���0x8000000

*��ȡ��һ���ֽ�,Ȼ����������=8,������һ,����Ƿ�Ϊ��,
*��Ϊ��,���ٴζ�ȡ��һ���ֽ�,Ȼ��������=8 ����
*����һ���ֽ��Ƿ�û��7λ
*û�еĻ���ȡ�ڶ����ֽ�д���ڴ� �ܳ���-1 ����ܳ����Ƿ�Ϊ0
*��Ϊ0�Ļ���һ���ֽ�����1h Ȼ���������-1,����Ƿ�Ϊ��
*����һ���ֽ��Ƿ�û��7λ...

*����һ���ֽ���7λ,��ȡ�ڶ����ֽ�,��������4�ټ�3 (ȥ����λ��+3)(len)
*�ٴζ�ȡ�ڶ����ֽ�ȥ����λ,Ȼ������8h,��ȡ�������ֽ�,orrǰ��
*����ĵڶ��ֽ�,��ֵ��1.(�ڴ���ǰƫ��ֵ)
*��ȡ��ǰ�ڴ��ǰ��(�ڴ���ǰƫ��),д����ǰ���ڴ��ַ,��ָ������
*len�ݼ�,ֻҪlen��Ϊ0,������д

*lenΪ���,��һ���ֽ�����1λ����ѭ��,ֻҪ8��ѭ����û����

*ÿ��д����ѭ��8��,ֱ���ﵽ1000h����
*���ֽ�λ�������˴�ʲôʱ��ʼ����֮ǰ���ڴ渴д.��Ȼ����Ϊ����ѭ��һ��

*�ڴ治��д�Ĳ���ֻ�ǵ��ֽڵĶ�дѭ��.���8��,Ȼ���ٴζ�ȡ��һ���ֽ�
**/

//���ݿ���rom�е���βҪ��4���� ����۲�����˵�(δȷ��)

void Lz77BgData::getLz77CompressData(ifstream &inf){
    inf.read((char*)&bgSize,4);
    inf.read((char*)&decompressedLen,4);
    inf.read((char*)Lz77BgData::lz77CompressedTileTable,
             Lz77BgData::decompressedLen>>8);
    uint16_t bit16;
    uint32_t definelen=Lz77BgData::decompressedLen>>8;//�����ܳ���
    uint32_t total=0,len=0;//��ѹ�ۼƳ��Ⱥ͵�ǰ��ȡλ��
    uint8_t bit8,byte1,ut8;
    uint8_t *lzDat=Lz77BgData::lz77CompressedTileTable;//����ʹ��
    while(total<=definelen){
        bit8=lzDat[len];    //��һ���ֽ�
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
    uint8_t *data=SpriteData::data;//����ʹ��
    for(uint8_t i=0;i<75;i+=3){
        if(data[i]==0xFF&&data[i+1]==0xFF&&data[i+2]==0xFF){
            SpriteData::length=i+3;
            break;
        }
    }
}

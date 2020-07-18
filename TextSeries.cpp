#include "TextSeries.h"
Texts::Texts(string rom,string path):readRom(rom),curPath(path){
    textStoryDataOfts.reserve(3);
    textStoryDataOfts[0].reserve(6);
    textStoryDataOfts[1].reserve(6);
    textStoryDataOfts[2].reserve(6);

    textDescriptionDataOfts.reserve(3);
    textDescriptionDataOfts[0].reserve(19);
    textDescriptionDataOfts[1].reserve(19);
    textDescriptionDataOfts[2].reserve(19);

    textLocationsDataOfts.reserve(3);
    textLocationsDataOfts[0].reserve(26);
    textLocationsDataOfts[1].reserve(26);
    textLocationsDataOfts[2].reserve(26);

    textMessagesDataOfts.reserve(3);
    textMessagesDataOfts[0].reserve(39);
    textMessagesDataOfts[1].reserve(39);
    textMessagesDataOfts[2].reserve(39);

    textScreenDataOfts.reserve(3);
    textScreenDataOfts[0].reserve(37);
    textScreenDataOfts[1].reserve(37);
    textScreenDataOfts[2].reserve(37);

    ifstream inf;
    uint32_t bit32,byte4,offset;
    //textͼƬ��ַ�õ�
    File::OpenFile(inf,readRom,true);
    inf.seekg(ldrTextGraphicOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextGraphic��ַ��ָ��!");
    }
    textGraphicOft=bit32<<5>>5;
    //�ַ�������ݵ�ַ�õ�
    inf.seekg(ldrCharacterWidthOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("CharacterWidth���ݵ�ַ��ָ��!");
    }
    characterWidthOft=bit32<<5>>5;
    //textstory���ݵ�ַȫ�õ�
    inf.seekg(ldrTextStoryLanguageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextStory���Ա��ַ��ָ��!");
    }
    bit32^=0x8000000;
    for(uint32_t i=0;i<3;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("TextStory���Ե�ַ��ָ��!");
        }
        byte4^=0x8000000;
        inf.seekg(byte4,ios::beg);
        for(uint32_t j=0;j<6;++j){
            inf.read((char*)&offset,4);
            if((offset>>25)!=4){
                DataException::DataError("TextStory���ݵ�ַ��ָ��!");
            }
            textStoryDataOfts[i].push_back(offset<<5>>5);
        }
    }
    //textdescription���ݵ�ַȫ�õ�
    inf.seekg(ldrTextDescriptionLanguageTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextDescription���Ա��ַ��ָ��!");
    }
    bit32^=0x8000000;
    for(uint32_t i=0;i<3;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("TextDescription���Ե�ַ��ָ��!");
        }
        byte4^=0x8000000;
        inf.seekg(byte4,ios::beg);
        for(uint32_t j=0;j<19;++j){
            inf.read((char*)&offset,4);
            if((offset>>25)!=4){
                DataException::DataError("TextDescription���ݵ�ַ��ָ��!");
            }
            textDescriptionDataOfts[i].push_back(offset<<5>>5);
        }
    }
    //textLocations���ݵ�ַȫ�õ�
    inf.seekg(ldrTextLocationsLanguageTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextLocations���Ա��ַ��ָ��!");
    }
    bit32^=0x8000000;
    for(uint32_t i=0;i<3;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("TextLocations���Ե�ַ��ָ��!");
        }
        byte4^=0x8000000;
        inf.seekg(byte4,ios::beg);
        for(uint32_t j=0;j<26;++j){
            inf.read((char*)&offset,4);
            if((offset>>25)!=4){
                DataException::DataError("TextLocations���ݵ�ַ��ָ��!");
            }
            textLocationsDataOfts[i].push_back(offset<<5>>5);
        }
    }
    //textMessages���ݵ�ַȫ�õ�
    inf.seekg(ldrTextMessagesLanguageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextMessages���Ա��ַ��ָ��!");
    }
    bit32^=0x8000000;
    for(uint32_t i=0;i<3;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("TextMessages���Ե�ַ��ָ��!");
        }
        byte4^=0x8000000;
        inf.seekg(byte4,ios::beg);
        for(uint32_t j=0;j<39;++j){
            inf.read((char*)&offset,4);
            if((offset>>25)!=4){
                DataException::DataError("TextMessages���ݵ�ַ��ָ��!");
            }
            textMessagesDataOfts[i].push_back(offset<<5>>5);
        }
    }
    //textScreen���ݵ�ַȫ�õ�
    inf.seekg(ldrTextScreenLanguageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextScreen���Ա��ַ��ָ��!");
    }
    bit32^=0x8000000;
    for(uint32_t i=0;i<3;++i){
        inf.seekg(bit32+i*4,ios::beg);
        inf.read((char*)&byte4,4);
        if((byte4>>25)!=4){
            DataException::DataError("TextScreen���Ե�ַ��ָ��!");
        }
        byte4^=0x8000000;
        inf.seekg(byte4,ios::beg);
        for(uint32_t j=0;j<37;++j){
            inf.read((char*)&offset,4);
            if((offset>>25)!=4){
                DataException::DataError("TextScreen���ݵ�ַ��ָ��!");
            }
            textScreenDataOfts[i].push_back(offset<<5>>5);
        }
    }
}
Texts::~Texts(){}
void Texts::MakeTextSeriesFolder(){
    File::makefolder(curPath+"Story");
    File::makefolder(curPath+"Description");
    File::makefolder(curPath+"Locations");
    File::makefolder(curPath+"Messages");
    File::makefolder(curPath+"Screen");
}
void Texts::TextDataOut(){
    ifstream inf;
    ofstream ouf;
    File::OpenFile(inf,readRom,true);
    string tes=curPath+"TextGeneralData.asm";
    File::MakeFile(ouf,tes,false);
    //��������
    ouf<<hex<<setiosflags(ios::uppcase)<<setfill('0')
    <<".org 0x"<<setw(7)<<(ldrTextGraphicOft[0]|0x8000000)
    <<"\n\t.word TextGraphicData\n.org 0x"
    <<setw(7)<<(ldrTextGraphicOft[1]|0x8000000)
    <<"\n\t.word TextGraphicData\n.org 0x"
    <<setw(7)<<(ldrTextGraphicOft[2]|0x8000000)
    <<"\n\t.word TextGraphicData\n.org 0x"
    <<setw(7)<<(ldrTextGraphicOft[3]|0x8000000)
    <<"\n\t.word TextGraphicData\n.org 0x"
    <<setw(7)<<(ldrTextGraphicOft[4]|0x8000000)
    <<"\n\t.word TextGraphicData\n.org 0x"
    <<setw(7)<<(ldrCharacterWidthOft|0x8000000)
    <<"\n\t.word CharacterWidthData"
    <<setw(7)<<(ldrTextStoryLanguageTableOft[0]|0x8000000)
    <<"\n\t.word TextStoryLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextStoryLanguageTableOft[1]|0x8000000)
    <<"\n\t.word TextStoryLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextDescriptionLanguageTableOft|0x8000000)
    <<"\n\t.word TextDescriptionLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextLocationsLanguageTableOft|0x8000000)
    <<"\n\t.word TextLocationsLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextMessagesLanguageTableOft[0]|0x8000000)
    <<"\n\t.word TextMessagesLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextMessagesLanguageTableOft[1]|0x8000000)
    <<"\n\t.word TextMessagesLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextMessagesLanguageTableOft[2]|0x8000000)
    <<"\n\t.word TextMessagesLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextMessagesLanguageTableOft[3]|0x8000000)
    <<"\n\t.word TextMessagesLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextMessagesLanguageTableOft[4]|0x8000000)
    <<"\n\t.word TextMessagesLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextScreenLanguageTableOft[0]|0x8000000)
    <<"\n\t.word TextScreenLanguageTable\n.org 0x"
    <<setw(7)<<(ldrTextScreenLanguageTableOft[0]|0x8000000)
    <<"\n\t.word TextScreenLanguageTable"<<endl;
    ouf.close();
    //����textͼƬ����
    DataListPrint tedlp;
    tedlp.explain="TextGraphic";
    tedlp.offset=textGraphicOft;
    tedlp.len=0x2C000;
    textdlp.push_back(tedlp);

    tes=curPath+"TextGraphic.raw";
    File::MakeFile(ouf,tes,true);
    inf.seekg(textGraphicOft,ios::beg);
    inf.read((char*)buffer,0x2C000);
    ouf.write((char*)buffer,0x2C000);
    ouf.close();
    //�����ַ��������
    tedlp.explain="CharacterWidth";
    tedlp.offset=characterWidthOft;
    tedlp.len=0x4A0;
    textdlp.push_back(tedlp);

    tes=curPath+"CharacterWidth.raw";
    File::MakeFile(ouf,tes,true);
    inf.seekg(characterWidthOft,ios::beg);
    inf.read((char*)buffer,0x4A0);
    ouf.write((char*)buffer,0x4A0);
    ouf.close();
    //����TextStoryTable
    tedlp.explain="TextStoryTable";
    tedlp.offset=


}

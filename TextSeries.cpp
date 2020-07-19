#include "TextSeries.h"
#include "HeaderSeriesOut.h"
Texts::Texts(string rom,string path):readRom(rom),curPath(path){
    ifstream inf;
    uint32_t bit32,byte4,offset;
    vector<uint32_t>tevec;
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
    textStoryLanguageTable=bit32<<5>>5;
    tevec.reserve(6);
    for(uint32_t i=0;i<3;++i){
        inf.seekg(textStoryLanguageTable+i*4,ios::beg);
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
            tevec.push_back(offset<<5>>5);
        }
        textStoryDataOfts.insert(pair<uint32_t,vector<uint32_t> >(byte4,tevec));
        tevec.clear();
    }
    //textdescription���ݵ�ַȫ�õ�
    inf.seekg(ldrTextDescriptionLanguageTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextDescription���Ա��ַ��ָ��!");
    }
    textDescriptionLanguageTable=bit32<<5>>5;
    tevec.reserve(19);
    for(uint32_t i=0;i<3;++i){
        inf.seekg(textDescriptionLanguageTable+i*4,ios::beg);
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
            tevec.push_back(offset<<5>>5);
        }
        textDescriptionDataOfts.insert(pair<uint32_t,vector<uint32_t> >(byte4,tevec));
        tevec.clear();
    }
    //textLocations���ݵ�ַȫ�õ�
    inf.seekg(ldrTextLocationsLanguageTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextLocations���Ա��ַ��ָ��!");
    }
    textLocationsLanguageTable=bit32<<5>>5;
    tevec.reserve(26);
    for(uint32_t i=0;i<3;++i){
        inf.seekg(textLocationsLanguageTable+i*4,ios::beg);
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
            tevec.push_back(offset<<5>>5);
        }
        textLocationsDataOfts.insert(pair<uint32_t,vector<uint32_t> >(byte4,tevec));
        tevec.clear();
    }
    //textMessages���ݵ�ַȫ�õ�
    inf.seekg(ldrTextMessagesLanguageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextMessages���Ա��ַ��ָ��!");
    }
    textMessagesLanguageTable=bit32<<5>>5;
    tevec.reserve(39);
    for(uint32_t i=0;i<3;++i){
        inf.seekg(textMessagesLanguageTable+i*4,ios::beg);
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
            tevec.push_back(offset<<5>>5);
        }
        textMessagesDataOfts.insert(pair<uint32_t,vector<uint32_t> >(byte4,tevec));
        tevec.clear();
    }
    //textScreen���ݵ�ַȫ�õ�
    inf.seekg(ldrTextScreenLanguageTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("TextScreen���Ա��ַ��ָ��!");
    }
    textScreenLanguageTable=bit32<<5>>5;
    tevec.reserve(37);
    for(uint32_t i=0;i<3;++i){
        inf.seekg(textScreenLanguageTable+i*4,ios::beg);
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
            tevec.push_back(offset<<5>>5);
        }
        textScreenDataOfts.insert(pair<uint32_t,vector<uint32_t> >(byte4,tevec));
        tevec.clear();
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
string Texts::LanguageStr(int num){
    switch(num){
    case 0:
        return "Japanese";
    case 1:
        return "Hiragana";
    case 2:
        return "English";
    default:
        return "";
    }
}
void Texts::TextDataOut(){
    ifstream inf;
    ofstream ouf,otf,odf;
    uint32_t bit32;
    textdlp.reserve(404);
    File::OpenFile(inf,readRom,true);
    string tes=curPath+"TextGeneralData.asm";
    File::MakeFile(ouf,tes,false);
    //��������
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
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
    <<"\n\t.word CharacterWidthData\n.org 0x"
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

    tes=curPath+"TextDataDef.asm";
    File::MakeFile(odf,tes,false);
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
    odf<<".align\nTextGraphicData:\n\t.import "
    <<"\"TextSeries\\TextGraphic.raw\""<<endl;

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
    odf<<".align\nCharacterWidthData:\n\t.import "
    <<"\"TextSeries\\CharacterWidth.raw\""<<endl;

    //����TextStoryLanguageTable
    tes=curPath+"TextLanguageTable.asm";
    File::MakeFile(ouf,tes,false);

    tedlp.explain="TextStoryLanguageTable";
    tedlp.len=28;//��������
    tedlp.offset=textStoryLanguageTable;
    textdlp.push_back(tedlp);

    tedlp.explain="TextDescriptionLanguageTable";
    tedlp.offset=textDescriptionLanguageTable;
    textdlp.push_back(tedlp);

    tedlp.explain="TextLocationsLanguageTable";
    tedlp.offset=textLocationsLanguageTable;
    textdlp.push_back(tedlp);

    tedlp.explain="TextMessagesLanguageTable";
    tedlp.offset=textMessagesLanguageTable;
    textdlp.push_back(tedlp);

    tedlp.explain="TextScreenLanguageTable";
    tedlp.offset=textScreenLanguageTable;
    textdlp.push_back(tedlp);

    ouf<<".align\n"<<"TextStoryLanguageTable:"
    <<"\n\t.word JapaneseTextStoryTable"
    <<"\n\t.word HiraganaTextStoryTable"
    <<"\n\t.word EnglishTextStoryTable"
    <<"\n\t.word EnglishTextStoryTable"
    <<"\n\t.word EnglishTextStoryTable"
    <<"\n\t.word EnglishTextStoryTable"
    <<"\n\t.word EnglishTextStoryTable"
    <<"\nTextDescriptionLanguageTable:"
    <<"\n\t.word JapaneseTextDescriptionTable"
    <<"\n\t.word HiraganaTextDescriptionTable"
    <<"\n\t.word EnglishTextDescriptionTable"
    <<"\n\t.word EnglishTextDescriptionTable"
    <<"\n\t.word EnglishTextDescriptionTable"
    <<"\n\t.word EnglishTextDescriptionTable"
    <<"\n\t.word EnglishTextDescriptionTable"
    <<"\nTextLocationsLanguageTable:"
    <<"\n\t.word JapaneseTextLocationsTable"
    <<"\n\t.word HiraganaTextLocationsTable"
    <<"\n\t.word EnglishTextLocationsTable"
    <<"\n\t.word EnglishTextLocationsTable"
    <<"\n\t.word EnglishTextLocationsTable"
    <<"\n\t.word EnglishTextLocationsTable"
    <<"\n\t.word EnglishTextLocationsTable"
    <<"\nTextMessagesLanguageTable:"
    <<"\n\t.word JapaneseTextMessagesTable"
    <<"\n\t.word HiraganaTextMessagesTable"
    <<"\n\t.word EnglishTextMessagesTable"
    <<"\n\t.word EnglishTextMessagesTable"
    <<"\n\t.word EnglishTextMessagesTable"
    <<"\n\t.word EnglishTextMessagesTable"
    <<"\n\t.word EnglishTextMessagesTable"
    <<"\nTextScreenLanguageTable:"
    <<"\n\t.word JapaneseTextScreenTable"
    <<"\n\t.word HiraganaTextScreenTable"
    <<"\n\t.word EnglishTextScreenTable"
    <<"\n\t.word EnglishTextScreenTable"
    <<"\n\t.word EnglishTextScreenTable"
    <<"\n\t.word EnglishTextScreenTable"
    <<"\n\t.word EnglishTextScreenTable"
    <<endl;
    ouf.close();
    //���Ƹ�����Table,���������Ͷ���
    tes=curPath+"TextDataTable.asm";
    File::MakeFile(otf,tes,false);
    map<uint32_t,vector<uint32_t> >::iterator it;
    vector<uint32_t>::iterator itv;
    //Story����
    int addit=0;
    for(it=textStoryDataOfts.begin();it!=textStoryDataOfts.end();++it){
        tes=LanguageStr(addit);
        tedlp.explain=tes+"TextStoryTable";
        tedlp.len=it->second.size()*4;
        tedlp.offset=it->first;
        textdlp.push_back(tedlp);
        otf<<".align\n"<<tedlp.explain<<":\n";
        ++addit;
        for(itv=it->second.begin();itv!=it->second.end();++itv){
            inf.seekg(*itv,ios::beg);
            inf.read((char*)doubuff,0x300);
            bit32=0;
            while(true){
                if(doubuff[bit32]==0xFF00){
                    ++bit32;
                    break;
                }
                ++bit32;
            }
            tedlp.explain="Text_"+HeaderSeriesOut::numToHexStr(*itv,7);
            tedlp.offset=*itv;
            tedlp.len=bit32*2;
            textdlp.push_back(tedlp);

            tes=curPath+"Story\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            ouf.write((char*)doubuff,tedlp.len);
            ouf.close();

            otf<<"\t.word "<<tedlp.explain<<endl;
            odf<<".align\n"<<tedlp.explain
            <<":\n\t.import \"TextSeries\\Story\\"
            <<tedlp.explain<<"\""<<endl;
        }
    }
    //Description����
    addit=0;
    for(it=textDescriptionDataOfts.begin();it!=textDescriptionDataOfts.end();++it){
        tes=LanguageStr(addit);
        tedlp.explain=tes+"TextDescriptionTable";
        tedlp.len=it->second.size()*4;
        tedlp.offset=it->first;
        textdlp.push_back(tedlp);
        otf<<".align\n"<<tedlp.explain<<":\n";
        ++addit;
        for(itv=it->second.begin();itv!=it->second.end();++itv){
            inf.seekg(*itv,ios::beg);
            inf.read((char*)doubuff,0x300);
            bit32=0;
            while(true){
                if(doubuff[bit32]==0xFF00){
                    ++bit32;
                    break;
                }
                ++bit32;
            }
            tedlp.explain="Text_"+HeaderSeriesOut::numToHexStr(*itv,7);
            tedlp.offset=*itv;
            tedlp.len=bit32*2;
            textdlp.push_back(tedlp);

            tes=curPath+"Description\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            ouf.write((char*)doubuff,tedlp.len);
            ouf.close();

            otf<<"\t.word "<<tedlp.explain<<endl;
            odf<<".align\n"<<tedlp.explain
            <<":\n\t.import \"TextSeries\\Description\\"
            <<tedlp.explain<<"\""<<endl;
        }
    }
    //Locations����
    addit=0;
    for(it=textLocationsDataOfts.begin();it!=textLocationsDataOfts.end();++it){
        tes=LanguageStr(addit);
        tedlp.explain=tes+"TextLocationsTable";
        tedlp.len=it->second.size()*4;
        tedlp.offset=it->first;
        textdlp.push_back(tedlp);
        otf<<".align\n"<<tedlp.explain<<":\n";
        ++addit;
        for(itv=it->second.begin();itv!=it->second.end();++itv){
            inf.seekg(*itv,ios::beg);
            inf.read((char*)doubuff,0x300);
            bit32=0;
            while(true){
                if(doubuff[bit32]==0xFF00){
                    ++bit32;
                    break;
                }
                ++bit32;
            }
            tedlp.explain="Text_"+HeaderSeriesOut::numToHexStr(*itv,7);
            tedlp.offset=*itv;
            tedlp.len=bit32*2;
            textdlp.push_back(tedlp);

            tes=curPath+"Locations\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            ouf.write((char*)doubuff,tedlp.len);
            ouf.close();

            otf<<"\t.word "<<tedlp.explain<<endl;
            odf<<".align\n"<<tedlp.explain
            <<":\n\t.import \"TextSeries\\Locations\\"
            <<tedlp.explain<<"\""<<endl;
        }
    }
    //Messages����
    addit=0;
    for(it=textMessagesDataOfts.begin();it!=textMessagesDataOfts.end();++it){
        tes=LanguageStr(addit);
        tedlp.explain=tes+"TextMessagesTable";
        tedlp.len=it->second.size()*4;
        tedlp.offset=it->first;
        textdlp.push_back(tedlp);
        otf<<".align\n"<<tedlp.explain<<":\n";
        ++addit;
        for(itv=it->second.begin();itv!=it->second.end();++itv){
            inf.seekg(*itv,ios::beg);
            inf.read((char*)doubuff,0x300);
            bit32=0;
            while(true){
                if(doubuff[bit32]==0xFF00){
                    ++bit32;
                    break;
                }
                ++bit32;
            }
            tedlp.explain="Text_"+HeaderSeriesOut::numToHexStr(*itv,7);
            tedlp.offset=*itv;
            tedlp.len=bit32*2;
            textdlp.push_back(tedlp);

            tes=curPath+"Messages\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            ouf.write((char*)doubuff,tedlp.len);
            ouf.close();

            otf<<"\t.word "<<tedlp.explain<<endl;
            odf<<".align\n"<<tedlp.explain
            <<":\n\t.import \"TextSeries\\Messages\\"
            <<tedlp.explain<<"\""<<endl;
        }
    }
    //Screen����
    addit=0;
    for(it=textScreenDataOfts.begin();it!=textScreenDataOfts.end();++it){
        tes=LanguageStr(addit);
        tedlp.explain=tes+"TextScreenTable";
        tedlp.len=it->second.size()*4;
        tedlp.offset=it->first;
        textdlp.push_back(tedlp);
        otf<<".align\n"<<tedlp.explain<<":\n";
        ++addit;
        for(itv=it->second.begin();itv!=it->second.end();++itv){
            inf.seekg(*itv,ios::beg);
            inf.read((char*)doubuff,0x300);
            bit32=0;
            while(true){
                if(doubuff[bit32]==0xFF00){
                    ++bit32;
                    break;
                }
                ++bit32;
            }
            tedlp.explain="Text_"+HeaderSeriesOut::numToHexStr(*itv,7);
            tedlp.offset=*itv;
            tedlp.len=bit32*2;
            textdlp.push_back(tedlp);

            tes=curPath+"Screen\\"+tedlp.explain;
            File::MakeFile(ouf,tes,true);
            ouf.write((char*)doubuff,tedlp.len);
            ouf.close();

            otf<<"\t.word "<<tedlp.explain<<endl;
            odf<<".align\n"<<tedlp.explain
            <<":\n\t.import \"TextSeries\\Screen\\"
            <<tedlp.explain<<"\""<<endl;
        }
    }
    inf.close();
    odf.close();
    otf.close();
}

#include "ConnectionsSeries.h"
#include "HeaderSeriesOut.h"

ConnectionsSeries::ConnectionsSeries(string name,string path)
                    :readRomRoute(name),connectionsPath(path){
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    uint32_t bit32;
    inf.seekg(ldrAreaDoorPointerTableOft[0],ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("����ָ�����ݵ�ַ����ָ��!");
    }
    bit32^=0x8000000;
    DataListPrint tedlp;
    tedlp.explain="AreaDoorPointersTable";
    tedlp.len=28;
    tedlp.offset=bit32;
    connectionsDLP.push_back(tedlp);
    //��¼��������ָ���
    inf.seekg(bit32,ios::beg);
    for(uint8_t i=0;i<7;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            bit32=inf.tellg();
            bit32-=4;
            DataException::DataError("�������ݵ�ַ����ָ��!",bit32);
        }
        bit32^=0x8000000;
        areaDoorPointers.push_back(bit32);
    }

    inf.seekg(ldrAreaHatchLockTableOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("���������ݵ�ַ���Ǻϸ��ָ��!");
    }
    bit32^=0x8000000;
    //��¼����lock����ָ���
    inf.seekg(bit32,ios::beg);
    tedlp.explain="AreaHatchDoorPointerTable";
    tedlp.offset=bit32;
    tedlp.len=28;
    connectionsDLP.push_back(tedlp);

    for(uint8_t i=0;i<7;++i){
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            bit32=inf.tellg();
            bit32-=4;
            DataException::DataError("����Lock���ݵ�ַ����ָ��!",bit32);
        }
        bit32^=0x8000000;
        areaHatchLockPointer.push_back(bit32);
    }
    //����������¼
    inf.seekg(ldrNumberOfAreaHatchLock,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("����������ַ��ָ��!");
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    tedlp.explain="AreaHatchLockNumberTable";
    tedlp.len=14;
    tedlp.offset=bit32;
    connectionsDLP.push_back(tedlp);
    for(uint8_t i=0;i<7;++i){
        bit32=0;
        inf.read((char*)&bit32,2);
        if(bit32>0xFF){
            DataException::DataError("���������������ܳ�255?");
        }
        numberOfAreaHatchLock.push_back(bit32);
    }

    inf.seekg(modifiedDoorFunctionDataOft,ios::beg);
    bit32=0;
    inf.read((char*)&bit32,1);
    if(bit32==6){
        doorLabelModified=true;//�����޸ı��Ϊ��
        inf.seekg(numberOfmodifiedEventDoorOft,ios::beg);
        inf.read((char*)&bit32,4);
        if(bit32>0xFF){
            DataException::DataError("�¼��������ܳ�255?");
        }
        defaultNumberOfEventDoor=bit32;
    }
    inf.close();
}
/**
*0x8000000���Ĵ�����ת,�����Ƽ���Ϊ:
*��λȡ11λ,��λȡ11λ,Ȼ���λ����1λ,��λ����12λ,���߻�
*��ֵ����ָ������+4,����23λ,��Ϊ��ת��λ��
**/
uint32_t ConnectionsSeries::CallPointerCalculate(ifstream &inf,uint32_t blOffset){
    uint32_t bit32;
    uint16_t high,low;
    inf.seekg(blOffset,ios::beg);
    inf.read((char*)&bit32,4);
    high=(bit32&0x7FF0000)>>16;
    low=bit32&0x7FF;
    bit32=(high<<12)|(low<<1);
    return (blOffset+4+bit32)&0xFF7FFFFF;
}

ConnectionsSeries::~ConnectionsSeries(){}
//void ConnectionsSeries::MakeConnectionsFolder(){}
void ConnectionsSeries::ConnectionsSeriesDataOut(){
    uint32_t bit32;
    ifstream inf(readRomRoute,ios::in|ios::binary);
    if(inf.fail()){
        DataException::FileError(readRomRoute,1);
    }
    string tes=connectionsPath+"ConnectionGeneralData.asm";
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[0]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[1]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[2]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[3]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[4]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[5]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaDoorPointerTableOft[6]|0x8000000)
    <<"\n\t.word AreaDoorPointerTable\n.org "<<setw(7)
    <<(int)(ldrAreaHatchLockTableOft|0x8000000)
    <<"\n\t.word AreaHatchLockTable\n.org "<<setw(7)
    <<(int)(ldrNumberOfAreaHatchLock|0x8000000)
    <<"\n\t.word NumberOfAreaHatchLock\n.org "<<setw(7)
    <<(int)(ldrDoorAreaTransferDataOft|0x8000000)
    <<"\n\t.word DoorAreaTransferData\n.org "<<setw(7)
    <<(int)(ldrEventDoorDataOft|0x8000000)
    <<"\n\t.word EventDoorData"<<endl;
    if(doorLabelModified){
        ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
        <<"\n.org "<<setw(7)
        <<(int)(ldrModifiedLabelDoorDataOft|0x8000000)
        <<"\n\t.word LabelDoorData\n.org "<<setw(7)
        <<(int)(doorLabelBLmodifiedOft|0x8000000)
        <<"\n\t.db 0xFF,0xF7,0xD0,0xFC,0x2E,0xE0\n.org "<<setw(7)
        <<(int)(modifiedDoorFunctionDataOft|0x8000000)
        <<"\n\t.import \"ConnectionsSeries\\ModifiedDoorFunctionData.bin\""
        <<endl;
    }
    ouf.close();
    DataListPrint tedlp;
    //�Ǳ���ĵ���bin
    tes=connectionsPath+"EventDoorData.bin";
    //�¼������ݵ���
    ouf.open(tes,ios::out|ios::binary);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    inf.seekg(ldrEventDoorDataOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("�¼��ŵ�ַ����ָ��!");
    }
    bit32^=0x8000000;
    inf.seekg(bit32,ios::beg);
    tedlp.explain="EventDoorData";
    tedlp.offset=bit32;
    bit32=4*defaultNumberOfEventDoor;
    tedlp.len=bit32;
    connectionsDLP.push_back(tedlp);

    inf.read((char*)buffer,bit32);
    ouf.write((char*)buffer,bit32);
    ouf.close();
    //�����������ݵ���
    tes=connectionsPath+"DoorAreaTransferData.bin";
    ouf.open(tes,ios::out|ios::binary);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    inf.seekg(ldrDoorAreaTransferDataOft,ios::beg);
    inf.read((char*)&bit32,4);
    if((bit32>>25)!=4){
        DataException::DataError("�������ݵ�ַ����ָ��!");
    }
    bit32^=0x8000000;
    tedlp.explain="DoorAreaTransferData";
    tedlp.offset=bit32;
    inf.seekg(bit32,ios::beg);
    inf.read((char*)buffer,0x300);//��ȡFF������
    uint32_t k=2;
    for(;k<0x300;k+=3){
        if(buffer[k-2]==0xFF&&buffer[k-1]==0xFF&&buffer[k]==0xFF){
            break;
        }
    }
    tedlp.len=(++k);
    ouf.write((char*)buffer,tedlp.len);
    connectionsDLP.push_back(tedlp);
    ouf.close();
    //�����������ݵ���
    tes=connectionsPath+"NumberOfAreaHatchLock.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<hex<<setiosflags(ios::uppercase)<<setfill('0')
    <<".align\nNumberOfAreaHatchLock:\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[0])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[1])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[2])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[3])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[4])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[5])<<"\n\t.dh 0x"
    <<setw(4)<<int(numberOfAreaHatchLock[6])<<endl;
    ouf.close();
    //�ű�ǩ���ݵ���
    if(doorLabelModified){
        tes=connectionsPath+"LabelDoorData.bin";
        ouf.open(tes,ios::out|ios::binary);
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }
        inf.seekg(ldrModifiedLabelDoorDataOft,ios::beg);
        inf.read((char*)&bit32,4);
        if((bit32>>25)!=4){
            DataException::DataError("�ű�ǩ���ݵ�ַ��ָ��!");
        }
        bit32^=0x8000000;
        tedlp.explain="LabelDoorData";
        tedlp.offset=bit32;
        inf.seekg(bit32,ios::beg);
        inf.read((char*)buffer,0x300);//��ȡFF������
        k=2;
        for(;k<0x300;k+=3){
            if(buffer[k-2]==0xFF&&buffer[k-1]==0xFF&&buffer[k]==0xFF){
                break;
            }
        }
        tedlp.len=(++k);
        ouf.write((char*)buffer,tedlp.len);
        connectionsDLP.push_back(tedlp);
        ouf.close();
        //�����޸����ݵ���
        tes=connectionsPath+"ModifiedDoorFunctionData.bin";
        ouf.open(tes,ios::out|ios::binary);
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }
        tedlp.explain="ModifiedDoorFunctionData";
        tedlp.offset=modifiedDoorFunctionDataOft;
        tedlp.len=0x48;
        connectionsDLP.push_back(tedlp);
        inf.seekg(modifiedDoorFunctionDataOft,ios::beg);
        inf.read((char*)buffer,72);
        ouf.write((char*)buffer,72);
        ouf.close();
    }
    //�ļ����������bin(�����ź�������)
    tes=connectionsPath+"DoorBinDataDef.asm";
    ouf.open(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    ouf<<".align\nEventDoorData:\n\t.import \"ConnectionsSeries\\\
EventDoorData.bin\"\n.align\nDoorAreaTransferData:\n\t.import \
\"ConnectionsSeries\\DoorAreaTransferData.bin\"\n.align\n\
\t.include \"ConnectionsSeries\\NumberOfAreaHatchLock.asm\""<<endl;
    if(doorLabelModified){
        ouf<<".align\nLabelDoorData:\n\t.import \"ConnectionsSeries\\\
LabelDoorData.bin\""<<endl;
    }
    ouf.close();
    //�����ݵ���
    tes=connectionsPath+"AreaDoorDataDef.asm";
    ofstream oudef(tes,ios::out);
    if(oudef.fail()){
        DataException::FileError(tes,0);
    }
    for(uint32_t i=0;i<7;++i){
        bit32=areaDoorPointers[i];
        inf.seekg(bit32,ios::beg);
        inf.read((char*)buffer,0xC00);
        uint32_t j=0xB;
        for(;j<0xC00;j+=0xC){
            if(buffer[j-11]==0&&buffer[j-10]==0&&
               buffer[j-9]==0&&buffer[j-8]==0&&
               buffer[j-7]==0&&buffer[j-6]==0&&
               buffer[j-5]==0&&buffer[j-4]==0&&
               buffer[j-3]==0&&buffer[j-2]==0&&
               buffer[j-1]==0&&buffer[j]==0){
                break;
               }
        }
        tedlp.explain="Area"+HeaderSeriesOut::numToHexStr(i,2)+"DoorData";
        oudef<<".align\n"<<tedlp.explain<<":\n\t.import \"connections\\"
        <<tedlp.explain<<".bin\""<<endl;
        tedlp.len=(++j);
        tedlp.offset=bit32;
        connectionsDLP.push_back(tedlp);
        tes=connectionsPath+tedlp.explain+"bin";
        ouf.open(tes,ios::out|ios::binary);
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }
        ouf.write((char*)buffer,tedlp.len);
        ouf.close();
    }
    oudef.close();
    //������������
    tes=connectionsPath+"AreaHatchLockDataDef.asm";
    oudef.open(tes,ios::out);
    if(oudef.fail()){
        DataException::FileError(tes,0);
    }
    for(uint32_t i=0;i<7;++i){
        tedlp.offset=areaHatchLockPointer[i];
        tedlp.explain="Area"+HeaderSeriesOut::numToHexStr(i,2)+"HatchLockData";
        oudef<<".align\n"<<tedlp.explain<<":\n\t.import \"ConnectionsSeries\\"
        <<tedlp.explain<<".bin\""<<endl;
        tes=connectionsPath+tedlp.explain+".bin";
        ouf.open(tes,ios::out|ios::binary);
        if(ouf.fail()){
            DataException::FileError(tes,0);
        }
        bit32=numberOfAreaHatchLock[i]*8;
        tedlp.len=bit32;
        connectionsDLP.push_back(tedlp);
        inf.seekg(tedlp.offset,ios::beg);
        inf.read((char*)buffer,bit32);
        ouf.write((char*)buffer,bit32);
        ouf.close();
    }
    oudef.close();
}

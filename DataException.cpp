#include "DataException.h"
#include "HeaderSeriesOut.h"

void DataException::PrintAndEnd(const string &s){
    cerr<<s;
    cin.get();
    exit(-1);
}

void DataException::DataError(){
    PrintAndEnd("δ����Ĵ���!");
}
void DataException::DataError(const string &s){
    PrintAndEnd(s);
}
void DataException::DataError(const string &s,const uint32_t address){
    PrintAndEnd("�쳣! "+s+" λ��: 0x"+HeaderSeriesOut::numToHexStr(address,7));
}
void DataException::FileError(const string &file,const int errorNum){
    switch(errorNum){
    case 0:
        PrintAndEnd("�ļ�: \""+file+"\"�޷�����!");
    case 1:
        PrintAndEnd("�ļ�: \""+file+"\"�޷���ȡ!");
    case 2:
        PrintAndEnd("û�������κ��ļ�! ZeroAnatomy v1.0 2020/6/29 by XXXXXX-Diwa");
    case 3:
        PrintAndEnd("�ļ�: \""+file+"\"����쳣!");
    default:
        PrintAndEnd("δ������ļ�������!");
    }
}

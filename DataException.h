#ifndef DATAEXCEPTION_H_INCLUDED
#define DATAEXCEPTION_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <sstream>
//#include "HeaderSeriesOut.h"
using namespace std;
class DataException{
private:
    static void PrintAndEnd(const string &s){
        cerr<<s;
        cin.get();
        exit(1);
    }
public:
    static void DataError(){
        PrintAndEnd("δ����Ĵ���!");
    }
    static void DataError(const string &s){
        PrintAndEnd(s);
    }
    static void DataError(const string &s,const uint32_t address){
        PrintAndEnd("�쳣! "+s+" λ��: "+to_string(address));
    }
    static void FileError(const string &file,const int errorNum){
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

};


#endif // DATAEXCEPTION_H_INCLUDED

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
        PrintAndEnd("未定义的错误!");
    }
    static void DataError(const string &s){
        PrintAndEnd(s);
    }
    static void DataError(const string &s,const uint32_t address){
        PrintAndEnd("异常! "+s+" 位置: "+to_string(address));
    }
    static void FileError(const string &file,const int errorNum){
        switch(errorNum){
        case 0:
            PrintAndEnd("文件: \""+file+"\"无法创建!");
        case 1:
            PrintAndEnd("文件: \""+file+"\"无法读取!");
        case 2:
            PrintAndEnd("没有载入任何文件! ZeroAnatomy v1.0 2020/6/29 by XXXXXX-Diwa");
        case 3:
            PrintAndEnd("文件: \""+file+"\"规格异常!");
        default:
            PrintAndEnd("未定义的文件错误编号!");
        }
    }

};


#endif // DATAEXCEPTION_H_INCLUDED

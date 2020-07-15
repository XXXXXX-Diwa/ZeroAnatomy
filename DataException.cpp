#include "DataException.h"
#include "HeaderSeriesOut.h"

void DataException::PrintAndEnd(const string &s){
    cerr<<s;
    cin.get();
    exit(-1);
}

void DataException::DataError(){
    PrintAndEnd("未定义的错误!");
}
void DataException::DataError(const string &s){
    PrintAndEnd(s);
}
void DataException::DataError(const string &s,const uint32_t address){
    PrintAndEnd("异常! "+s+" 位置: 0x"+HeaderSeriesOut::numToHexStr(address,7));
}
void DataException::FileError(const string &file,const int errorNum){
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

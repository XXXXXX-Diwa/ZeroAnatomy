#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <io.h>
#include <map>
#include <chrono>
#include "DataException.h"
#include "RunTimes.h"

using namespace std;

struct DataListPrint{
    uint32_t offset;
    uint32_t len;
    string explain;
};

class File{
    public:
    string FileRoute;   //全名
    string FilePath;    //目录
    string FileName;    //仅文件名
    vector<DataListPrint> allPrint;
    vector<string> armf;

    static void makefolder(string s);
    static void OpenFile(ifstream &in,string &s,bool bin);
    static void MakeFile(ofstream &out,string &s,bool bin);
    File(string s);
    void MakeMainAsmFile();
    static bool MyCompare(const DataListPrint &D1,const DataListPrint &D2);
    static bool MyIque(const DataListPrint &D1,const DataListPrint &D2);
    void PrintList();
    void AnsiToUtf8(string&);
    void GetAsmFileName(string path);
    void FileToFile();
    ~File();
};


#endif //MAIN_H

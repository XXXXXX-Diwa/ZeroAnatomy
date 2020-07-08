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
#include "DataException.h"

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

    static void makefolder(string s);
    File(string s);
    void MakeMainAsmFile();
    void PrintList();
    ~File();
};


#endif //MAIN_H

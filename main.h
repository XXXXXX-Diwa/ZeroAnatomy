#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>
#include <fstream>
#include <io.h>
#include "StructSet.h"

using namespace std;

struct DataListPrint{
    uint32_t offset;
    uint32_t len;
    string explain;
};

class File{
    public:
    string FileRoute;   //ȫ��
    string FilePath;    //Ŀ¼
    string FileName;    //���ļ���
    vector<DataListPrint> allPrint;

    static void FileFail(string &fileName);
    static void AuthorInfo();
    static void makefolder(string s);
    File(string s);
    void MakeMainAsmFile();
    void MakeAllFolder();
    ~File();
};


#endif //MAIN_H

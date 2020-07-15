#ifndef DATAEXCEPTION_H_INCLUDED
#define DATAEXCEPTION_H_INCLUDED

#include "main.h"
using namespace std;

class DataException{
private:
    static void PrintAndEnd(const string &s);
    DataException();
    ~DataException();
public:
    static void DataError();
    static void DataError(const string &s);
    static void DataError(const string &s,const uint32_t address);
    static void FileError(const string &file,const int errorNum);

};


#endif // DATAEXCEPTION_H_INCLUDED

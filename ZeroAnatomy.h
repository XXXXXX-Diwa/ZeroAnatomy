#ifndef ZEROANATOMY_H
#define ZEROANATOMY_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <sstream>

using namespace std;
class File{
    public:
    string FileRoute;   //全名
    string FilePath;    //目录
    string FileName;    //仅文件名
    static void AuthorInfo(){
        cerr<<"ZeroMission Anatomy v1.0 2020/6/24 by XXXXXX-Diwa";
        exit(1);
    }
    File(string s):FileRoute(s){
        size_t pos=s.find_last_of('\\');
        if(pos>0&&pos!=string::npos){
            FilePath=s.substr(0,pos);
            s=s.substr(pos+1);
        }else{
            FilePath="";
        }
        pos=s.find_last_of('.');
        if(pos>0&&pos!=string::npos){
            FileName=s.substr(0,pos);
        }else{
            AuthorInfo();
        }  
    }
    ~File(){}
};


#endif //ZEROANATOMY_H
#ifndef RUNTIMES_H_INCLUDED
#define RUNTIMES_H_INCLUDED
#include "main.h"

using namespace std::chrono;

class RunTimes{
private:
    static bool startflag;
    steady_clock::time_point run;
    steady_clock::time_point close;
    //构造函数私有化
    RunTimes();
    //析构函数私有化
    ~RunTimes();
    //拷贝构造函数私有化
    RunTimes(const RunTimes &runtime);
    //指针对象私有化
    static RunTimes *ms_runtime;
    //赋值操作私有化
    void operator=(const RunTimes &runtime);
    void PrintTime();
    void PrintTime(std::string &);
    static void DeleteRunTime();
public:
    static RunTimes *SharedRunTime();
    void RunBegin();
    void RunFinish();
    void RunFinish(std::string );

};

#endif // RUNTIMES_H_INCLUDED

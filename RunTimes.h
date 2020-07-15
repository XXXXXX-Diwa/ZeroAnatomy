#ifndef RUNTIMES_H_INCLUDED
#define RUNTIMES_H_INCLUDED
#include "main.h"

using namespace std::chrono;

class RunTimes{
private:
    static bool startflag;
    steady_clock::time_point run;
    steady_clock::time_point close;
    //���캯��˽�л�
    RunTimes();
    //��������˽�л�
    ~RunTimes();
    //�������캯��˽�л�
    RunTimes(const RunTimes &runtime);
    //ָ�����˽�л�
    static RunTimes *ms_runtime;
    //��ֵ����˽�л�
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

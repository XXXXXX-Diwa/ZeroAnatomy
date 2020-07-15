#include "RunTimes.h"
//静态变量初始化
bool RunTimes::startflag=false;
RunTimes *RunTimes::ms_runtime=NULL;
RunTimes *RunTimes::SharedRunTime(){
    //线程非安全
    if(ms_runtime==NULL){
        ms_runtime=new RunTimes();
    }
    ms_runtime->RunBegin();
    return ms_runtime;
}
RunTimes::RunTimes(){}
RunTimes::~RunTimes(){}
void RunTimes::DeleteRunTime(){
    //线程非安全
    if(ms_runtime!=NULL){
        delete ms_runtime;
        ms_runtime=NULL;
    }
}
void RunTimes::RunBegin(){
    if(startflag){
        DataException::DataError("上次要查看的时间还没有输出呢!");
    }
    startflag=true;
    run=steady_clock::now();
}

void RunTimes::RunFinish(){
    if(!startflag){
        DataException::DataError("还没有开始计时呢!");
    }
    startflag=false;
    close=steady_clock::now();
    PrintTime();
}
void RunTimes::PrintTime(){
    duration<double> runtime=
    duration_cast<duration<double> >(close-run);
    cout<<"运行时间为:"<<dec
    <<runtime.count()<<endl;
    DeleteRunTime();
}
void RunTimes::RunFinish(std::string s){
    if(!startflag){
        DataException::DataError(s+"还没有开始计时呢!");
    }
    startflag=false;
    close=steady_clock::now();
    PrintTime(s);
}
void RunTimes::PrintTime(std::string &s){
    steady_clock::duration runtime=close-run;
    cout<<s<<"运行时间为: "<<dec
    <<duration_cast<microseconds>(runtime).count()
    <<" 微秒!"<<endl;
}

#include "RunTimes.h"
//��̬������ʼ��
bool RunTimes::startflag=false;
RunTimes *RunTimes::ms_runtime=NULL;
RunTimes *RunTimes::SharedRunTime(){
    //�̷߳ǰ�ȫ
    if(ms_runtime==NULL){
        ms_runtime=new RunTimes();
    }
    ms_runtime->RunBegin();
    return ms_runtime;
}
RunTimes::RunTimes(){}
RunTimes::~RunTimes(){}
void RunTimes::DeleteRunTime(){
    //�̷߳ǰ�ȫ
    if(ms_runtime!=NULL){
        delete ms_runtime;
        ms_runtime=NULL;
    }
}
void RunTimes::RunBegin(){
    if(startflag){
        DataException::DataError("�ϴ�Ҫ�鿴��ʱ�仹û�������!");
    }
    startflag=true;
    run=steady_clock::now();
}

void RunTimes::RunFinish(){
    if(!startflag){
        DataException::DataError("��û�п�ʼ��ʱ��!");
    }
    startflag=false;
    close=steady_clock::now();
    PrintTime();
}
void RunTimes::PrintTime(){
    duration<double> runtime=
    duration_cast<duration<double> >(close-run);
    cout<<"����ʱ��Ϊ:"<<dec
    <<runtime.count()<<endl;
    DeleteRunTime();
}
void RunTimes::RunFinish(std::string s){
    if(!startflag){
        DataException::DataError(s+"��û�п�ʼ��ʱ��!");
    }
    startflag=false;
    close=steady_clock::now();
    PrintTime(s);
}
void RunTimes::PrintTime(std::string &s){
    steady_clock::duration runtime=close-run;
    cout<<s<<"����ʱ��Ϊ: "<<dec
    <<duration_cast<microseconds>(runtime).count()
    <<" ΢��!"<<endl;
}

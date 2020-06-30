#include "main.h"
#include "HeaderSeriesOut.h"

void File::makefolder(string s){
    if(access(s.c_str(),0)==-1){
        mkdir(s.c_str());
    }
}

File::File(string s):FileRoute(s){
    if(access(s.c_str(),0)==-1){
        DataException::FileError(s,2);
    }
    size_t pos=s.find_last_of('\\');
    if(pos>0&&pos!=string::npos){
        FilePath=s.substr(0,pos+1);
        s=s.substr(pos+1);
    }else{
        FilePath="";
    }
    pos=s.find_last_of('.');
    if(pos>0&&pos!=string::npos){
        FileName=s.substr(0,pos);
    }else{
        DataException::FileError(s,2);
    }
    makefolder(FilePath+"HeaderSeries");//����Header�����ļ���
}
File::~File(){

}

void File::MakeMainAsmFile(){
    string tes=FilePath+"ZeroAnatomy.asm";
    ofstream mf(tes,ios::out);//�������ļ�
    if(mf.fail()){
        DataException::FileError(tes,0);
    }

    mf<<".definelabel ZeroAnatomyFreeDataOffset,0x8770000\n"<<endl;
    //ռ�ò���
    mf<<".include HeaderSeries\\HeadGerenalData.asm"<<endl;
    //�Զ��岿��
    mf<<".org ZeroAnatomyFreeDataOffset"<<endl;
    mf<<".include HeaderSeries\\HeadCustomData.asm"<<endl;
    mf.close();
}

int main(int argc,char* const argv[]){
    ios::sync_with_stdio(false);
    unique_ptr<File>readFile;
    if(argc==1){
        DataException::FileError("",2);
    }else{
        readFile=make_unique<File>(argv[1]);
    }
    readFile->MakeMainAsmFile();//����ʹ�ӡ���ļ�
    //head���ݲ���
    unique_ptr<HeaderSeriesOut> hso(new HeaderSeriesOut(readFile->FileRoute,
                                    readFile->FilePath+"HeaderSeries\\"));
    hso->MakeHeaderSeriesFolders();//����Header����������ļ���
    hso->HeadAllDataOut();     //Header���ݵ���
    readFile->allPrint.insert(readFile->allPrint.end(),//Headϵ�е���Ϣ���ݽ������洢
        hso->headSeriesDLP.begin(),hso->headSeriesDLP.end());
    hso.reset();

    readFile.reset();

    return 0;
}

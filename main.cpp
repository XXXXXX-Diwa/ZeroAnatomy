#include "main.h"
#include "HeaderSeriesOut.h"


void File::AuthorInfo(){
    cerr<<"ZeroMission Anatomy v1.0 2020/6/24 by XXXXXX-Diwa";
    cin.get();
    exit(1);
}
void File::makefolder(string s){
    if(access(s.c_str(),0)==-1){
        mkdir(s.c_str());
    }
}

File::File(string s):FileRoute(s){
    if(access(s.c_str(),0)==-1){
        AuthorInfo();
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
        AuthorInfo();
    }
}
File::~File(){

}

void File::MakeMainAsmFile(){
    string tes=FilePath+"ZeroAnatomy.asm";
    ofstream mf(tes,ios::out);//创建主文件
    if(mf.fail()){
        FileFail(tes);
    }

    mf<<".definelabel ZeroAnatomyFreeDataOffset,0x8770000\n"<<endl;
    //占用部分
    mf<<".include HeaderSeries\\HeadGerenalData.asm"<<endl;
    //自定义部分
    mf<<".org ZeroAnatomyFreeDataOffset"<<endl;
    mf<<".include HeaderSeries\\HeadCustomData.asm"<<endl;
    mf.close();
}

void File::MakeAllFolder(){
    makefolder(FilePath+"HeaderSeries");//创建Header数据文件夹
}

void File::FileFail(string &fileName){
    throw "无法创建或读取文件: \""+fileName+"\"!";
}

int main(int argc,char* const argv[]){
    try{
        ios::sync_with_stdio(false);
        unique_ptr<File>readFile;
        if(argc==1){
            File::AuthorInfo();
        }else{
            readFile=make_unique<File>(argv[1]);
        }
        readFile->MakeMainAsmFile();//制造和打印主文件
        readFile->MakeAllFolder();//创建所有的文件夹
        //head数据部分
        unique_ptr<HeaderSeriesOut> hso(new HeaderSeriesOut(readFile->FileRoute,
                                        readFile->FilePath+"HeaderSeries\\"));
        readFile->allPrint.insert(readFile->allPrint.end(),//Head系列的信息数据交由主存储
            hso->headSeriesDLP.begin(),hso->headSeriesDLP.end());

        hso->HeadAllDataOut();
        hso.reset();

        readFile.reset();
    }catch(string &es){
        cerr<<es;
        cin.get();
        exit(1);
    }
    return 0;
}

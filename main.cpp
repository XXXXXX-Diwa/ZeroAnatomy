#include "main.h"
#include "HeaderSeriesOut.h"
#include "TileSetSeriesOut.h"

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
}
File::~File(){

}

void File::MakeMainAsmFile(){
    string tes=FilePath+"ZeroAnatomy.asm";
    ofstream mf(tes,ios::out);//�������ļ�
    if(mf.fail()){
        DataException::FileError(tes,0);
    }
    //�������(��ʱ��)
    mf<<".gba"<<endl;
    mf<<".open \"zm.gba\",\"ts.gba\",0x8000000"<<endl;


    mf<<".definelabel ZeroAnatomyFreeDataOffset,0x8770000\n"<<endl;
    //ռ�ò���
    mf<<".include \"HeaderSeries\\HeadGerenalData.asm\""<<endl;
    mf<<".include \"TileSetSeries\\TileSetGerenalData.asm\""<<endl;
    mf<<endl;
    //�Զ��岿��
    mf<<".org ZeroAnatomyFreeDataOffset"<<endl;
    mf<<".include \"HeaderSeries\\HeadCustomData.asm\""<<endl;
    mf<<".include \"TileSetSeries\\TileSetCustomData.asm\""<<endl;


    //�������(��ʱβ)
    mf<<".close"<<endl;


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
    File::makefolder(readFile->FilePath+"HeaderSeries");//����Header�����ļ���
    readFile->MakeMainAsmFile();//����ʹ�ӡ���ļ�
    //head���ݲ���
    unique_ptr<HeaderSeriesOut> hso(new HeaderSeriesOut(readFile->FileRoute,
                                    readFile->FilePath+"HeaderSeries\\"));
    hso->MakeHeaderSeriesFolders();//����Header����������ļ���
    hso->HeadAllDataOut();     //Header���ݵ���
    readFile->allPrint.insert(readFile->allPrint.end(),//Headϵ�е���Ϣ���ݽ������洢
        hso->headSeriesDLP.begin(),hso->headSeriesDLP.end());
    hso.reset();//headerϵ�������ͷ�
    //Tileset���ݲ���
    File::makefolder(readFile->FilePath+"TileSetSeries");//����TileSet�����ļ���
    unique_ptr<TileSetSeriesOut> tsso(new TileSetSeriesOut(readFile->FileRoute,
                                        readFile->FilePath+"TileSetSeries\\"));
    tsso->MakeTileSetSeriesFolders();//����Tileset����������ļ���
    tsso->TileSetAllDataOut();      //TileSet���ݵ���
    readFile->allPrint.insert(readFile->allPrint.end(),
        tsso->tileSetSeriesDLP.begin(),tsso->tileSetSeriesDLP.end());
    tsso.reset();

    readFile.reset();

    return 0;
}

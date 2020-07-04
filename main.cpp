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
    ofstream mf(tes,ios::out);//创建主文件
    if(mf.fail()){
        DataException::FileError(tes,0);
    }
    //方便测试(临时首)
    mf<<".gba"<<endl;
    mf<<".open \"zm.gba\",\"ts.gba\",0x8000000"<<endl;


    mf<<".definelabel ZeroAnatomyFreeDataOffset,0x8770000\n"<<endl;
    //占用部分
    mf<<".include \"HeaderSeries\\HeadGerenalData.asm\""<<endl;
    mf<<".include \"TileSetSeries\\TileSetGerenalData.asm\""<<endl;
    mf<<endl;
    //自定义部分
    mf<<".org ZeroAnatomyFreeDataOffset"<<endl;
    mf<<".include \"HeaderSeries\\HeadCustomData.asm\""<<endl;
    mf<<".include \"TileSetSeries\\TileSetCustomData.asm\""<<endl;


    //方便测试(临时尾)
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
    File::makefolder(readFile->FilePath+"HeaderSeries");//创建Header数据文件夹
    readFile->MakeMainAsmFile();//制造和打印主文件
    //head数据部分
    unique_ptr<HeaderSeriesOut> hso(new HeaderSeriesOut(readFile->FileRoute,
                                    readFile->FilePath+"HeaderSeries\\"));
    hso->MakeHeaderSeriesFolders();//创建Header数据所需的文件夹
    hso->HeadAllDataOut();     //Header数据导出
    readFile->allPrint.insert(readFile->allPrint.end(),//Head系列的信息数据交由主存储
        hso->headSeriesDLP.begin(),hso->headSeriesDLP.end());
    hso.reset();//header系列数据释放
    //Tileset数据部分
    File::makefolder(readFile->FilePath+"TileSetSeries");//创建TileSet数据文件夹
    unique_ptr<TileSetSeriesOut> tsso(new TileSetSeriesOut(readFile->FileRoute,
                                        readFile->FilePath+"TileSetSeries\\"));
    tsso->MakeTileSetSeriesFolders();//创建Tileset数据所需的文件夹
    tsso->TileSetAllDataOut();      //TileSet数据导出
    readFile->allPrint.insert(readFile->allPrint.end(),
        tsso->tileSetSeriesDLP.begin(),tsso->tileSetSeriesDLP.end());
    tsso.reset();

    readFile.reset();

    return 0;
}

#include "main.h"
#include "HeaderSeriesOut.h"
#include "TileSetSeriesOut.h"
#include "AnimatedOut.h"
#include "ProjInfo.h"
#include "SpriteSeries.h"
#include "ConnectionsSeries.h"
#include "Scrolls.h"
#include "MiniMap.h"
#include "Sounds.h"
#include "Demo.h"
#include "TextSeries.h"
#include "EndingImage.h"

bool MyCompare(const DataListPrint &D1,const DataListPrint &D2){
    return D1.offset<D2.offset;
}

bool MyIque(const DataListPrint &D1,const DataListPrint &D2){
    return D1.offset==D2.offset;
}

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

void File::OpenFile(ifstream &in,string &s,bool bin){
    if(bin){
        in.open(s,ios::in|ios::binary);
    }else{
        in.open(s,ios::in);
    }
    if(in.fail()){
        DataException::FileError(s,1);
    }

}
void File::MakeFile(ofstream &out,string &s,bool bin){
    if(bin){
        out.open(s,ios::out|ios::binary);
    }else{
        out.open(s,ios::out);
    }
    if(out.fail()){
        DataException::FileError(s,0);
    }
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
    mf<<".include \"AnimatedSeries\\AnimatedGerenalData.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpriteGeneralData.asm\""<<endl;
    mf<<".include \"ConnectionsSeries\\ConnectionGeneralData.asm\""<<endl;
    mf<<".include \"ScrollSeries\\ScrollGeneralData.asm\""<<endl;
    mf<<".include \"MiniMapSeries\\MiniMapGeneralData.asm\""<<endl;
    mf<<".include \"SoundSeries\\TrackGerenalData.asm\""<<endl;
    mf<<".include \"DemoSeries\\DemoGeneralData.asm\""<<endl;
    mf<<".include \"TextSeries\\TextGeneralData.asm\""<<endl;
    mf<<".include \"EndingImageSeries\\EndingImageGeneral.asm\""<<endl;
    mf<<endl;
    //�Զ��岿��
    mf<<".org ZeroAnatomyFreeDataOffset"<<endl;

    mf<<".include \"HeaderSeries\\HeadPointerDef.asm\""<<endl;
    mf<<".include \"HeaderSeries\\HeadCustomData.asm\""<<endl;

    mf<<".include \"TileSetSeries\\TileSetPointerDef.asm\""<<endl;
    mf<<".include \"TileSetSeries\\TileSetCustomData.asm\""<<endl;

    mf<<".include \"AnimatedSeries\\AnimatedGpcPointerDef.asm\""<<endl;
    mf<<".include \"AnimatedSeries\\AnimatedGpcDataTable.asm\""<<endl;
    mf<<".include \"AnimatedSeries\\AnimatedPalPointerDef.asm\""<<endl;
    mf<<".include \"AnimatedSeries\\AnimatedPalDataTable.asm\""<<endl;

    mf<<".include \"SpriteSeries\\SpriteAITableData.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpriteGpcPointerDef.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpriteGpcDataTable.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpritePalPointerDef.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpritePalDataTable.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpriteSetPointerDef.asm\""<<endl;
    mf<<".include \"SpriteSeries\\SpriteSetDataTable.asm\""<<endl;

    mf<<".include \"ConnectionsSeries\\DoorBinDataDef.asm\""<<endl;
    mf<<".include \"ConnectionsSeries\\AreaHatchLockDataDef.asm\""<<endl;
    mf<<".include \"ConnectionsSeries\\AreaHatchLockPointerTable.asm\""<<endl;
    mf<<".include \"ConnectionsSeries\\AreaDoorDataDef.asm\""<<endl;
    mf<<".include \"ConnectionsSeries\\AreaDoorPointerTable.asm\""<<endl;

    mf<<".include \"ScrollSeries\\ScrollDataDef.asm\""<<endl;
    mf<<".include \"ScrollSeries\\ScrollPointerDef.asm\""<<endl;
    mf<<".include \"ScrollSeries\\AreaScrollPointerTable.asm\""<<endl;

    mf<<".include \"MiniMapSeries\\MiniMapDataDef.asm\""<<endl;
    mf<<".include \"MiniMapSeries\\MiniMapDataPointerTable.asm\""<<endl;

    mf<<".include \"SoundSeries\\DamageData.asm\""<<endl;
    mf<<".include \"SoundSeries\\SamplesDef.asm\""<<endl;
    mf<<".include \"SoundSeries\\TrackData.asm\""<<endl;
    mf<<".include \"SoundSeries\\TrackHeaderData.asm\""<<endl;
    mf<<".include \"SoundSeries\\TrackTableData.asm\""<<endl;
    mf<<".include \"SoundSeries\\VoiceGroupData.asm\""<<endl;
    mf<<".include \"SoundSeries\\WaveMemoryData.asm\""<<endl;

    mf<<".include \"DemoSeries\\DemoDataDef.asm\""<<endl;
    mf<<".include \"DemoSeries\\DemoStateOftTable.asm\""<<endl;
    mf<<".include \"DemoSeries\\DemoKeysOftTable.asm\""<<endl;

    mf<<".include \"TextSeries\\TextDataDef.asm\""<<endl;
    mf<<".include \"TextSeries\\TextDataTable.asm\""<<endl;
    mf<<".include \"TextSeries\\TextLanguageTable.asm\""<<endl;

    mf<<".include \"EndingImageSeries\\EndingImageDef.asm\""<<endl;
    mf<<".include \"EndingImageSeries\\EndingImageTable.asm\""<<endl;

    //�������(��ʱβ)
    mf<<".close"<<endl;


    mf.close();
}

void File::PrintList(){
    string tes=FilePath+"DataList.asm";
    sort(allPrint.begin(),allPrint.end(),MyCompare);
    allPrint.erase(unique(allPrint.begin(),allPrint.end(),MyIque),allPrint.end());
    ofstream ouf(tes,ios::out);
    if(ouf.fail()){
        DataException::FileError(tes,0);
    }
    vector<DataListPrint>::iterator it;
    for(it=allPrint.begin();it!=allPrint.end();++it){
        ouf<<hex<<setiosflags(ios::uppercase)
        <<setfill('0')<<"0x"<<setw(7)<<(int)it->offset
        <<" - 0x"<<setw(7)<<(int)(it->offset+it->len)
        <<"\t"<<it->explain<<endl;

    }
    ouf.close();
}
int main(int argc,char* const argv[]){
    ios::sync_with_stdio(false);
    //�ļ���Ϣ����
    unique_ptr<File>readFile;
    if(argc==1){
        DataException::FileError("",2);
    }else{
        readFile=make_unique<File>(argv[1]);
    }
    //����ʱ�䲿��
    RunTimes *durt=RunTimes::SharedRunTime();

    //Proj���ݲ���
    unique_ptr<ProjInfo>proj(new ProjInfo(readFile->FileRoute));

    File::makefolder(readFile->FilePath+"HeaderSeries");//����Header�����ļ���
    readFile->MakeMainAsmFile();//����ʹ�ӡ���ļ�
    //head���ݲ���
    unique_ptr<HeaderSeriesOut> hso(new HeaderSeriesOut(readFile->FileRoute,
                        readFile->FilePath+"HeaderSeries\\",proj->roomsPerArea));
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
    //Animated���ݲ���
    File::makefolder(readFile->FilePath+"AnimatedSeries");//����Animated�����ļ���
    unique_ptr<AnimatedOut> ao(new AnimatedOut(readFile->FileRoute,
                    readFile->FilePath+"AnimatedSeries\\",proj->numOfAnimaTilesets,
                        proj->numOfAnimaGfx,proj->numOfAnimaPalettes));
    ao->MakeAnimatedOutFolders();
    ao->AnimatedAllDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
        ao->animatedDLP.begin(),ao->animatedDLP.end());
    ao.reset();
    //Sprite���ݲ���
    File::makefolder(readFile->FilePath+"SpriteSeries");//����Sprite�����ļ���
    unique_ptr<SpriteSeries> ss(new SpriteSeries(readFile->FileRoute,
                            readFile->FilePath+"SpriteSeries\\",proj->numOfSpritesets));
    ss->MakeSpriteSeriesFolders();
    ss->SpriteSeriesDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
        ss->spriteDLP.begin(),ss->spriteDLP.end());
    ss.reset();
    //Connections���ݲ���
    File::makefolder(readFile->FilePath+"ConnectionsSeries");
    unique_ptr<ConnectionsSeries> cs(new ConnectionsSeries(readFile->FileRoute,
                            readFile->FilePath+"ConnectionsSeries\\"));
    cs->MakeConnectionsSeriesFolder();
    cs->ConnectionsSeriesDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
        cs->connectionsDLP.begin(),cs->connectionsDLP.end());
    cs.reset();
    //Scroll���ݲ���
    File::makefolder(readFile->FilePath+"ScrollSeries");
    unique_ptr<Scrolls> so(new Scrolls(readFile->FileRoute,
            readFile->FilePath+"ScrollSeries\\",proj->roomsPerArea));
    so->MakeScrollsFolder();
    so->ScrollsDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
        so->scrollsDLP.begin(),so->scrollsDLP.end());
    so.reset();
    //MiniMap���ݲ���
    File::makefolder(readFile->FilePath+"MiniMapSeries");
    unique_ptr<MiniMap> mm(new MiniMap(readFile->FileRoute,
                        readFile->FilePath+"MiniMapSeries\\"));
    mm->MakeMiniMapSeriesFolder();
    mm->MapDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
                    mm->mdlp.begin(),mm->mdlp.end());
    mm.reset();

    durt->RunFinish("Sounds���ݴ���֮ǰ");

    //Sounds���ݲ���
    File::makefolder(readFile->FilePath+"SoundSeries");
    unique_ptr<Sounds> Sod(new Sounds(readFile->FileRoute,
                        readFile->FilePath+"SoundSeries\\"));
    Sod->MakeSoundSeriesFolder();
    Sod->SoundSeriesDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
                    Sod->songdlp.begin(),Sod->songdlp.end());
    Sod.reset();

    //Demo���ݲ���
    File::makefolder(readFile->FilePath+"DemoSeries");
    unique_ptr<Demo> demo(new Demo(readFile->FileRoute,
                        readFile->FilePath+"DemoSeries\\"));
    demo->MakeDemoSeriesFolder();
    demo->DemoDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
                    demo->demodlp.begin(),demo->demodlp.end());
    demo.reset();
    //Text���ݲ���
    File::makefolder(readFile->FilePath+"TextSeries");
    unique_ptr<Texts> text(new Texts(readFile->FileRoute,
                        readFile->FilePath+"TextSeries\\"));
    text->MakeTextSeriesFolder();
    text->TextDataOut();
    readFile->allPrint.insert(readFile->allPrint.end(),
                    text->textdlp.begin(),text->textdlp.end());
    text.reset();
    //EndingImage���ݲ���
    File::makefolder(readFile->FilePath+"EndingImageSeries");
    unique_ptr<EndingImage> emg(new EndingImage(readFile->FileRoute,
                        readFile->FilePath+"EndingImageSeries\\"));
    emg->MakeEndingImageFolder();
    emg->EndingImageOut();
    emg.reset();

    proj.reset();
    readFile->PrintList();
    readFile.reset();
    return 0;
}

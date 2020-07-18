#ifndef TEXTSERIES_H_INCLUDED
#define TEXTSERIES_H_INCLUDED
#include"main.h"

/**
*图片的地址在0x415460-0x44145F,总共0x2c000(未压缩字节),但只有前0x8000是U版本用到的.
*调色板在0x3FD030,暂时认为只有0x20字节 没找到ldr处
*但加0x20则找到有ldr 在0x6A69C处ldr
*字符宽度在0x40D7B0-40DC4F,长度0x4A0 6E478处ldr

*text_story部分 首文字数据地址在0x445CE4(英文) 文字结束以0xFF00
*该地址指针在0x7609F4 共6个
*该指针的指针在0x75FEE8(连续5个都是)
*应该是不同版本语言的分类,但是日文和片假名似乎不在其中之列
*片假名的首文字数据在0x44473C 指针的指针应该减8
*因为前两个恰恰是日文和片假名的数据指针
*0x62D64 和 0x6F550处ldr

*text_description部分 首文字数据地址在0x4453A4
*该地址指针在0x7609A8 共0x13个
*该指针的指针在0x7602DC(连续5个都是) 减8
*0x6F7D8处ldr

*text_locations部分 首文字数据地址在0x442E48
*该地址指针在0x7606FC 共0x1A个
*该指针的指针在0x76076C(连续5个都是) 减8
*0x6F08C处ldr

*text_messages部分 首文字数据地址在0x442560
*该地址指针在0x760660 共0x27个
*该指针的指针在0x760788(连续5个都是) 减8
*0x6F1EC 0x6F398 0x6FB54 0x6FC50 0x6FCF4处ldr

*text_screen部分 首文字数据地址在0x446250
*该地址指针在0x760A0C 共0x25个
*该指针的指针在0x7607A4(连续5个都是) 减8
*0x6F634 0x0x79034处ldr
**/

//数目暂未解析
class Texts{
private:
    string readRom;
    string curPath;
    uint32_t ldrTextGraphicOft[5]={
        0x6E554,0x6E9EC,0x7B130,0x7B924,0x7B9E0};
    uint32_t textGraphicOft;

	uint32_t ldrCharacterWidthOft=0x6E478;
	uint32_t characterWidthOft;

    uint32_t ldrTextStoryLanguageTableOft[2]={
        0x62D64,0x6F550};
    uint32_t textStoryLanguageTable;
    vector<vector<uint32_t> >textStoryDataOfts;

    uint32_t ldrTextDescriptionLanguageTableOft=0x6F7D8;
    uint32_t textDescriptionLanguageTable;
    vector<vector<uint32_t> >textDescriptionDataOfts;

    uint32_t ldrTextLocationsLanguageTableOft=0x6F08C;
    uint32_t textLocationsLanguageTable;
    vector<vector<uint32_t> >textLocationsDataOfts;

    uint32_t ldrTextMessagesLanguageTableOft[5]={
        0x6F1EC,0x6F398,0x6FB54,0x6FC50,0x6FCF4};
    uint32_t textMessagesLanguageTable;
    vector<vector<uint32_t> > textMessagesDataOfts;

    uint32_t ldrTextScreenLanguageTableOft[2]={
        0x6F634,0x79034};
    uint32_t textScreenLanguageTable;
    vector<vector<uint32_t> >textScreenDataOfts;
    uint8_t buffer[0x2C000];
public:
    vector<DataListPrint>textdlp;
    Texts(string rom,string path);
    ~Texts();
    void MakeTextSeriesFolder();
    void TextDataOut();
};

#endif // TEXTSERIES_H_INCLUDED

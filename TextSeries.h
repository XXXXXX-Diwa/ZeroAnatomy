#ifndef TEXTSERIES_H_INCLUDED
#define TEXTSERIES_H_INCLUDED
#include"main.h"

/**
*ͼƬ�ĵ�ַ��0x415460-0x44145F,�ܹ�0x2c000(δѹ���ֽ�),��ֻ��ǰ0x8000��U�汾�õ���.
*��ɫ����0x3FD030,��ʱ��Ϊֻ��0x20�ֽ� û�ҵ�ldr��
*����0x20���ҵ���ldr ��0x6A69C��ldr
*�ַ������0x40D7B0-40DC4F,����0x4A0 6E478��ldr

*text_story���� ���������ݵ�ַ��0x445CE4(Ӣ��) ���ֽ�����0xFF00
*�õ�ַָ����0x7609F4 ��6��
*��ָ���ָ����0x75FEE8(����5������)
*Ӧ���ǲ�ͬ�汾���Եķ���,�������ĺ�Ƭ�����ƺ���������֮��
*Ƭ������������������0x44473C ָ���ָ��Ӧ�ü�8
*��Ϊǰ����ǡǡ�����ĺ�Ƭ����������ָ��
*0x62D64 �� 0x6F550��ldr

*text_description���� ���������ݵ�ַ��0x4453A4
*�õ�ַָ����0x7609A8 ��0x13��
*��ָ���ָ����0x7602DC(����5������) ��8
*0x6F7D8��ldr

*text_locations���� ���������ݵ�ַ��0x442E48
*�õ�ַָ����0x7606FC ��0x1A��
*��ָ���ָ����0x76076C(����5������) ��8
*0x6F08C��ldr

*text_messages���� ���������ݵ�ַ��0x442560
*�õ�ַָ����0x760660 ��0x27��
*��ָ���ָ����0x760788(����5������) ��8
*0x6F1EC 0x6F398 0x6FB54 0x6FC50 0x6FCF4��ldr

*text_screen���� ���������ݵ�ַ��0x446250
*�õ�ַָ����0x760A0C ��0x25��
*��ָ���ָ����0x7607A4(����5������) ��8
*0x6F634 0x0x79034��ldr
**/

//��Ŀ��δ����
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

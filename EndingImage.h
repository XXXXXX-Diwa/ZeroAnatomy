#ifndef ENDINGIMAGE_H_INCLUDED
#define ENDINGIMAGE_H_INCLUDED
#include "main.h"
/**Fusionͼ���ݵ�ַ����Ϊ:
*ÿ��ͼ��5����ַ,1Ϊͼ�ϴ�벿,2Ϊͼ��С�벿
*3Ϊ����ͼ����,4Ϊ����ͼ����,5Ϊ��ɫ��(0x200)?
**/

/**Zeroͼ���ݵ�ַ����Ϊ:
*54c06c���д����ϴ�벿ͼ����ָ��
*54c08c���д�����С�벿ͼ����ָ��
*54c0ac���д�������ϴ�벿����ͼ����ָ��
*54c0cc���д��������С�벿����ͼ����ָ��
*54c0ec���д��˵�ɫ��(ÿ��0x200)
**/
class EndingImage{
private:
    string readRom;
    string curPath;
    uint32_t ldrFusionEndingImageTableOft=0x849F4;
    uint32_t ldrZeroEndingImageTableOft[2]={0x868F4,0x874AC};
    uint32_t FusionEndingImageOftTable;
    uint32_t ZeroEndingImageOftTable;
    vector<uint32_t>fusionPointer;
    vector<uint32_t>zeroPointer;
    uint8_t buffer[0x200];
public:
    vector<DataListPrint>eidlp;
    EndingImage(string,string);
    ~EndingImage();
    void MakeEndingImageFolder();
    void EndingImageOut();
};


#endif // ENDINGIMAGE_H_INCLUDED

#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED
#include "main.h"
/**363ce8��¼��demo�ݳ���˳��� 60c0c 60cec��ldr
*411510�ǰ�����Լ������ 75884��ldr �����ǵ���
*75fdfc��¼��demo��״̬���ݵ�ַ,ÿ����0x280�ֽ� 753c8��ldr
*363be8��¼��demo�İ������ݵ�ַ,�Ե�ַ+������ʽ
*,����Ϊ���ֽ�,���Ǿ�������,�ƺ��޷�����FD��
*ÿ��demo��������,60c14��ldr
*��һ��Ϊ����������,ÿ��4�ֽ�,�ڶ���Ϊ�����ĳ���ʱ��,ÿ�����ֽ�
*demo���¼��޸���60c2a
**/

class Demo{
private:
    struct Keys{
        uint32_t keyOft;
        uint32_t keyLen;
        uint32_t keySusOft;
        uint32_t susLen;
    };
    string readRom;
    string curPath;
    uint32_t ldrDemoOrder[2]={0x60C0C,0x60CEC};
    uint32_t ldrDemoState=0x753C8;
    uint32_t ldrDemoKeys=0x60C14;
    uint8_t buffer[0x440];
    uint32_t orderOft;
    uint32_t stateOftTableOft;
    uint32_t keysOftTableOft;
    vector<uint32_t>stateOft;
    vector<Keys>keysOft;
public:
    vector<DataListPrint>demodlp;
    Demo(string rom,string path);
    ~Demo();
    void MakeDemoSeriesFolder();
    void DemoDataOut();
};


#endif // DEMO_H_INCLUDED

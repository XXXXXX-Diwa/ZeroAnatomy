#ifndef CONNECTIONSSERIES_H_INCLUDED
#define CONNECTIONSSERIES_H_INCLUDED
/**
*��label��ʾ
*12534����ת ��11ED8-11F07 ��ת���ݹ̶�ΪFF F7 D0 FC 2E E0

*�Ķ���11F04��ldr û�иĶ��򲻵��� ����3�ֽ�Ϊһ��,����Ϊ3��FF

*�¼���
*������Ķ���5efb0���ֽ�Ϊ06
*5efb0-5Eff7��Χ�Ķ�
*5Efec��ldr ���۸Ķ���� 5efe4Ϊ�޸Ĺ����¼��������

*����
*����Ķ����¼����غ� 5EEB8��ldr ���۸Ķ���� ���ݽ�������ff

*����  Table 75Fde0
*�ṹ
*0 ����
*1 �¼�
*2 before flag
*3 1 2 4 8 10 20 40 80 ����8����
*4-7 ����

*3602d8�������ÿ������������,16bit��� ldr 5f708
**/
#include "main.h"
class ConnectionsSeries{
private:
    string readRomRoute;
    string connectionsPath;
    uint32_t ldrAreaDoorPointerTableOft[7]=
        {0x569B0,0x5ECAC,0x5ED6C,0x5EE30,0x5EF38,0x5F080,0x5F3C0};
    uint32_t ldrAreaHatchLockTableOft=0x5F70C;
    uint32_t ldrNumberOfAreaHatchLock=0x5F708;
    uint8_t defaultNumberOfEventDoor=0x29;//Ĭ���¼��ŵ�����
    bool doorLabelModified;//��(����)�޸�flag
	uint32_t numberOfmodifiedEventDoorOft=0x5EFE4;//�޸ĺ���¼���������¼�� ldr
	uint32_t ldrModifiedLabelDoorDataOft=0x11F04;
	uint32_t ldrEventDoorDataOft=0x5EFEC;
	uint32_t modifiedDoorFunctionDataOft=0x5EFB0;//72�ֽ�(71)
	uint32_t doorLabelBLmodifiedOft=0x12534;//6�ֽ�
	uint32_t ldrDoorAreaTransferDataOft=0x5EEB8;
	vector<uint16_t> numberOfAreaHatchLock;//ÿ�������������������
    vector<uint32_t> areaHatchLockPointer;//ÿ������������ָ��
    vector<uint32_t> areaDoorPointers;//ÿ�������������ָ��
    uint8_t buffer[0xC00];
public:
    vector<DataListPrint>connectionsDLP;
    ConnectionsSeries(string name,string path);
    ~ConnectionsSeries();
    void ConnectionsSeriesDataOut();
    void MakeConnectionsSeriesFolder();
    uint32_t CallPointerCalculate(ifstream &inf,uint32_t blOffset);
};


#endif // CONNECTIONSSERIES_H_INCLUDED

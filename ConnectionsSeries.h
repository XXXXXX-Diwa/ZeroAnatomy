#ifndef CONNECTIONSSERIES_H_INCLUDED
#define CONNECTIONSSERIES_H_INCLUDED
/**
*门label显示
*12534处跳转 到11ED8-11F07 跳转数据固定为FF F7 D0 FC 2E E0

*改动在11F04处ldr 没有改动则不导出 数据3字节为一个,结束为3个FF

*事件门
*若程序改动则5efb0处字节为06
*5efb0-5Eff7范围改动
*5Efec处ldr 无论改动与否 5efe4为修改过后记录的总数量

*区门
*程序改动和事件门重合 5EEB8处ldr 无论改动与否 数据结束三个ff

*门锁  Table 75Fde0
*结构
*0 房间
*1 事件
*2 before flag
*3 1 2 4 8 10 20 40 80 控制8个门
*4-7 无用

*3602d8处标记了每个区门锁数量,16bit间距 ldr 5f708
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
    uint8_t defaultNumberOfEventDoor=0x29;//默认事件门的数量
    bool doorLabelModified;//门(程序)修改flag
	uint32_t numberOfmodifiedEventDoorOft=0x5EFE4;//修改后的事件门数量记录处 ldr
	uint32_t ldrModifiedLabelDoorDataOft=0x11F04;
	uint32_t ldrEventDoorDataOft=0x5EFEC;
	uint32_t modifiedDoorFunctionDataOft=0x5EFB0;//72字节(71)
	uint32_t doorLabelBLmodifiedOft=0x12534;//6字节
	uint32_t ldrDoorAreaTransferDataOft=0x5EEB8;
	vector<uint16_t> numberOfAreaHatchLock;//每个区域门锁房间的数量
    vector<uint32_t> areaHatchLockPointer;//每个区门锁数据指针
    vector<uint32_t> areaDoorPointers;//每个区域的门数据指针
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

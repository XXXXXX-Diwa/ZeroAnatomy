#ifndef ENDINGIMAGE_H_INCLUDED
#define ENDINGIMAGE_H_INCLUDED
#include "main.h"
/**Fusion图数据地址分析为:
*每个图有5个地址,1为图上大半部,2为图下小半部
*3为不明图数据,4为不明图数据,5为调色板(0x200)?
**/

/**Zero图数据地址分析为:
*54c06c集中存了上大半部图数据指针
*54c08c集中存了下小半部图数据指针
*54c0ac集中存了配合上大半部不明图数据指针
*54c0cc集中存了配合下小半部不明图数据指针
*54c0ec击中存了调色板(每个0x200)
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

#ifndef TEXTSERIES_H_INCLUDED
#define TEXTSERIES_H_INCLUDED
#include"main.h"

/**
*图片的地址在0x415460-0x44145F,总共0x2c000(未压缩字节),但只有前0x8000是U版本用到的.
*调色板在0x3FD030,暂时认为只有0x20字节
*字符宽度在0x407B0-40DC4F,长度0x4A0

*text_story部分 首文字数据地址在0x445CE4
*该地址指针在0x7609F4 共6个
*该指针的指针在0x75FEE8(连续5个都是)

*text_description部分 首文字数据地址在0x4453A4
*该地址指针在0x7609A8 共0x13个
*该指针的指针在0x7602DC(连续5个都是)

*text_locations部分 首文字数据地址在0x442E48
*该地址指针在0x7606FC 共0x1A个
*该指针的指针在0x76076C(连续5个都是)

*text_messages部分 首文字数据地址在0x442560
*该地址指针在0x760660 共0x27个
*该指针的指针在0x760788(连续5个都是)

*text_screen部分 首文字数据地址在0x446250
*该地址指针在0x760A0C 共0x25个
*该指针的指针在0x7607A4(连续5个都是)
**/

//数目暂未解析-要拆解每种的具体函数
class Texts{
private:
    string readRom;
    string curPath;
public:
    Texts();
    ~Texts();

};

#endif // TEXTSERIES_H_INCLUDED

#ifndef TEXTSERIES_H_INCLUDED
#define TEXTSERIES_H_INCLUDED
#include"main.h"

/**
*ͼƬ�ĵ�ַ��0x415460-0x44145F,�ܹ�0x2c000(δѹ���ֽ�),��ֻ��ǰ0x8000��U�汾�õ���.
*��ɫ����0x3FD030,��ʱ��Ϊֻ��0x20�ֽ�
*�ַ������0x407B0-40DC4F,����0x4A0

*text_story���� ���������ݵ�ַ��0x445CE4
*�õ�ַָ����0x7609F4 ��6��
*��ָ���ָ����0x75FEE8(����5������)

*text_description���� ���������ݵ�ַ��0x4453A4
*�õ�ַָ����0x7609A8 ��0x13��
*��ָ���ָ����0x7602DC(����5������)

*text_locations���� ���������ݵ�ַ��0x442E48
*�õ�ַָ����0x7606FC ��0x1A��
*��ָ���ָ����0x76076C(����5������)

*text_messages���� ���������ݵ�ַ��0x442560
*�õ�ַָ����0x760660 ��0x27��
*��ָ���ָ����0x760788(����5������)

*text_screen���� ���������ݵ�ַ��0x446250
*�õ�ַָ����0x760A0C ��0x25��
*��ָ���ָ����0x7607A4(����5������)
**/

//��Ŀ��δ����-Ҫ���ÿ�ֵľ��庯��
class Texts{
private:
    string readRom;
    string curPath;
public:
    Texts();
    ~Texts();

};

#endif // TEXTSERIES_H_INCLUDED

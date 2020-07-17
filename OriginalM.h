#ifndef ORIGINALM_H_INCLUDED
#define ORIGINALM_H_INCLUDED

#include "main.h"
class OriginalM{//由于要检查其它数据是否会覆盖此处的情况,所以放在最后
private:
    string readRom;
    string curPath;
    const uint32_t originalOft=0x7D8000;//无法更改,更改会失效
    const uint32_t originalLen=0x1F734;
public:

};

#endif // ORIGINALM_H_INCLUDED

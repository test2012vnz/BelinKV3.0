#include "MFM.h"

int MFM::id_count = 1;

float MFM::__swap_float(float i)
{
    uint32_t c = *(uint32_t *)&i;
    uint16_t ch = c >> 16;
    ch = __bswap_16(ch);
    uint16_t cl = c & 0xFFFFFFFF;
    cl = __bswap_16(cl);
    c = (ch << 16) | cl;
    return *(float *)&c;
}

bool MFM::load()
{
    if (readIreg(0, (uint16_t *)&d, 64) == 0)
    {
        status = true;
        getData();
        getString();
        return status;
    }
    status = false;
    return status;
}

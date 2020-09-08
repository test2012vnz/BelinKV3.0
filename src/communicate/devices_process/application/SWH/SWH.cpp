#include "SWH.h"

int SWH::id_count;
TaskHandle_t SWH::Task_t = NULL;

String SWH::getString()
{
    String data = "[A0=" + String( __swap_float(*((uint32_t *)&d + 0))); //A0

    for (uint8_t i = 1; i < 250; i++)
    {
        String key = String("&A") + String(i) + String("=");
        data += FloatToString(key, __swap_float(*((uint32_t *)&d + i)));
    }
    data += "]OK=1";

    String s = "";
#if API_VERSION == 0
    s += APIHeader(1);
    s += data;

#elif API_VERSION == 1
    s += APIHeader(1);
    s += data;
#endif

    return s;
}
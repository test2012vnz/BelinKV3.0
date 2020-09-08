#include "Hybrid.h"

int Hybrid::id_count = 1;

bool Hybrid::load()
{
    if (readIreg(0x200, (uint16_t *)&d.Operation_Status, 0x55) == 0)
    {
        is_Real_Time_Data_OK = true;
    }
    else
    {
        is_Real_Time_Data_OK = false;
    }
    if (readIreg(0x10B0, (uint16_t *)&d.Battery_Type, 13) == 0)
    {
        is_Battery_Read_OK = true;
    }
    else
    {
        is_Battery_Read_OK = false;
    }
    if (readIreg(0x2000, (uint16_t *)&d.Product_Code_Name, 0x10) == 0)
    {
        is_Product_Information_OK = true;
    }
    else
    {
        is_Product_Information_OK = false;
    }
    return status;
}
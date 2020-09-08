#include "Sofar.h"

int Sofar::id_count = 1;

bool Sofar::load()
{
    d = {};
    if (readHreg(0, (uint16_t *)&d.Operating_state, 40) == 0)
    {
        status = true;
        is_Real_Time_Data_OK = true;
    }
    else
    {
        status = false;
        is_Real_Time_Data_OK = false;
    }

    if (readIreg(256, (uint16_t *)&d.Alarm_message_table1, 21) == 0)
    {
        status = true;
        is_Built_in_combiner_data_address_table_OK = true;
    }
    else
    {
        status = false;
        is_Built_in_combiner_data_address_table_OK = false;
    }

    if (readIreg(0x2000, (uint16_t *)&d.Product_code, 12) == 0)
    {
        status = true;
        is_Product_Information_OK = true;
        //log_d("read devsn ok---");
    }
    else
    {
        is_Product_Information_OK = false;
        status = false;
    }

    return is_Real_Time_Data_OK;
}
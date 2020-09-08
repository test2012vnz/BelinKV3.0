#include "HuaweiV2.h"

int HuaweiV2::id_count = 1;

bool HuaweiV2::load()
{
    status = false;
    alarm_read_ok = false;
    d = {};
    if (readHreg(50000, (uint16_t *)&d.Alarm_1, 17) == 0)
    {
        alarm_read_ok = true;
    }
    if (readHreg(32262, (uint16_t *)&d.PV1_Volt, 67) == 0)
    {
        status = true;
    }
    if (readHreg(33022, (uint16_t *)&d.MPPT1_Total_input_power, 6) == 0)
    {
    }
    if (readHreg(33070, (uint16_t *)&d.MPPT4_Total_input_power, 2) == 0)
    {
    }
    if (readHreg(32001, (uint16_t *)&d.Model_ID, 2) == 0)
    {
    }
    return status;
}

String HuaweiV2::getString()
{
    String data = "";
    data += ",A0:" + String(to_u16(d.Inverter_status));
    data += ",A1:" + String(to_u16(d.Ouput_Mode));
    data += ",A2:" + String(to_u16(d.Alarm_1));
    data += ",A3:" + String(to_u16(d.Alarm_2));
    data += ",A4:" + String(to_u16(d.Alarm_3));
    data += ",A5:" + String(to_u16(d.Alarm_4));
    data += ",A6:" + String(to_u16(d.Alarm_5));
    data += ",A7:" + String(to_u16(d.Alarm_6));
    data += ",A8:" + String(to_u16(d.Alarm_7));
    data += ",A9:" + String(to_u16(d.Alarm_8));
    data += ",A10:" + String(to_u16(d.Alarm_9));
    data += ",A11:" + String(to_u16(d.Alarm_10));
    data += ",A12:" + String(to_u16(d.Alarm_17));
    data += ",A13:" + String(to_u16(d.Model_ID));
    data += ",A14:" + String(to_float(d.PV1_Volt, 10));
    data += ",A15:" + String(to_float(d.PV1_Curr, 10));
    data += ",A16:" + String(to_float(d.PV2_Volt, 10));
    data += ",A17:" + String(to_float(d.PV2_Curr, 10));
    data += ",A18:" + String(to_float(d.PV3_Volt, 10));
    data += ",A19:" + String(to_float(d.PV3_Curr, 10));
    data += ",A20:" + String(to_float(d.PV4_Volt, 10));
    data += ",A21:" + String(to_float(d.PV4_Curr, 10));
    data += ",A22:" + String(to_float(d.PV5_Volt, 10));
    data += ",A23:" + String(to_float(d.PV5_Curr, 10));
    data += ",A24:" + String(to_float(d.PV6_Volt, 10));
    data += ",A25:" + String(to_float(d.PV6_Curr, 10));
    data += ",A26:" + String(to_float(d.PV7_Volt, 10));
    data += ",A27:" + String(to_float(d.PV7_Curr, 10));
    data += ",A28:" + String(to_float(d.PV8_Volt, 10));
    data += ",A29:" + String(to_float(d.PV8_Curr, 10));
    data += ",A30:" + String(to_float(d.Uab, 10));
    data += ",A31:" + String(to_float(d.Ubc, 10));
    data += ",A32:" + String(to_float(d.Uca, 10));

    data += ",A33:" + String(to_float(d.Ua, 10));
    data += ",A34:" + String(to_float(d.Ub, 10));
    data += ",A35:" + String(to_float(d.Uc, 10));

    data += ",A36:" + String(to_float(d.Ia, 10));
    data += ",A37:" + String(to_float(d.Ib, 10));
    data += ",A38:" + String(to_float(d.Ic, 10));

    data += ",A39:" + String(to_float(d.Frequency, 100));
    data += ",A40:" + String(to_float(d.Power_factor, 1000));
    data += ",A41:" + String(to_float(d.Inverter_efficiency, 100));
    data += ",A42:" + String(to_float(d.Cabinet_temperature, 10));

    data += ",A43:" + String(to_float((uint32_t *)d.Active_power, 1000));
    data += ",A44:" + String(to_float((uint32_t *)d.Reactive_power, 1000));
    data += ",A45:" + String(to_float((uint32_t *)d.Total_input_power, 1000));

    data += ",A46:" + String(to_float((uint32_t *)d.E_Hour, 100));
    data += ",A47:" + String(to_float((uint32_t *)d.E_Day, 100));
    data += ",A48:" + String(to_float((uint32_t *)d.E_Month, 100));
    data += ",A49:" + String(to_float((uint32_t *)d.E_Year, 100));
    data += ",A50:" + String(to_float((uint32_t *)d.E_Total, 100));

    data += ",A51:" + String(to_u32((uint32_t *)d.Inverter_start_time));
    data += ",A52:" + String(to_u32((uint32_t *)d.Inverter_shutdown_time));

    data += ",A53:" + String(to_float((uint32_t *)d.MPPT1_Total_input_power, 1000));
    data += ",A54:" + String(to_float((uint32_t *)d.MPPT2_Total_input_power, 1000));
    data += ",A55:" + String(to_float((uint32_t *)d.MPPT3_Total_input_power, 1000));
    data += ",A56:" + String(to_float((uint32_t *)d.MPPT4_Total_input_power, 1000));

    String s = "";
#if API_VERSION == 1
    
#elif API_VERSION == 0
    s += APIHeader();
    s += ",HuaweiInverterList:[{";
    s += "HwId:" + String(id);
    if (status == false)
    {
        s += ",Status:0";
    }
    else
    {
        s += ",Status:1";
        s += data;
    }
    s += "}]}";
#endif

    return s;
}
#include "SolidQ.h"

int SolidQ50::id_count = 1;

bool SolidQ50::load()
{
    d = {};
    d2 = {};
    status = false;
    if (readIreg(0, (uint16_t *)&d, 59) == 0)
    {
        status = true;
    }
    if (readHreg(12, (uint16_t *)&d2.Inverter_Operation, 1) == 0)
    {
        status = true;
    }
    if (readHreg(2020, (uint16_t *)&d2.SerialNumber[0], 8) == 0)
    {
        status = true;
    }
    if (readHreg(5000, (uint16_t *)&d2.Grid_Code, 1) == 0)
    {
        status = true;
    }
    return status;
}

String SolidQ50::getString()
{
    //data
    String data = "";
    data += ",A0:" + String(to_u16(d.Device_Type) - 48);
    data += ",A1:" + String(to_float((uint32_t *)d.E_today, 10));
    data += ",A2:" + String(to_float((uint32_t *)d.E_total, 10));
    data += ",A3:" + String(to_float((uint32_t *)d.E_total));
    data += ",A4:" + String(to_u16(d.Device_State));
    data += ",A5:" + String(to_u16(d.Error_Code));
    data += ",A6:" + String(to_u16(d.Connect_Time));
    data += ",A7:" + String(to_float(d.Temperature, 10));
    data += ",A8:" + String(to_u16(d.Connection_Mode_PVStrings));
    data += ",A9:" + String(to_float(d.BusVoltage, 10));
    data += ",A10:" + String(to_float(d.PV1Voltage, 10));
    data += ",A11:" + String(to_float(d.PV1Current, 10));
    data += ",A12:" + String(to_float(d.PV2Voltage, 10));
    data += ",A13:" + String(to_float(d.PV2Current, 10));
    data += ",A14:" + String(to_float(d.PV3Voltage, 10));
    data += ",A15:" + String(to_float(d.PV3Current, 10));
    data += ",A16:" + String(to_float(d.L1_Phase_Voltage, 10));
    data += ",A17:" + String(to_float(d.L1_Phase_Current, 10));
    data += ",A18:" + String(to_float(d.L2_Phase_Voltage, 10));
    data += ",A19:" + String(to_float(d.L2_Phase_Current, 10));
    data += ",A20:" + String(to_float(d.L3_Phase_Current, 10));
    data += ",A21:" + String(to_float(d.L3_Phase_Voltage, 10));
    data += ",A22:" + String(to_float(d.Grid_Frequency, 10));
    data += ",A23:" + String(to_float((uint32_t *)d.Pac));
    data += ",A24:" + String(to_float((uint32_t *)d.Qac));
    data += ",A25:" + String((int16_t)to_u16(d.Power_Factor));
    data += ",A26:" + String(to_u16(d.Reactive_Power_Mode));
    data += ",A27:" + String(to_u16(d.Active_Power_Mode));
    data += ",A28:" + String(to_u16(d.Protocol_Version));
    data += ",A29:" + String(to_float(d.String_Current_1, 10));
    data += ",A30:" + String(to_float(d.String_Current_2, 10));
    data += ",A31:" + String(to_float(d.String_Current_3, 10));
    data += ",A32:" + String(to_float(d.String_Current_4, 10));
    data += ",A33:" + String(to_float(d.String_Current_5, 10));
    data += ",A34:" + String(to_float(d.String_Current_6, 10));
    data += ",A35:" + String(to_float(d.String_Current_7, 10));
    data += ",A36:" + String(to_float(d.String_Current_8, 10));
    data += ",A37:" + String(to_float(d.String_Current_9, 10));
    data += ",A38:" + String(to_float(d.String_Current_10, 10));
    data += ",A39:" + String(to_float(d.String_Current_11, 10));
    data += ",A40:" + String(to_float(d.String_Current_12, 10));
    data += ",A41:" + String(to_u16(d2.Inverter_Operation));
    data += ",A42:\"" + String((char *)d2.SerialNumber) + "\"";
    data += ",A43:" + String(to_u16(d2.Grid_Code));

    //api
    String s = "";
#if API_VERSION == 1
   
#elif API_VERSION == 0
    s += APIHeader();
    s += ",SolidQList:[{";
    s += "ID:" + String(local_id);
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
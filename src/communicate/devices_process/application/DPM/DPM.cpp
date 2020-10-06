#include "DPM.h"
int DPM::id_count = 1;

bool DPM::load()
{
    status = false;
    d = {};
    rs485->config(9600, SERIAL_8N1);
    if (readHreg(4000, (uint16_t *)&d, 88) == 0)
    {
        status = true;
    }
    return status;
}

String DPM::getString()
{
    // data
    String data = "";
    data += "V1:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L1)) / (float)10);
    data += ",V2:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L2)) / (float)10);
    data += ",V3:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L3)) / (float)10);
    data += ",V12:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L12)) / (float)10);
    data += ",V23:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L23)) / (float)10);
    data += ",V31:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Voltage_Phase_L31)) / (float)10);
    data += ",I1:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Instantaneous_Current_L1)) / (float)1000);
    data += ",I2:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Instantaneous_Current_L2)) / (float)1000);
    data += ",I3:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Instantaneous_Current_L3)) / (float)1000);
    data += ",PF:" + String(((int32_t)__bswap_16(d.Total_power_factor)) / (float)1000);
    data += ",Freq:" + String((__bswap_16(d.Frequency)) / (float)100);
    data += ",P1:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Real_Power_L1)) / (float)1000);
    data += ",P2:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Real_Power_L2)) / (float)1000);
    data += ",P3:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Real_Power_L3)) / (float)1000);
    data += ",AP1:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Apparent_Power_L1)) / (float)1000);
    data += ",AP2:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Apparent_Power_L2)) / (float)1000);
    data += ",AP3:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Apparent_Power_L3)) / (float)1000);
    data += ",ReP1:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Reactive_Power_L1)) / (float)1000);
    data += ",ReP2:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Reactive_Power_L2)) / (float)1000);
    data += ",ReP3:" + String(((int32_t)__bswap_32(*(int32_t *)&d.Reactive_Power_L3)) / (float)1000);
    data += ",TotalP:" + String((int32_t)(__bswap_32(*(int32_t *)&d.Total_real_power)) / (float)1000);
    data += ",TotalAP:" + String((int32_t)(__bswap_32(*(int32_t *)&d.Total_apparent_power)) / (float)1000);
    data += ",TotalReP:" + String((int32_t)(__bswap_32(*(int32_t *)&d.Total_reactive_power)) / (float)1000);
    data += ",PosEnergy:" + String(__bswap_32(*(int32_t *)&d.Positive_real_energ));
    data += ",NegEnergy:" + String(__bswap_32(*(int32_t *)&d.Negative_real_energy));

    // data += ",THDI1:" + String((__bswap_16(d.THD_Current_L1)) / (float)10); //TDHI1
    // data += ",THDI2:" + String((__bswap_16(d.THD_Current_L2)) / (float)10); //TDHI2
    // data += ",THDI3:" + String((__bswap_16(d.THD_Current_L3)) / (float)10); //TDHI3
    // data += ",THDV1:" + String((__bswap_16(d.THD_Voltage_L1)) / (float)10);//TDHV1
    // data += ",THDV2:" + String((__bswap_16(d.THD_Voltage_L2)) / (float)10);//TDHV2
    // data += ",THDV3:" + String((__bswap_16(d.THD_Voltage_L3)) / (float)10);//TDHV3

    // data += ",PF1:" + String(__bswap_16(d.Power_Factor_L1));//PF1
    // data += ",PF2:" + String(__bswap_16(d.Power_Factor_L2));//PF2
    // data += ",PF3:" + String(__bswap_16(d.Power_Factor_L3));//PF3
    // data += ",SPF1:" + String(__bswap_16(d.Sign_Power_Factor_L1));//SPF1
    // data += ",SPF2:" + String(__bswap_16(d.Sign_Power_Factor_L2));//SPF2
    // data += ",SPF3:" + String(__bswap_16(d.Sign_Power_Factor_L3));//SPF3
    // data += ",TotalSPF:" + String(__bswap_16(d.Sign_Total_Power_Factor));//TotalSPF

    // api
    String res = "";
#if API_VERSION == 1
    res += "{\"manufacture\":\"selec\"";
    res += ",\"md\":\"dpm 380\"";
    res += ",\"serial\":\"\"";
    res += ",\"code\":\"\",";
    res += "\"hwver\":\"\"";
    res += ",\"fwver\":\"\"";
    res += ",\"time\":\"\"";
    res += ",\"connection\":{\"status\":" + String(status ? 1 : 0);
    res += ",\"protocol\":1";
    res += ",\"id\":" + String(id);
    res += ",\"mid\":" + String(local_id);
    res += ",\"ip\":\"\"";
    res += ",\"port\":0";
    res += ",\"latency\":0}";
    res += ",\"state\":{\"state\":1";
    res += ",\"fault\":0";
    res += ",\"warning\":0}";
    res += ",\"data\":{";
    if (status == true)
    {
        res += data;
    }
    res += "},\"root\":\"\"}";

#elif API_VERSION == 0
    res += APIHeader();
    res += ",FullMetterList:[{";
    res += "MetterId:" + String(id);
    if (status == false)
    {
        res += ",Status:0";
    }
    else
    {
        res += ",Status:1,";
        res += data; /// data
    }
    res += "}]}";
#endif

    return res;
}
#include "HuaweiV3.h"
int HuaweiV3::id_count = 1;

bool HuaweiV3::load()
{
    d = {};
    status = false;
    // Serial number
    if (readHreg(30000, (uint16_t *)&d.Inverter_Model, 13) == 0)
    {
    }
    // model ID
    if (readHreg(30070, (uint16_t *)&d.Model_ID, 3) == 0)
    {
    }
    // state and alarm
    if (readHreg(32000, (uint16_t *)&d.State_1, 5) == 0)
    {
    }
    if (readHreg(32008, (uint16_t *)&d.Alarm_1, 3) == 0)
    {
    }

    // read string
    uint16_t num_strings = to_u16(d.Number_of_PV_Strings);
    if (num_strings != 0 && readHreg(32016, (uint16_t *)&d.PV1_Volt, 2 * num_strings) == 0)
    {
    }

    if (readHreg(32064, (uint16_t *)&d.Input_Power, 31) == 0)
    {
        status = true;
    }
    if (readHreg(32106, (uint16_t *)&d.E_total, 2) == 0)
    {
    }
    if (readHreg(32114, (uint16_t *)&d.E_day, 2) == 0)
    {
    }

    return status;
}

String HuaweiV3::getString()
{
    // data
    String data = "";
    data += ",A0:" + String(to_u16(d.Device_Status));
    data += ",A1:" + String(to_u16(d.Fault_Code));
    data += ",A2:" + String(to_u16(d.State_1));
    data += ",A3:" + String(to_u16(d.State_2));
    data += ",A4:" + String(to_u32((uint32_t *)d.State_3));
    data += ",A5:" + String(to_u16(d.Alarm_1));
    data += ",A6:" + String(to_u16(d.Alarm_2));
    data += ",A7:" + String(to_u16(d.Alarm_3));
    data += ",A8:" + String(to_u16(d.Model_ID));

    uint16_t num = to_u16(d.Number_of_PV_Strings);
    uint8_t k_ptr = 0;
    for (uint16_t i = 0; i < num; i++)
    {
        data += String(",A") + String(9 + k_ptr) + String(":") + to_float(*((int16_t *)&d.PV1_Volt + k_ptr), 10);
        data += String(",A") + String(9 + k_ptr + 1) + String(":") + to_float(*((int16_t *)&d.PV1_Volt + k_ptr + 1), 100);
        k_ptr += 2;
    }

    data += ",A57:" + String(to_float(d.Uab, 10));
    data += ",A58:" + String(to_float(d.Ubc, 10));
    data += ",A59:" + String(to_float(d.Uca, 10));

    data += ",A60:" + String(to_float(d.Ua, 10));
    data += ",A61:" + String(to_float(d.Ub, 10));
    data += ",A62:" + String(to_float(d.Uc, 10));

    data += ",A63:" + String(to_float((uint32_t *)d.Ia, 1000));
    data += ",A64:" + String(to_float((uint32_t *)d.Ib, 1000));
    data += ",A65:" + String(to_float((uint32_t *)d.Ic, 1000));

    data += ",A66:" + String(to_float(d.Grid_frequency, 100));
    data += ",A67:" + String(to_float(d.Power_factor, 1000));
    data += ",A68:" + String(to_float(d.Efficiency, 100));
    data += ",A69:" + String(to_float(d.Internal_temperature, 10));

    data += ",A70:" + String(to_float((uint32_t *)d.Active_power, 1000));
    data += ",A71:" + String(to_float((uint32_t *)d.Reactive_power, 1000));
    data += ",A72:" + String(to_float((uint32_t *)d.Input_Power, 1000));

    data += ",A73:" + String(to_float((uint32_t *)d.E_day, 100));
    data += ",A74:" + String(to_float((uint32_t *)d.E_total, 100));

    data += ",A75:" + String(to_u32((uint32_t *)d.Startup_time));
    data += ",A76:" + String(to_u32((uint32_t *)d.Shutdown_time));
    data += ",A77:" + String(to_u16(d.Number_of_PV_Strings));
    data += ",A78:" + String(to_u16(d.NUmber_of_MPPT_trackers));

    char str[15];
    memset(str, 0, 15);
    memcpy(str, d.Inverter_Model, 15);
    data += ",A79:\"" + String(str) + "\"";
    memset(str, 0, 15);
    memcpy(str, d.SN, 10);
    data += ",A80:\"" + String(str) + "\"";

    // api
    String s = "";
#if API_VERSION == 1
    res += "{\"manufacture\":\"Huawei\"";
    res += ",\"md\":\"solar inverter\"";
    res += ",\"serial\":\"" + String("") + "\"";
    res += ",\"code\":\"" + String("") + "\",";
    res += "\"hwver\":\"" + String("") + "\"";
    res += ",\"fwver\":\"" + String("") + "\"";
    res += ",\"time\":\"" String("") "\"";
    res += ",\"connection\":{\"status\":" + String(status ? 1 : 0);
    res += ",\"protocol\":1";
    res += ",\"id\":" + String(id);
    res += ",\"mid\":" + String(local_id);
    res += ",\"ip\":\"" + String("") + "\"";
    res += ",\"port\":0";
    res += ",\"latency\":0}";
    res += ",\"state\":{\"state\":" + String(0);
    res += ",\"fault\":" + String(0);
    res += ",\"warning\":" + String(0) + "}";
    res += ",\"data\":{";
    if (status == true)
    {
        res += data;
    }
    res += "},\"root\":\"\"}";
#elif API_VERSION == 0
    s += APIHeader();
    s += ",HuaweiInverterV3List:[{";
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
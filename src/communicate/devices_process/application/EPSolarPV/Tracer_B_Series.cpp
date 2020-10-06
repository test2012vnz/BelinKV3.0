#include "Tracer_B_Series.h"

int EPSolarTracer::id_count = 1;

bool EPSolarTracer::load()
{
    status = true;
    drated = {};
    dreal = {};
    dstatus = {};
    dpram = {};
    dtime = {};
    dcoils = {};

    rs485->config(115200, SERIAL_8N1);
    delay(100);
    if (readIreg(0x3000, (uint16_t *)&drated.PV_rated_voltage, 9) != 0 ||
        readIreg(0x300E, (uint16_t *)&drated.Load_rated_curent, 1) != 0)
    {
        status = false;
        return status;
    }
    if (readIreg(0x3100, (uint16_t *)&dreal.PV_voltage, 8) != 0 ||
        readIreg(0x310C, (uint16_t *)&dreal.Load_voltage, 7) != 0 ||
        readIreg(0x311A, (uint16_t *)&dreal.Battery_SOC, 2) != 0 ||
        readIreg(0x311D, (uint16_t *)&dreal.Battery_rated_power, 1) != 0)
    {
        status = false;
        return status;
    }

    if (readIreg(0x3200, (uint16_t *)&dstatus.Battery_status, 2) != 0)
    {
        status = false;
        return status;
    }
    readIreg(0x3202, (uint16_t *)&dstatus.Discharging_status, 1); // not sure

    if (readIreg(0x3300, (uint16_t *)&dpram.PV_input_today_volt_maximum, 15) != 0 ||
        readIreg(0x331A, (uint16_t *)&dpram.Battery_voltage, 5) != 0)
    {
        status = false;
        return status;
    }

    if (readHreg(0x9042, (uint16_t *)&dtime.Timming_1_ON_Sec, 12) != 0)
    {
        status = false;
        return status;
    }
    readCoil(0x0000, (bool*)&dcoils.Charging_device);
    readCoil(0x0002, &dcoils.Manual_control_load);
    readCoil(0x0003, &dcoils.Default_control_load);
    readCoil(0x0006, &dcoils.Force_control_load);
    return status;
}

String EPSolarTracer::getString()
{
    String data = "";
    data += ",\"A0\":" + String(to_u16(dstatus.Battery_status));
    data += ",\"A1\":" + String(to_u16(dstatus.Charging_status));
    data += ",\"A2\":" + String(to_u16(dstatus.Discharging_status));

    data += ",\"A3\":" + String(to_float(drated.PV_rated_voltage, 100));
    data += ",\"A4\":" + String(to_float(drated.PV_rated_current, 100));
    data += ",\"A5\":" + String((*(uint32_t *)drated.PV_rated_power) / 100);
    data += ",\"A6\":" + String(to_float(drated.Battery_rated_voltage, 100));
    data += ",\"A7\":" + String(to_float(drated.Battery_rated_current, 100));
    data += ",\"A8\":" + String((*(uint32_t *)drated.Battery_rated_power) / 100);
    data += ",\"A9\":" + String(to_u16(drated.Charging_mode));
    data += ",\"A10\":" + String(to_float(drated.Load_rated_curent, 100));

    data += ",\"A11\":" + String(to_float(dreal.PV_voltage, 100));
    data += ",\"A12\":" + String(to_float(dreal.PV_current, 100));
    data += ",\"A13\":" + String((*(uint32_t *)dreal.PV_power) / 100);
    data += ",\"A14\":" + String(to_float(dreal.Battery_voltage, 100));
    data += ",\"A15\":" + String(to_float(dreal.Battery_charing_current, 100));
    data += ",\"A16\":" + String((*(uint32_t *)dreal.Battery_charing_power) / 100);
    data += ",\"A17\":" + String(to_float(dreal.Load_voltage, 100));
    data += ",\"A18\":" + String(to_float(dreal.Load_current, 100));
    data += ",\"A19\":" + String((*(uint32_t *)dreal.Load_power) / 100);
    data += ",\"A20\":" + String(to_float(dreal.Battery_temperature, 100));
    data += ",\"A21\":" + String(to_float(dreal.Temperature_inside_case, 100));
    data += ",\"A22\":" + String(to_float(dreal.Temperature_hear_sink_surface, 100));
    data += ",\"A23\":" + String(to_u16(dreal.Battery_SOC));
    data += ",\"A24\":" + String(to_float(dreal.Temperature_remote, 100));
    data += ",\"A25\":" + String(to_float(dreal.Battery_rated_power, 100));

    data += ",\"A26\":" + String(to_float(dpram.PV_input_today_volt_maximum, 100));
    data += ",\"A27\":" + String(to_float(dpram.PV_input_today_volt_minimum, 100));
    data += ",\"A28\":" + String(to_float(dpram.Battery_today_volt_maximum, 100));
    data += ",\"A29\":" + String(to_float(dpram.Battery_today_volt_minimum, 100));
    data += ",\"A30\":" + String((*(uint32_t *)dpram.Energy_consumed_today) / 100);
    data += ",\"A31\":" + String((*(uint32_t *)dpram.Energy_comsumed_month) / 100);
    data += ",\"A32\":" + String((*(uint32_t *)dpram.Energy_comsumed_year) / 100);
    data += ",\"A33\":" + String((*(uint32_t *)dpram.Energy_comsumed_total) / 100);
    data += ",\"A34\":" + String((*(uint32_t *)dpram.Generate_energy_today) / 100);
    data += ",\"A35\":" + String((*(uint32_t *)dpram.Generate_energy_month) / 100);
    data += ",\"A36\":" + String((*(uint32_t *)dpram.Generate_energy_year) / 100);
    data += ",\"A37\":" + String((*(uint32_t *)dpram.Generate_energy_total) / 100);
    data += ",\"A38\":" + String((*(uint32_t *)dpram.Carbon_reduction) / 100);
    data += ",\"A39\":" + String(to_float(dpram.Battery_voltage, 100));
    data += ",\"A40\":" + String((*(uint32_t *)dpram.Battery_current) / 100);
    data += ",\"A41\":" + String(to_float(dpram.Battery_temperature, 100));
    data += ",\"A42\":" + String(to_float(dpram.Temperature_ambient, 100));

    data += ",\"A43\":" + String(to_u16(dtime.Timming_1_ON_Sec));
    data += ",\"A44\":" + String(to_u16(dtime.Timming_1_ON_Min));
    data += ",\"A45\":" + String(to_u16(dtime.Timming_1_ON_Hour));
    data += ",\"A46\":" + String(to_u16(dtime.Timming_1_OFF_Sec));
    data += ",\"A47\":" + String(to_u16(dtime.Timming_1_OFF_Min));
    data += ",\"A48\":" + String(to_u16(dtime.Timming_1_OFF_Hour));
    data += ",\"A49\":" + String(to_u16(dtime.Timming_2_ON_Sec));
    data += ",\"A50\":" + String(to_u16(dtime.Timming_2_ON_Min));
    data += ",\"A51\":" + String(to_u16(dtime.Timming_2_ON_Hour));
    data += ",\"A52\":" + String(to_u16(dtime.Timming_2_OFF_Sec));
    data += ",\"A53\":" + String(to_u16(dtime.Timming_2_OFF_Min));
    data += ",\"A54\":" + String(to_u16(dtime.Timming_2_OFF_Hour));

    data += ",\"A55\":" + String((int)dcoils.Charging_device);
    data += ",\"A56\":" + String((int)dcoils.Manual_control_load);
    data += ",\"A57\":" + String((int)dcoils.Default_control_load);
    data += ",\"A58\":" + String((int)dcoils.Force_control_load);

    String res = "";
#if API_VERSION == 1
    res += "{\"manufacture\":\"epsolar\"";
    res += ",\"md\":\"tracer_b_series\"";
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
    res += ",EPSolarTracerB:[{";
    res += "id:" + String(id);
    if (status == false)
    {
        res += ",Status:0";
    }
    else
    {
        res += ",Status:1,";
    }
    res += data; /// data
    res += "}]}";
#endif

    return res;
}
#include "SMA_RTU.h"
int SMARTUClass::id_count = 1;

bool SMARTUClass::load1()
{
    d = {};
    d2 = {};
    d3 = {};
    d4 = {};
    d5 = {};
    d6 = {};
    d7 = {};
    status = true;
    log_d("Load STP nn0000TL-10");
    //d1
    if (readHreg(30201, (uint16_t *)&d.Status_of_device, 2) != 0 ||
        readHreg(30211, (uint16_t *)&d.Recommended_action, 2) != 0 ||
        readHreg(30217, (uint16_t *)&d.Utility_grid_contactor, 2) != 0)
    {
        status = false;
    }
    //d2
    if (readHreg(30531, (uint16_t *)&d2.Total_AC_energy_fed_in_on_all_line_conductors_kWh, 4) != 0 ||
        readHreg(30541, (uint16_t *)&d2.Operating_time, 4) != 0)
    {
        return status;
    }
    //d3
    if (readHreg(30769, (uint16_t *)&d3.DC_current_input_1, 20) != 0 ||
        readHreg(30797, (uint16_t *)&d3.Line_current_of_line_conductor_L1, 10) != 0 ||
        readHreg(40009, (uint16_t *)&d7.SMA_Openrating_Status_Control, 2) != 0)
    {
        status = false;
        return status;
    }
    tcp_disconect();
    return status;
}
String SMARTUClass::getString1()
{
    String data = "";
    data += ",A0:" + String(to_u32((uint32_t *)d.Status_of_device));
    data += ",A1:" + String(to_u32((uint32_t *)d.Recommended_action));
    data += ",A2:" + String("0");
    data += ",A6:" + String(to_float((uint32_t *)d3.DC_voltage_input_1, 100));
    data += ",A7:" + String(to_float((uint32_t *)d3.DC_current_input_1, 1000));
    data += ",A8:" + String("0");
    data += ",A9:" + String("0");
    data += ",A10:" + String(to_float((uint32_t *)d3.DC_power_input_1, 1000));
    data += ",A11:" + String("0");
    data += ",A12:" + String(to_float((uint32_t *)d3.Active_power_on_all_line_conductors, 1000));
    data += ",A13:" + String(to_float((uint32_t *)d3.Reactive_power_on_all_line_conductors, 1000));
    data += ",A14:" + String(to_float((uint32_t *)d3.Power_frequency, 100));
    data += ",A15:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L1_to_N, 100));
    data += ",A16:" + String(to_float((uint32_t *)d3.Line_current_of_line_conductor_L1, 1000));
    data += ",A17:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L2_to_N, 100));
    data += ",A18:" + String(to_float((uint32_t *)d3.Line_current_of_line_conductor_L2, 1000));
    data += ",A19:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L3_to_N, 100));
    data += ",A20:" + String(to_float((uint32_t *)d3.Line_current_of_line_conductor_L3, 1000));
    data += ",A21:" + String(to_float((uint32_t *)d2.Total_AC_energy_fed_in_on_all_line_conductors_kWh));
    data += ",A22:" + String(to_float((uint32_t *)d2.Operating_time, 3600));
    data += ",A23:" + String("0");
    data += ",A24:" + String(to_float((uint32_t *)d7.SMA_Openrating_Status_Control));
    // api
    String res = "";
#if API_VERSION == 1

#elif API_VERSION == 0
    res += APIHeader();
    res += ",listRecord:[{";
    res += "InverterID:" + String(id);
    if (status == false)
    {
        res += ",Status:0";
    }
    else
    {
        res += ",Status:1";
        res += data; /// data
    }

    res += "}]}";
#endif

    return res;
}

bool SMARTUClass::load2()
{
    d = {};
    d2 = {};
    d3 = {};
    d4 = {};
    d5 = {};
    d6 = {};
    d7 = {};
    log_d("Load SMA TCP");
    status = true;
    if (readHreg(30201, (uint16_t *)&d, 48) != 0 ||
        readHreg(30531, (uint16_t *)&d2, 14) != 0)
    {
        status = false;
    }
    if (readHreg(30769, (uint16_t *)&d3, 38) != 0)
    {
        status = false;
    }
    if (readHreg(30957, (uint16_t *)&d4, 6) != 0)
    {
        status = false;
    }
    if (readHreg(30197, (uint16_t *)&d5, 2) != 0)
    {
        status = false;
    }
    if (readHreg(30977, (uint16_t *)&d6, 6) != 0)
    {
        status = false;
    }
    if (readHreg(40009, (uint16_t *)&d7, 2) != 0)
    {
        status = false;
    }
    tcp_disconect();
    return status;
}

String SMARTUClass::getString2()
{
    String data = "";
    data += ",A0:" + String(to_u32((uint32_t *)d.Status_of_device));
    data += ",A1:" + String(to_u32((uint32_t *)d.Recommended_action));
    data += ",A2:" + String(to_u32((uint32_t *)d.Status_Message));
    data += ",A6:" + String(to_float((uint32_t *)d3.DC_voltage_input_1, 100));
    data += ",A7:" + String(to_float((uint32_t *)d3.DC_current_input_1, 1000));
    data += ",A8:" + String(to_float((uint32_t *)d4.DC_voltage_input_2, 100));
    data += ",A9:" + String(to_float((uint32_t *)d4.DC_current_input_2, 1000));
    data += ",A10:" + String(to_float((uint32_t *)d3.DC_power_input_1, 1000));
    data += ",A11:" + String(to_float((uint32_t *)d4.DC_power_input_2, 1000));
    data += ",A12:" + String(to_float((uint32_t *)d3.Active_power_on_all_line_conductors, 1000));
    data += ",A13:" + String(to_float((uint32_t *)d3.Reactive_power_on_all_line_conductors, 1000));
    data += ",A14:" + String(to_float((uint32_t *)d3.Power_frequency, 100));
    data += ",A15:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L1_to_N, 100));
    data += ",A16:" + String(to_float((uint32_t *)d6.Grid_current_phase_L1, 1000));
    data += ",A17:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L2_to_N, 100));
    data += ",A18:" + String(to_float((uint32_t *)d6.Grid_current_phase_L2, 1000));
    data += ",A19:" + String(to_float((uint32_t *)d3.Line_voltage_line_conductor_L3_to_N, 100));
    data += ",A20:" + String(to_float((uint32_t *)d6.Grid_current_phase_L3, 1000));
    data += ",A21:" + String(to_float((uint32_t *)d2.Total_AC_energy_fed_in_on_all_line_conductors_kWh));
    data += ",A22:" + String(to_float((uint32_t *)d2.Operating_time, 3600));
    data += ",A23:" + String(to_float((uint32_t *)d2.Energy_fed_in_on_the_current_day_on_all_line_conductors_kWh));
    data += ",A24:" + String(to_float((uint32_t *)d7.SMA_Openrating_Status_Control));
    // api
    String res = "";
#if API_VERSION == 1

#elif API_VERSION == 0
    res += APIHeader();
    res += ",listRecord:[{";
    res += "InverterID:" + String(id);
    if (status == false)
    {
        res += ",Status:0";
    }
    else
    {
        res += ",Status:1";
        res += data; /// data
    }

    res += "}]}";
#endif

    return res;
}
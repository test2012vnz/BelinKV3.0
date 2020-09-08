#ifndef _SUNGROW_H_
#define _SUNGROW_H_

#include "../../mb.h"

typedef struct
{
    uint16_t Device_type;             //5000	device type
    uint16_t Nominal_active_power;    //5001
    uint16_t Output_type;             //5002
    uint16_t Daily_power_yiels;       //5003
    uint16_t Total_power_yields[2];   //5004 - 5005
    uint16_t Total_running_time[2];   //5006 - 5007
    int16_t Internal_temperature;     //5008
    uint16_t Total_Apparent_Power[2]; //5009 - 5010
    uint16_t DC_Voltage1;             //5011
    uint16_t DC_current1;             //5012
    uint16_t DC_Voltage2;             //5013
    uint16_t DC_current2;             //5014
    uint16_t DC_Voltage3;             //5015
    uint16_t DC_current3;             //5016
    uint16_t Total_DC_power[2];       //5017 - 5018
    uint16_t phase_A_voltage;         //5019
    uint16_t phase_B_voltage;         //5020
    uint16_t phase_C_voltage;         //5021
    uint16_t Phase_A_current;         //5022
    uint16_t Phase_B_current;         //5023
    uint16_t Phase_C_current;         //5024
    uint16_t Reservered1[6];          //5025 - 5030
    uint16_t Total_active_power[2];   //5031 - 5032
    int16_t Reactive_power[2];        //5033 - 5034
    int16_t Power_factor;             //5035
    uint16_t Grid_frequency;          //5036
    uint16_t Reservered2;             //5037
    uint16_t State;                   //5038
    uint16_t Reservered3[6];          //5039 - 5044
    uint16_t Fault;                   //5045
} Sungrow_110KW_Modbus_Register_1;

typedef struct
{
    uint16_t Work_state[2];               //5081 - 5082
    int16_t Metter_power[2];              //5083 - 5084
    int16_t Metter_phase_A_power[2];      //5085 - 5086
    int16_t Metter_phase_B_power[2];      //5087 - 5088
    int16_t Metter_phase_C_power[2];      //5089 - 5090
    int16_t Load_Power[2];                //5091 - 5092
    uint16_t Daily_export_energy[2];      //5093 - 5094
    uint16_t Total_export_energy[2];      //5095 - 5096
    uint16_t Daily_import_energy[2];      //5097 - 5098
    uint16_t Total_import_energy[2];      //5099 - 5100
    uint16_t Daily_energy_consumption[2]; //5101 - 5102
    uint16_t Total_energy_consumption[2]; //5103 - 5104
    uint16_t Reservered[8];
    uint16_t Daily_running_time;      //5113
    uint16_t Present_country;         //5114
    uint16_t DC_Voltage4;             //5115
    uint16_t DC_current4;             //5116
    uint16_t DC_Voltage5;             //5117
    uint16_t DC_current5;             //5118
    uint16_t DC_Voltage6;             //5119
    uint16_t DC_current6;             //5120
    uint16_t DC_Voltage7;             //5121
    uint16_t DC_current7;             //5122
    uint16_t DC_Voltage8;             //5123
    uint16_t DC_current8;             //5124
    uint16_t Reservered1;             //5125
    uint16_t Reservered2[2];          //5126 - 5127
    uint16_t Monthly_power_yields[2]; //5128 - 5129
    uint16_t DC_Voltage9;             //5130
    uint16_t DC_current9;             //5131
    uint16_t DC_Voltage10;            //5132
    uint16_t DC_current10;            //5133
    uint16_t DC_Voltage11;            //5134
    uint16_t DC_current11;            //5135
    uint16_t DC_Voltage12;            //5136
    uint16_t DC_current12;            //5137
} Sungrow_110KW_Modbus_Register_2;

typedef struct // 7013
{
    uint16_t String_1_current;  //7013
    uint16_t String_2_current;  //7014
    uint16_t String_3_current;  //7015
    uint16_t String_4_current;  //7016
    uint16_t String_5_current;  //7017
    uint16_t String_6_current;  //7018
    uint16_t String_7_current;  //7019
    uint16_t String_8_current;  //7020
    uint16_t String_9_current;  //7021
    uint16_t String_10_current; //7022
    uint16_t String_11_current; //7023
    uint16_t String_12_current; //7024
    uint16_t String_13_current; //7025
    uint16_t String_14_current; //7026
    uint16_t String_15_current; //7027
    uint16_t String_16_current; //7028
    uint16_t String_17_current; //7029
    uint16_t String_18_current; //7030
    uint16_t String_19_current; //7031
    uint16_t String_20_current; //7032
    uint16_t String_21_current; //7033
    uint16_t String_22_current; //7034
    uint16_t String_23_current; //7035
    uint16_t String_24_current; //7036

} Sungrow_110KW_Modbus_Combiner_Register;

class Sungrow : public MB
{

public:
    Sungrow(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = Constructor;
        status = false;
        local_id = t.info.RTU.ID;
    }
    ~Sungrow()
    {
        id_count--;
    }
    String toString()
    {
        return getString();
    }
    bool load();

private:
    bool status = false;
    uint8_t local_id = 0;
    Sungrow_110KW_Modbus_Register_1 d = {};
    Sungrow_110KW_Modbus_Register_2 d2 = {};
    Sungrow_110KW_Modbus_Combiner_Register d3 = {};
    static int id_count;
    float __swap_float(float i);
    static void Constructor()
    {
        id_count--;
    }
    String getString()
    {
        String data = "";
        data += ",A0:" + String(__bswap_16(d.State));
        data += ",A1:" + String(__bswap_32(*(uint32_t *)(&d2.Work_state)));
        data += ",A2:" + String(__bswap_16(d.Fault));
        data += ",A3:" + String(__bswap_32(*(uint32_t *)(&d.Total_Apparent_Power)));
        data += ",A4:" + String(__bswap_16(d.Device_type));
        data += ",A5:" + String(((int16_t)__bswap_16(d.Power_factor)) / (float)1000);

        data += ",A6:" + String(__bswap_16(d.DC_Voltage1) / (float)10);
        data += ",A7:" + String(__bswap_16(d.DC_current1) / (float)10);
        data += ",A8:" + String(__bswap_16(d.DC_Voltage2) / (float)10);
        data += ",A9:" + String(__bswap_16(d.DC_current2) / (float)10);
        data += ",A10:" + String(__bswap_16(d.DC_Voltage3) / (float)10);
        data += ",A11:" + String(__bswap_16(d.DC_current3) / (float)10);

        data += ",A12:" + String(__bswap_32(*(uint32_t *)(&d.Total_active_power)) / (float)1000);
        data += ",A13:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d.Reactive_power))) / (float)1000);
        data += ",A14:" + String(__bswap_16(d.Grid_frequency) / (float)10);

        data += ",A15:" + String(__bswap_16(d.phase_A_voltage) / (float)10);
        data += ",A16:" + String(__bswap_16(d.Phase_A_current) / (float)10);
        data += ",A17:" + String(__bswap_16(d.phase_B_voltage) / (float)10);
        data += ",A18:" + String(__bswap_16(d.Phase_B_current) / (float)10);
        data += ",A19:" + String(__bswap_16(d.phase_C_voltage) / (float)10);
        data += ",A20:" + String(__bswap_16(d.Phase_C_current) / (float)10);

        data += ",A21:" + String(__bswap_32(*(uint32_t *)(&d.Total_power_yields)));
        data += ",A22:" + String(__bswap_32(*(uint32_t *)(&d.Total_running_time)));
        data += ",A23:" + String(__bswap_16(d.Daily_power_yiels) / (float)10);
        data += ",A24:" + String(__bswap_16(d2.Daily_running_time)); // d2
        data += ",A25:" + String(__bswap_32(*(uint32_t *)(&d.Total_DC_power)) / (float)1000);
        data += ",A26:" + String(((int16_t)__bswap_16(d.Internal_temperature)) / (float)10);
        //27
        data += ",A27:" + String(__bswap_16(d2.DC_Voltage4) / (float)10);
        data += ",A28:" + String(__bswap_16(d2.DC_current4) / (float)10);
        data += ",A29:" + String(__bswap_16(d2.DC_Voltage5) / (float)10);
        data += ",A30:" + String(__bswap_16(d2.DC_current5) / (float)10);
        data += ",A31:" + String(__bswap_16(d2.DC_Voltage6) / (float)10);
        data += ",A32:" + String(__bswap_16(d2.DC_current6) / (float)10);
        data += ",A33:" + String(__bswap_16(d2.Present_country)); // d2

        data += ",A34:" + String(__bswap_16(d2.DC_Voltage7) / (float)10);
        data += ",A35:" + String(__bswap_16(d2.DC_current7) / (float)10);
        data += ",A36:" + String(__bswap_16(d2.DC_Voltage8) / (float)10);
        data += ",A37:" + String(__bswap_16(d2.DC_current8) / (float)10);
        data += ",A38:" + String(__bswap_16(d2.DC_Voltage9) / (float)10);
        data += ",A39:" + String(__bswap_16(d2.DC_current9) / (float)10);

        data += ",A40:" + String(__bswap_16(d2.DC_Voltage10) / (float)10);
        data += ",A41:" + String(__bswap_16(d2.DC_current10) / (float)10);
        data += ",A42:" + String(__bswap_16(d2.DC_Voltage11) / (float)10);
        data += ",A43:" + String(__bswap_16(d2.DC_current11) / (float)10);
        data += ",A44:" + String(__bswap_16(d2.DC_Voltage12) / (float)10);
        data += ",A45:" + String(__bswap_16(d2.DC_current12) / (float)10);

        data += ",A46:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d2.Metter_power))) / (float)1000);
        // data += ",A47:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d2.Metter_phase_A_power))) /(float)1000);
        // data += ",A46:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d2.Metter_phase_B_power))) /(float)1000);
        // data += ",A46:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d2.Metter_phase_C_power))) /(float)1000);
        // String combiner
        data += ",A50:" + String(__bswap_16(d3.String_1_current) / (float)100);
        data += ",A51:" + String(__bswap_16(d3.String_2_current) / (float)100);
        data += ",A52:" + String(__bswap_16(d3.String_3_current) / (float)100);
        data += ",A54:" + String(__bswap_16(d3.String_4_current) / (float)100);
        data += ",A55:" + String(__bswap_16(d3.String_5_current) / (float)100);
        data += ",A56:" + String(__bswap_16(d3.String_6_current) / (float)100);
        data += ",A57:" + String(__bswap_16(d3.String_7_current) / (float)100);
        data += ",A58:" + String(__bswap_16(d3.String_8_current) / (float)100);
        data += ",A59:" + String(__bswap_16(d3.String_9_current) / (float)100);
        data += ",A60:" + String(__bswap_16(d3.String_10_current) / (float)100);
        data += ",A61:" + String(__bswap_16(d3.String_11_current) / (float)100);
        data += ",A62:" + String(__bswap_16(d3.String_12_current) / (float)100);

        data += ",A63:" + String(__bswap_16(d3.String_13_current) / (float)100);
        data += ",A64:" + String(__bswap_16(d3.String_14_current) / (float)100);
        data += ",A65:" + String(__bswap_16(d3.String_15_current) / (float)100);
        data += ",A66:" + String(__bswap_16(d3.String_16_current) / (float)100);
        data += ",A67:" + String(__bswap_16(d3.String_17_current) / (float)100);
        data += ",A68:" + String(__bswap_16(d3.String_18_current) / (float)100);
        data += ",A69:" + String(__bswap_16(d3.String_19_current) / (float)100);
        data += ",A70:" + String(__bswap_16(d3.String_20_current) / (float)100);
        data += ",A71:" + String(__bswap_16(d3.String_21_current) / (float)100);
        data += ",A72:" + String(__bswap_16(d3.String_22_current) / (float)100);
        data += ",A73:" + String(__bswap_16(d3.String_23_current) / (float)100);
        data += ",A74:" + String(__bswap_16(d3.String_24_current) / (float)100);

        //api
        String res = "";
#if API_VERSION == 1
       

#elif API_VERSION == 0
        res += APIHeader();
        res += ",SungrowList:[{";
        res += "Id:" + String(local_id);
        if (!status == false)
        {
            res += ",Status:0";
        }
        else
        {
            res += ",Status:1";
            res += data;
        }
        res += "}]}";
#endif
        return res;
    }
};

#endif
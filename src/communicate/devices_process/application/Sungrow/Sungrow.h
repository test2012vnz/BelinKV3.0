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
    uint16_t Work_state[2]; //5081 - 5082
    //
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
    String getString();
    char SN[20];
};

#endif
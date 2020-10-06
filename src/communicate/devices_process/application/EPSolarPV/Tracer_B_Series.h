#ifndef _TRACER_B_SERIES_H
#define _TRACER_B_SERIES_H
#include "../../mb.h"
typedef struct
{
    uint16_t PV_rated_voltage;       // 3000 - HEX
    uint16_t PV_rated_current;       // 30001
    uint16_t PV_rated_power[2];      //3002-3003
    uint16_t Battery_rated_voltage;  //3004
    uint16_t Battery_rated_current;  //3005
    uint16_t Battery_rated_power[2]; // 3006 - 3007
    uint16_t Charging_mode;          //3008
    uint16_t Load_rated_curent;      //300E

} Tracer_Rated_Data_Structure;

typedef struct
{
    uint16_t PV_voltage;               // 3100
    uint16_t PV_current;               // 3101
    uint16_t PV_power[2];              // 3102-3103
    uint16_t Battery_voltage;          //3104
    uint16_t Battery_charing_current;  //3105
    uint16_t Battery_charing_power[2]; // 3106-3107
    // reserverd 5 addres
    uint16_t Load_voltage;                  // 310C  --------
    uint16_t Load_current;                  // 310D
    uint16_t Load_power[2];                 // 310E - 310F
    uint16_t Battery_temperature;           // 3110
    uint16_t Temperature_inside_case;       // 3111
    uint16_t Temperature_hear_sink_surface; // 3112
    // reservered
    uint16_t Battery_SOC;         // 311A
    uint16_t Temperature_remote;  // 311B
    uint16_t Reservered;          //311C
    uint16_t Battery_rated_power; // 311D

} Tracer_Real_Time_Data_Structure;

typedef struct
{
    uint16_t Battery_status;     //3200
    uint16_t Charging_status;    //3201
    uint16_t Discharging_status; //3202
} Tracer_Real_Time_Status_Structure;

typedef struct
{
    uint16_t PV_input_today_volt_maximum; //3300
    uint16_t PV_input_today_volt_minimum; //3301
    uint16_t Battery_today_volt_maximum;  //3302
    uint16_t Battery_today_volt_minimum;  //3303
    uint16_t Energy_consumed_today[2];    // 3304- 3305
    uint16_t Energy_comsumed_month[2];    //3306-3307
    uint16_t Energy_comsumed_year[2];     //3308-3309
    uint16_t Energy_comsumed_total[2];    //330A -330B
    uint16_t Generate_energy_today[2];    //330C -330D
    uint16_t Generate_energy_month[2];    //330E - 330F
    uint16_t Generate_energy_year[2];     //3310 - 3311
    uint16_t Generate_energy_total[2];    //3312-3313
    uint16_t Carbon_reduction[2];         //3314-3314
    //reserver
    uint16_t Battery_voltage;     // 3331A
    uint16_t Battery_current[2];  //331B-331C
    uint16_t Battery_temperature; //331D
    uint16_t Temperature_ambient; //331F

} Tracer_Statistical_Parameters_Structure;

typedef struct
{
    uint16_t Timming_1_ON_Sec;   //9042
    uint16_t Timming_1_ON_Min;   //9043
    uint16_t Timming_1_ON_Hour;  //9044
    uint16_t Timming_1_OFF_Sec;  //9045
    uint16_t Timming_1_OFF_Min;  //9046
    uint16_t Timming_1_OFF_Hour; //9047
    uint16_t Timming_2_ON_Sec;   //9048
    uint16_t Timming_2_ON_Min;   //9049
    uint16_t Timming_2_ON_Hour;  //904A
    uint16_t Timming_2_OFF_Sec;  //904B
    uint16_t Timming_2_OFF_Min;  //904C
    uint16_t Timming_2_OFF_Hour; //904D

} Tracer_Setting_Time_Structure;

typedef struct{
    bool Charging_device; //0
    bool Manual_control_load; //2
    bool Default_control_load;//3
    bool Force_control_load; //6
}Tracer_Switch_Value_Structure;

class EPSolarTracer : public MB
{
public:
    EPSolarTracer(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        local_id = t.info.RTU.ID;
        status = false;
        del = destructor;
    }
    ~EPSolarTracer()
    {
    }
    bool load();
    String toString()
    {
        return getString();
    }

private:
    bool status;
    bool local_id = 0;
    static int id_count;

    Tracer_Rated_Data_Structure drated = {};
    Tracer_Real_Time_Data_Structure dreal = {};
    Tracer_Real_Time_Status_Structure dstatus = {};
    Tracer_Statistical_Parameters_Structure dpram = {};
    Tracer_Switch_Value_Structure dcoils = {};

    Tracer_Setting_Time_Structure dtime = {};

    static void destructor()
    {
        id_count--;
    }
    String getString();
};

#endif
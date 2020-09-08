#ifndef _HUAWEIV3_H_
#define _HUAWEIV3_H_

#include "../../mb.h"

typedef struct
{

    // alarm
    char Inverter_Model[15]; //30000
    char SN[10];             //30015
    char PN[10];             //30025

    uint16_t Model_ID;                //30070
    uint16_t Number_of_PV_Strings;    //30071
    uint16_t NUmber_of_MPPT_trackers; //30072

    uint16_t State_1;    //32000
    uint16_t reserved1;  //32001
    uint16_t State_2;    //32002
    uint16_t State_3[2]; //32003

    uint16_t Alarm_1; //32008
    uint16_t Alarm_2; //32009
    uint16_t Alarm_3; //32010

    // pv struct
    int16_t PV1_Volt; //32016  --------
    int16_t PV1_Curr; //32017
    int16_t PV2_Volt; //32018
    int16_t PV2_Curr; //32019
    int16_t PV3_Volt; //32020
    int16_t PV3_Curr; //32021
    int16_t PV4_Volt; //32022
    int16_t PV4_Curr; //32023
    int16_t PV5_Volt; //32024
    int16_t PV5_Curr; //32025
    int16_t PV6_Volt; //32026
    int16_t PV6_Curr; //32027
    int16_t PV7_Volt; //32028
    int16_t PV7_Curr;
    int16_t PV8_Volt;
    int16_t PV8_Curr;
    int16_t PV9_Volt;
    int16_t PV9_Curr;
    int16_t PV10_Volt;
    int16_t PV10_Curr;
    int16_t PV11_Volt;
    int16_t PV11_Curr;
    int16_t PV12_Volt;
    int16_t PV12_Curr;
    int16_t PV13_Volt;
    int16_t PV13_Curr;
    int16_t PV14_Volt;
    int16_t PV14_Curr;
    int16_t PV15_Volt;
    int16_t PV15_Curr;
    int16_t PV16_Volt;
    int16_t PV16_Curr;
    int16_t PV17_Volt;
    int16_t PV17_Curr;
    int16_t PV18_Volt;
    int16_t PV18_Curr;
    int16_t PV19_Volt;
    int16_t PV19_Curr;
    int16_t PV20_Volt;
    int16_t PV20_Curr;
    int16_t PV21_Volt;
    int16_t PV21_Curr;
    int16_t PV22_Volt;
    int16_t PV22_Curr;
    int16_t PV23_Volt;
    int16_t PV23_Curr;
    int16_t PV24_Volt;
    int16_t PV24_Curr;

    int16_t Input_Power[2];                      //32064 --------
    uint16_t Uab;                                //32066
    uint16_t Ubc;                                //32067
    uint16_t Uca;                                //32068
    uint16_t Ua;                                 //32069
    uint16_t Ub;                                 //32070
    uint16_t Uc;                                 //32071
    int16_t Ia[2];                               //32072 - 32073
    int16_t Ib[2];                               //32074 - 32075
    int16_t Ic[2];                               //32076 - 32076
    int16_t Active_power_peak_of_current_day[2]; //32078 - 32079
    int16_t Active_power[2];                     //32080
    int16_t Reactive_power[2];                   //32082
    int16_t Power_factor;                        //32084
    uint16_t Grid_frequency;                     // 32085
    uint16_t Efficiency;                         //32087
    uint16_t Internal_temperature;               //32087
    uint16_t Insulation_resistance;              //32088
    uint16_t Device_Status;                      //32089
    uint16_t Fault_Code;                         //32090
    uint16_t Startup_time[2];                    //32091 - 32092
    uint16_t Shutdown_time[2];                   //32093 - 32094

    uint16_t E_total[2]; //32106 - 32107 -----
    uint16_t E_day[2];   //32114 - 32115 ------

} HuaweiV3_Structure;

class HuaweiV3 : public MB
{

public:
    HuaweiV3(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = destructor;
        status = false;
        local_id = t.info.RTU.ID;
    }
    ~HuaweiV3()
    {
    }

    bool load();
    String toString()
    {
        return getString();
    }

private:
    static void destructor()
    {
        id_count--;
    }
    bool status = false;
    uint8_t local_id = 0;
    static int id_count;

    HuaweiV3_Structure d = {};

    String getString();

    String floatToString(String key, float val)
    {
        if (val == 0.00)
        {
            return "";
        }
        return (key + String(val));
    }
};

#endif
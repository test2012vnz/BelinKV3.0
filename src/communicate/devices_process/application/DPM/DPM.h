#ifndef _DPM_H_
#define _DPM_H_

#include "../../mb.h"

typedef struct
{
    int16_t Negative_real_energy[2];      //1 -  uint16_t
    int16_t Positive_real_energ[2];       //3
    int16_t reservered1[2];               //5
    int16_t Apparent_energy[2];           //7
    int16_t Negative_reactive_energy[2];  //9
    int16_t Positive_reactive_energy[2];  //11
    int16_t Total_real_power[2];          //13
    int16_t Total_apparent_power[2];      //15
    int16_t Total_reactive_power[2];      //17
    int16_t Total_power_factor;           //18
    int16_t Frequency;                    //19
    int16_t Instantaneous_Current_L1[2];  //21
    int16_t Instantaneous_Current_L2[2];  //23
    int16_t Instantaneous_Current_L3[2];  //25
    int16_t Instantaneous_Current_Ln[2];  //27
    int16_t Voltage_Phase_L12[2];         //29
    int16_t Voltage_Phase_L23[2];         //31
    int16_t Voltage_Phase_L31[2];         //33
    int16_t Voltage_Phase_L1[2];          //35
    int16_t Voltage_Phase_L2[2];          //37
    int16_t Voltage_Phase_L3[2];          //39
    int16_t Real_Power_L1[2];             //41
    int16_t Real_Power_L2[2];             //43
    int16_t Real_Power_L3[2];             //45
    int16_t Apparent_Power_L1[2];         //47
    int16_t Apparent_Power_L2[2];         //49
    int16_t Apparent_Power_L3[2];         //51
    int16_t Reactive_Power_L1[2];         //53
    int16_t Reactive_Power_L2[2];         //55
    int16_t Reactive_Power_L3[2];         //57
    int16_t reservered2[6];               //63
    int16_t Real_Power_Demand[2];         //65
    int16_t Maximum_Real_Power_Demand[2]; //67
    int16_t reservered3[14];              //81
    int16_t THD_Current_L1;               //82
    int16_t THD_Current_L2;               //83
    int16_t THD_Current_L3;               //84
    int16_t THD_Voltage_L1;               //85
    int16_t THD_Voltage_L2;               //86
    int16_t THD_Voltage_L3;               //87
    int16_t Energy_full_flag;             //88
                                          // uint16_t Power_Factor_L1;//89
                                          // uint16_t Sign_Power_Factor_L1;//90
                                          // uint16_t Power_Factor_L2;//91
                                          // uint16_t Sign_Power_Factor_L2;//92
                                          // uint16_t Power_Factor_L3;//93
                                          // uint16_t Sign_Power_Factor_L3;//94
                                          // uint16_t Sign_Total_Power_Factor;//95
} DPM_Modbus_Register;

class DPM : public MB
{

public:
    DPM(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        local_id = t.info.RTU.ID;
        status = false;
        del = destructor;
    }
    ~DPM()
    {
        destructor();
    }

    bool load();
    String toString()
    {
        return getString();
    }

private:
    bool status;
    bool local_id = 0;
    DPM_Modbus_Register d = {};
    static int id_count;
    static void destructor()
    {
        id_count--;
    }

    String getString();
};

#endif
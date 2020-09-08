#ifndef _HUAWEIV2_H_
#define _HUAWEIV2_H_

#include "../../mb.h"

typedef struct
{
    // info
    uint16_t Model_ID;   //32001
    uint16_t Ouput_Mode; //32002
    char SN[10];
    char PN[10];

    // PV struct
    int16_t PV1_Volt;                            //32262        A14
    int16_t PV1_Curr;                            //32263        A15
    int16_t PV2_Volt;                            //32264        A16
    int16_t PV2_Curr;                            //32265        A17
    int16_t PV3_Volt;                            //32266        A18
    int16_t PV3_Curr;                            //32267        A19
    int16_t PV4_Volt;                            //32268        A20
    int16_t PV4_Curr;                            //32269        A21
    int16_t PV5_Volt;                            //32270        A22
    int16_t PV5_Curr;                            //32271        A23
    int16_t PV6_Volt;                            //32272        A24
    int16_t PV6_Curr;                            //32273        A25
    uint16_t Uab;                                //32274        A30
    uint16_t Ubc;                                //32275        A31
    uint16_t Uca;                                //32276        A32
    uint16_t Ua;                                 //32277        A33
    uint16_t Ub;                                 //32278        A34
    uint16_t Uc;                                 //32279        A35
    uint16_t Ia;                                 //32280        A36
    uint16_t Ib;                                 //32281        A37
    uint16_t Ic;                                 //32282        A38
    uint16_t Frequency;                          //32283        A39
    int16_t Power_factor;                        //32284        A40
    uint16_t Inverter_efficiency;                //32285        A41
    int16_t Cabinet_temperature;                 //32286        A42
    uint16_t Inverter_status;                    //32287        A0
    int16_t Active_power_peak_of_current_day[2]; //32288
    int16_t Active_power[2];                     //32290        A43
    int16_t Reactive_power[2];                   //32292        A44
    uint16_t Total_input_power[2];               //32294        A45
    uint16_t Reservered[2];
    uint16_t E_Hour[2];  //32298        A46
    uint16_t E_Day[2];   //32300        A47
    uint16_t E_Month[2]; //32302        A48
    uint16_t E_Year[2];  //32304        A49
    uint16_t E_Total[2]; //32306        A50
    uint16_t Reservered1[6];
    int16_t PV7_Volt; //32314        A26
    int16_t PV7_Curr; //32315        A27
    int16_t PV8_Volt; //32316        A28
    int16_t PV8_Curr; //32317        A29
    uint16_t Reservered2[7];
    uint16_t Inverter_start_time[2];    //32325        A51
    uint16_t Inverter_shutdown_time[2]; //32327        A52

    uint16_t MPPT1_Total_input_power[2]; //33022-33023   /1000
    uint16_t MPPT2_Total_input_power[2]; //33024-33025   /1000
    uint16_t MPPT3_Total_input_power[2]; //33026-33027   /1000
    uint16_t MPPT4_Total_input_power[2]; //33070-33071   /1000

    //Alarm
    uint16_t Alarm_1;  //50000
    uint16_t Alarm_2;  //50001
    uint16_t Alarm_3;  //50002
    uint16_t Alarm_4;  //50003
    uint16_t Alarm_5;  //50004
    uint16_t Alarm_6;  //50005
    uint16_t Alarm_7;  //50006
    uint16_t Alarm_8;  //50007
    uint16_t Alarm_9;  //50008
    uint16_t Alarm_10; //50009
    uint16_t Alarm_11; //50010
    uint16_t Alarm_12; //50011
    uint16_t Alarm_13; //50012
    uint16_t Alarm_14; //50013
    uint16_t Alarm_15; //50014
    uint16_t Alarm_16; //50015
    uint16_t Alarm_17; //50016

} Huawei_V2_Structure;

class HuaweiV2 : public MB
{
public:
    HuaweiV2(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = destructor;
        status = false;
        local_id = t.info.RTU.ID;
    }
    ~HuaweiV2()
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

    Huawei_V2_Structure d = {};

    String getString();

    bool alarm_read_ok = false;

};

#endif
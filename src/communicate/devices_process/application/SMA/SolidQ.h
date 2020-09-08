#ifndef _SOLIDQ_H_
#define _SOLIDQ_H_

#include "../../mb.h"

typedef struct
{
    uint16_t Device_Type;               // 30001
    uint16_t E_today[2];                // 30002
    uint16_t E_total[2];                // 30004
    uint16_t H_total[2];                // 30006
    uint16_t Reservered1[4];            // 30008-30011
    uint16_t Device_State;              // 30012
    uint16_t Error_Code;                // 30013
    uint16_t Connect_Time;              // 30014
    uint16_t Temperature;               // 30015
    uint16_t Connection_Mode_PVStrings; // 30016
    uint16_t BusVoltage;                // 30017
    uint16_t PV1Voltage;                // 30018
    uint16_t PV1Current;                // 30019
    uint16_t PV2Voltage;                // 30020
    uint16_t PV2Current;                // 30021
    uint16_t PV3Voltage;                // 30022
    uint16_t PV3Current;                // 30023
    uint16_t Reservered2[5];            // 30024-30028
    uint16_t L1_Phase_Voltage;          // 30029
    uint16_t L1_Phase_Current;          // 30030
    uint16_t L2_Phase_Voltage;          // 30031
    uint16_t L2_Phase_Current;          // 30032
    uint16_t L3_Phase_Voltage;          // 30033
    uint16_t L3_Phase_Current;          // 30034
    uint16_t Grid_Frequency;            // 30035
    uint16_t Reservered3[2];            // 30036-30037
    uint16_t Pac[2];                    // 30038
    uint16_t Qac[2];                    // 30040
    int16_t Power_Factor;               // 30042
    uint16_t Reactive_Power_Mode;       // 30043
    uint16_t Active_Power_Mode;         // 30044
    uint16_t Reservered4[2];            // 30045-30046
    uint16_t Protocol_Version;          // 30047
    uint16_t String_Current_1;          // 30048
    uint16_t String_Current_2;          // 30049
    uint16_t String_Current_3;          // 30050
    uint16_t String_Current_4;          // 30051
    uint16_t String_Current_5;          // 30052
    uint16_t String_Current_6;          // 30053
    uint16_t String_Current_7;          // 30054
    uint16_t String_Current_8;          // 30055
    uint16_t String_Current_9;          // 30056
    uint16_t String_Current_10;         // 30057
    uint16_t String_Current_11;         // 30058
    uint16_t String_Current_12;         // 30059
} SolidQ50_Structure;

typedef struct
{
    uint16_t Inverter_Operation; // 40013
    uint16_t SerialNumber[8];    // 42021-42028
    uint16_t Grid_Code;          // 45001
} SolidQ50_Structure_2;

class SolidQ50 : public MB
{

public:
    SolidQ50(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = destructor;
        status = false;
        local_id = t.info.RTU.ID;
    }
    ~SolidQ50()
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

    SolidQ50_Structure d = {};
    SolidQ50_Structure_2 d2 = {};

    String getString();

    uint16_t to_u16(uint16_t i)
    {
        return __bswap_16(i);
    }
    uint32_t to_u32(uint32_t *i)
    {
        return __bswap_32(*(uint32_t *)i);
    }
    float to_float(uint16_t i, uint16_t div = 1)
    {
        return __bswap_16(i) / (float)div;
    }
    float to_float(uint32_t *i, uint16_t div = 1)
    {
        return __bswap_32(*(uint32_t *)i) / (float)div;
    }
};

#endif
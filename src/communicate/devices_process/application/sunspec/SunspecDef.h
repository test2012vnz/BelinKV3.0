#ifndef _SUNSPECDEF_H_
#define _SUNSPECDEF_H_

typedef struct
{
    uint16_t addr;
    uint16_t sunspecID;
    uint16_t length;
} SunSpec_Scan_List_Structure;
//------------- UUID 1 -------------
typedef struct
{
    uint16_t ID;  // 40003
    uint16_t L;   // 40004
    char Mn[32];  // 40005
    char Md[32];  // 40021
    char Opt[16]; // 40037
    char Vr[16];  // 40045
    char SN[32];  // 40053
    uint16_t DA;
    uint16_t PA;
} SunSpec_Common_Model_Structure;

//------------- UUID 101/102/103 -------------
typedef struct
{
    uint16_t ID;
    uint16_t L;
    uint16_t A;
    uint16_t AphaA;
    uint16_t AphaB;
    uint16_t AphaC;
    int16_t A_SF;
    uint16_t PPVphAB;
    uint16_t PPVphBC;
    uint16_t PPVphCA;
    uint16_t PhVphA;
    uint16_t PhVphB;
    uint16_t PhVphC;
    int16_t V_SF;
    uint16_t W;
    int16_t W_SF;
    uint16_t Hz;
    int16_t Hz_SF;
    uint16_t VA;
    int16_t VA_SF;
    uint16_t VAr;
    int16_t VAr_SF;
    uint16_t PF;
    int16_t PF_SF;
    uint16_t WH_HIGH;
    uint16_t WH_LOW;
    int16_t WH_SF;
    uint16_t DCA;
    int16_t DCA_SF;
    uint16_t DCV;
    int16_t DCV_SF;
    uint16_t DCW;
    int16_t DCW_SF;
    int16_t TmpCab;
    int16_t TmpSnk;
    int16_t TmpTrns;
    int16_t TmpOt;
    int16_t Tmp_SF;
    uint16_t St;
    uint16_t StVnd;
    uint32_t Evt1;
    uint32_t Evt2;
    uint32_t EvtVnd1;
    uint32_t EvtVnd2;
    uint32_t EvtVnd3;
    uint32_t EvtVnd4;
} Sunspec_Inverter_Modbus_Register;
//------------- UUID 111 112 113 -----
typedef struct
{
    uint16_t ID;
    uint16_t L;
    float A;
    float AphaA;
    float AphaB;
    float AphaC;
    float PPVphAB;
    float PPVphBC;
    float PPVphCA;
    float PhVphA;
    float PhVphB;
    float PhVphC;
    float W;
    float Hz;
    float VA;
    float VAr;
    float PF;
    float WH;
    float DCA;
    float DCV;
    float DCW;
    float TmpCab;
    float TmpSnk;
    float TmpTrns;
    float TmpOt;
    uint16_t St;
    uint16_t StVnd;
    uint32_t Evt1;
    uint32_t Evt2;
    uint32_t EvtVnd1;
    uint32_t EvtVnd2;
    uint32_t EvtVnd3;
    uint32_t EvtVnd4;
} Sunspec_Inverter_Float_Modbus_Register;
//------------- UUID 160 -------------
typedef struct
{
    uint16_t ID;
    uint16_t L;
    int16_t DCA_SF;
    int16_t DCV_SF;
    int16_t DCW_SF;
    int16_t DCWH_SF;
    uint32_t Evt;
    uint16_t N;
    uint16_t TmsPer;
} Sunspec_Multiple_MPPT_Inverter_Register;

typedef struct
{
    uint16_t ID;
    char IDStr[16];
    uint16_t DCA;
    uint16_t DCV;
    uint16_t DCW;
    uint16_t DCWH_H;
    uint16_t DCWH_L;
    uint32_t Tms;
    uint16_t Tmp;
    uint16_t DCSt;
    uint32_t DCEvt;
} Sunspec_Multiple_MPPT_Inverter_Register_Repeating;

//------------- UUID 401 -------------

typedef struct
{
    uint16_t ID;
    uint16_t L;
    int16_t DCA_SF;
    int16_t DCAhr_SF;
    int16_t DCV_SF;
    int16_t DCAMAX;
    uint16_t N;
    uint16_t Evt_High;
    uint16_t Evt_Low;
    uint16_t EvtVnd_High;
    uint16_t EvtVnd_Low;
    // uint32_t Evt;
    // uint32_t EvtVnd;
    int16_t DCA;
    uint32_t DCAhr;
    int16_t DCV;
    int16_t Tmp;
    // Sunspec_String_Combiner_Node_UUID_401 *str;
} Sunspec_String_Combiner_UUID_401;

typedef struct
{
    uint16_t inID;
    uint16_t inEvt_High;
    uint16_t inEvt_Low;
    uint16_t inEvtVnd_High;
    uint16_t inEvtVnd_Low;
    int16_t inDCA;
    uint32_t inDCAhr;
} Sunspec_String_Combiner_Array_UUID_401_Reapeating;
//------------- -------------

union SunSpec_Data_Structure_Union {
    SunSpec_Common_Model_Structure common;                 //1
    Sunspec_Inverter_Modbus_Register inverter;             //101 102 103
    Sunspec_Inverter_Float_Modbus_Register float_inverter; //111 112 113
    Sunspec_Multiple_MPPT_Inverter_Register mppt;          //106
    Sunspec_String_Combiner_UUID_401 uuid401;              //401
};

union SunSpec_Repeating_Union {
    Sunspec_Multiple_MPPT_Inverter_Register_Repeating u160;
    Sunspec_String_Combiner_Array_UUID_401_Reapeating u401;
    SunSpec_Repeating_Union()
    {
    }
    SunSpec_Repeating_Union(Sunspec_Multiple_MPPT_Inverter_Register_Repeating in)
    {
        u160 = in;
    }
    SunSpec_Repeating_Union(Sunspec_String_Combiner_Array_UUID_401_Reapeating in)
    {
        u401 = in;
    }
    // create constructor and operator for new template
    SunSpec_Repeating_Union &operator=(Sunspec_Multiple_MPPT_Inverter_Register_Repeating in)
    {
        u160 = in;
    }
    SunSpec_Repeating_Union &operator=(Sunspec_String_Combiner_Array_UUID_401_Reapeating in)
    {
        u401 = in;
    }
};

struct SunSpec_Repeating_Structure{
    SunSpec_Repeating_Union data;
    SunSpec_Repeating_Structure *pNext;
    SunSpec_Repeating_Structure(){
        pNext = nullptr;
    }
    SunSpec_Repeating_Structure(SunSpec_Repeating_Union in){
        data = in;
        pNext = nullptr;
    }
};

struct Sunspec_Structure
{
    uint16_t id;
    SunSpec_Data_Structure_Union data;
    SunSpec_Repeating_Structure *repeat;
    Sunspec_Structure *pNext;
    Sunspec_Structure()
    {
        repeat = nullptr;
        pNext = nullptr;
    }
    Sunspec_Structure(SunSpec_Data_Structure_Union in, uint16_t ssid)
    {
        id = ssid;
        data = in;
        repeat = nullptr;
        pNext = nullptr;
    }
};

#endif
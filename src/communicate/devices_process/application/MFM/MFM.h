#ifndef _MFM_H_
#define _MFM_H_

#include "../../mb.h"

typedef struct
{
    float V1N;       //0
    float V2N;       //1
    float V3N;       //2
    float AverageLN; //3
    float V12;       //4
    float V23;       //5
    float V31;       //6
    float AverageLL; //7
    float I1;        //8
    float I2;        //9
    float I3;        //10
    float AverageI;  //11
    float kW1;       //12
    float kW2;       //13
    float kW3;       //14
    float kVA1;      //15
    float kVA2;      //16
    float kVA3;      //17
    float kVAr1;     //18
    float kVAr2;     //19
    float kVAr3;     //20
    float kW;        //21
    float kVA;       //22
    float kVAr;      //23
    float PF1;       //24
    float PF2;       //25
    float PF3;       //26
    float AvergePF;  //27
    float Frequency; //28
    float kWh;       //29
    float kVAh;      //30
    float kVArh;     //31
} MFM_Data_Structure;

class MFM : public MB
{
public:
    MFM(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = Constructor;
        status = false;
        local_id = t.info.RTU.ID;
    }

    ~MFM()
    {
        // log_d("MFM delete!!!");
        id_count--;
    }
    String toString()
    {
        return getString();
    }
    void *toData()
    {
        return &d;
    }
    bool load();

private:
    float __swap_float(float i);
    static void Constructor()
    {
        // log_d("MFM delete!!!");
        id_count--;
    }
    bool status = false;
    uint8_t local_id = 0;
    MFM_Data_Structure d = {};
    static int id_count;
    void getData()
    {
        if (status == true)
        {
            d.V1N = __swap_float(d.V1N);
            d.V2N = __swap_float(d.V2N);
            d.V3N = __swap_float(d.V3N);
            d.AverageLN = __swap_float(d.AverageLN);
            d.V12 = __swap_float(d.V12);
            d.V23 = __swap_float(d.V23);
            d.V31 = __swap_float(d.V31);
            d.I1 = __swap_float(d.I1);
            d.I2 = __swap_float(d.I2);
            d.I3 = __swap_float(d.I3);
            d.AvergePF = __swap_float(d.AvergePF);
            d.Frequency = __swap_float(d.Frequency);
            d.kW1 = __swap_float(d.kW1);
            d.kW2 = __swap_float(d.kW2);
            d.kW3 = __swap_float(d.kW3);
            d.kVAr1 = __swap_float(d.kVAr1);
            d.kVAr2 = __swap_float(d.kVAr2);
            d.kVAr3 = __swap_float(d.kVAr3);
            d.kW = __swap_float(d.kW);
            d.kVAh = __swap_float(d.kVAh);
            d.kVArh = __swap_float(d.kVArh);
            d.kWh = __swap_float(d.kWh);
        }
    }

    String getString()
    {

        String data = "";
        data += ",V1:" + String(d.V1N);
        data += ",V2:" + String(d.V2N);
        data += ",V3:" + String(d.V3N);
        data += ",V12:" + String(d.V12);
        data += ",V23:" + String(d.V23);
        data += ",V31:" + String(d.V31);
        data += ",I1:" + String(d.I1);
        data += ",I2:" + String(d.I2);
        data += ",I3:" + String(d.I3);
        data += ",PF:" + String(d.AvergePF);
        data += ",Freq:" + String(d.Frequency);
        data += ",P1:" + String(d.kW1);
        data += ",P2:" + String(d.kW2);
        data += ",P3:" + String(d.kW3);
        data += ",ReP1:" + String(d.kVAr1);
        data += ",ReP2:" + String(d.kVAr2);
        data += ",ReP3:" + String(d.kVAr3);
        data += ",TotalP:" + String(d.kW);
        data += ",TotalAP:" + String(d.kVAh);
        data += ",TotalReP:" + String(d.kVArh);
        data += ",PosEnergy:" + String(d.kWh);

        String s = "";
#if API_VERSION == 1
      
#elif API_VERSION == 0
        s += APIHeader();
        s += ",FullMetterList:[{";
        s += "MetterId:" + String(local_id);
        if (status == false)
        {
            s += ",Status:0";
        }
        else
        {
            s += ",Status:1";
            s += data;
        }
        s += "}]}";
#endif

        return s;
    }

protected:
};

#endif
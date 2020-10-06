#ifndef _KIPPZONEN_H_
#define _KIPPZONEN_H_

#include "../../mb.h"

typedef struct
{
    // uint16_t Kipp_Status;		//0
    uint16_t DevType;        //0
    uint16_t DataSet;        //1
    uint16_t DevMode;        //2
    uint16_t Status;         //3
    uint16_t reservered1;    //4
    int16_t Sensor1;         //5
    uint16_t reservered2[2]; //5-6
    int16_t PanelTemp;       //8
    int16_t VSupply;         //9

    uint16_t Error_code;     // 26
    uint16_t Protocol_Error; // 27

    uint16_t Batch_number;     // 41
    uint16_t Serial_number;    // 42
    uint16_t Software_version; // 43
    uint16_t Hardware_version; // 44
    uint16_t Node_ID;          // 45

    uint16_t Calibration_date[2]; //332 -333

} KippZonen_Register;

class KippZonen : public MB
{
public:
    KippZonen(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = Constructor;
        status = false;
        local_id = t.info.RTU.ID;
    }
    ~KippZonen()
    {
        id_count--;
    }

    String toString()
    {
        return getString();
    }

    bool load();

private:
    static void Constructor()
    {
        id_count--;
    }
    bool status = false;
    uint8_t local_id = 0;
    static int id_count;

    // convert int32 to ieee floating point
    float int32ToIeeefloat(int32_t ieee754_bits)
    {
        float flt;
        *((int *)&flt) = ieee754_bits;
        return flt;
    }

    KippZonen_Register d = {};
    String getString()
    {
        // data
        String data = "";
        data += ",A1:" + String((__bswap_16(d.DevType)));
        data += ",A2:" + String((__bswap_16(d.DataSet)));
        data += ",A3:" + String((__bswap_16(d.DevMode)));
        data += ",A4:" + String((__bswap_16(d.Status)));
        data += ",A5:" + String((__bswap_16(d.Sensor1)));
        data += ",A6:" + String(((int16_t)__bswap_16(d.PanelTemp)) * (float)0.1);
        data += ",A7:" + String((__bswap_16(d.VSupply)) * (float)0.1);

        uint32_t t = (uint32_t)__bswap_16(d.Calibration_date[0]) << 16 | __bswap_16(d.Calibration_date[1]);
        data += ",A8:" + String(int32ToIeeefloat(t));
        data += ",A9:" + String((__bswap_16(d.Error_code)));
        data += ",A10:" + String((__bswap_16(d.Protocol_Error)));
        data += ",A11:" + String((__bswap_16(d.Batch_number)));
        data += ",A12:" + String((__bswap_16(d.Serial_number)));
        data += ",A13:" + String((__bswap_16(d.Software_version)));
        data += ",A14:" + String((__bswap_16(d.Hardware_version)));
        data += ",A15:" + String((__bswap_16(d.Node_ID)));

        // api
        String s = "";
#if API_VERSION == 1
       
#elif API_VERSION == 0
        s += APIHeader();
        s += ",KippZonenList:[{";
        s += "DevID:" + String(id);
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
};

#endif
#ifndef _SUNSPEC_H_
#define _SUNSPEC_H_

#include "../../mb.h"
#include "SunspecDef.h"

class SunSpec : public MB
{

public:
    SunSpec(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        pHead = nullptr;
        id = id_count++;
        del = destructor;
        if (t.type == Device_Modbus_RTU_SunSpec)
        {
            local_id = t.info.RTU.ID;
        }
        else if (t.type == Device_Modbus_TCP_SunSpec)
        {
            local_id = t.info.TCP.ID;
        }
    }

    ~SunSpec()
    {
        log_d("Ok OK");
    }

    bool load();

    String toString();

private:
    /* data */
    static int id_count;
    static void destructor()
    {
        id_count--;
    }
    bool SunSpecCheck(uint16_t address);
    void SunSpecLoad(uint16_t address);
    void SunSpecLoad(SunSpec_Repeating_Structure **u, uint16_t address, uint16_t ssid, uint16_t N);

    void *d = nullptr;
    uint8_t local_id;
    bool status;
    String scanList = "";
    Sunspec_Structure *pHead;

    Sunspec_Structure *addNode(SunSpec_Data_Structure_Union u, uint16_t ssid);
    SunSpec_Repeating_Structure *addNode(SunSpec_Repeating_Structure **h, SunSpec_Repeating_Union in);

    void deleteList();

    void Print();
    String getString();
    String getString(SunSpec_Common_Model_Structure in);                                                                  //get common
    String getString(Sunspec_Inverter_Modbus_Register info);                                                              //get inverter
    String getString(Sunspec_Inverter_Float_Modbus_Register info);                                                        //get float inverter
    String getString(Sunspec_Multiple_MPPT_Inverter_Register info, SunSpec_Repeating_Structure *u);                           //get multiple mppt
    String getString(Sunspec_Multiple_MPPT_Inverter_Register_Repeating info, Sunspec_Multiple_MPPT_Inverter_Register sf); //
    String getString(Sunspec_String_Combiner_UUID_401 info, SunSpec_Repeating_Structure *u);                                  // get 401
    String getString(Sunspec_String_Combiner_Array_UUID_401_Reapeating info, Sunspec_String_Combiner_UUID_401 sf);        //

    template <class V>
    float sf_convert(V in, int16_t datasf)
    {
        float da = 0;
        if (!sf_checkNaN<int16_t>(datasf) || !sf_checkNaN<V>(in))
        {
            da /= 0;
            return da;
        }
        da = pow(10, datasf);
        // log_d("datasf %f", da);
        return in * da;
    }

    template <class V>
    bool sf_checkNaN(V in)
    {
        if (std::is_same<V, uint32_t>::value)
        {
            if ((uint32_t)in > 0x7FFFFFFF)
            {
                return false;
            }
        }
        else if (std::is_same<V, uint64_t>::value)
        {
            if ((uint64_t)in == 0)
            {
                return false;
            }
        }
        else if (std::is_same<V, uint16_t>::value)
        {
            if ((uint16_t)in > 0x7FFF)
            {
                return false;
            }
        }
        else if (std::is_same<V, int16_t>::value)
        {
            if ((int16_t)in == 0xFFFF8000 || in == 0x8000)
            {
                return false;
            }
        }
        else if (std::is_same<V, int32_t>::value)
        {
            if ((int32_t)in == 0x80000000)
            {
                return false;
            }
        }
        return true;
    }

    template <class V>
    String ValueString(String key, V in)
    {
        String res;
        if (sf_checkNaN<V>(in))
        {
            res = key + String(in);
            return res;
        }
        else
        {
            return "";
        }
    }

    uint16_t convert(uint16_t info)
    {
        return __bswap_16(info);
    }

    int16_t convert(int16_t info)
    {
        return (int16_t)__bswap_16(info);
    }

    float convert(float *i)
    {
        *(uint32_t *)(i) = ((((*(uint32_t *)i) & 0xFF) << 24 | (((*(uint32_t *)i) >> 8) & 0xFF) << 16 | (((*(uint32_t *)i) >> 16) & 0xFF) << 8 | (((*(uint32_t *)i) >> 24) & 0xFF)));
        return *i;
    }

    uint32_t convert(uint32_t info)
    {
        uint16_t high = info >> 16;
        uint16_t low = info & 0xff;
        high = __bswap_16(high);
        low = __bswap_16(low);
        return (high) | (low << 16);
    }

    int32_t convert(int32_t info)
    {
        uint16_t high = info >> 16;
        uint16_t low = info & 0xff;
        high = __bswap_16(high);
        low = __bswap_16(low);
        return (high) | (low << 16);
    }
    //------------------------------- float to ieee float ting point
    float int32ToIeeefloat(int32_t ieee754_bits)
    {
        float flt;
        *((int *)&flt) = ieee754_bits;
        return flt;
    }

    String FloatString(String key, float in)
    {
        String res;
        if ((!std::isnan(in)) && (!std::isinf(in)))
        {
            res = key + String(in);
            return res;
        }
        return "";
    }
};

#endif
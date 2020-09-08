#ifndef _DEVICE_LIST_H
#define _DEVICE_LIST_H

#include "byteswap.h"
#include "../../type.h"
#include "../../driver/memory/NVS/NVS.h"

#define DEVICES_PARTITION "device_list"
#define DEVICES_MAXIMUM 32

class DeviceInfo
{
public:
    DeviceInfo()
    {
    }
    ~DeviceInfo()
    {
    }
    int status = -1;
    int size = 0;
    Device_Info_Structure info[DEVICES_MAXIMUM];

private:
};

class DeviceList
{
public:
    DeviceList(String name = DEVICES_PARTITION)
    {
        name_partition = name;
    }
    ~DeviceList()
    {
    }

    static bool sync_to_process()
    {
        if (sync_fnc != nullptr)
        {
            sync_fnc(info.info, info.size);
            print();
            return true;
        }
        return false;
    }

    void set_sync_process_ptr(void (*fn)(Device_Info_Structure *, int))
    {
        sync_fnc = fn;
    }

    void set_flag_process_ptr(void (*fn)(bool))
    {
        flag_sync = fn;
    }

    void begin();

    static bool add(Device_Info_Structure i);
    // add RTU
    static bool add(Device_Name_Can_Be_Read dev, Device_Modbus_RTU_Struture d, uint8_t ss_en = 0, uint16_t SS_Address = 0);
    static bool add(Device_Name_Can_Be_Read dev, uint8_t id, uint8_t com, uint8_t ss_en = 0, uint16_t SS_Address = 0);
    // add TCP
    static bool add(Device_Name_Can_Be_Read dev, Device_Modbus_TCP_Struture d, uint8_t ss_en = 0, uint16_t SS_Address = 0);
    static bool add(Device_Name_Can_Be_Read dev, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en = 0, uint16_t SS_Address = 0);
    // add IEC
    static bool add(Device_Name_Can_Be_Read dev, Device_IEC_Structure d);
    static bool add(Device_Name_Can_Be_Read dev, char *ID, uint8_t com, uint16_t primary, uint16_t secondary);
    // delete
    static bool deleteAll();
    static bool del(Device_Info_Structure i);
    // delete RTU
    static bool del(Device_Name_Can_Be_Read dev, Device_Modbus_RTU_Struture d, uint8_t ss_en = 0);
    static bool del(Device_Name_Can_Be_Read dev, uint8_t id, uint8_t com, uint8_t ss_en = 0);
    // deltete TCP
    static bool del(Device_Name_Can_Be_Read dev, Device_Modbus_TCP_Struture d, uint8_t ss_en = 0);
    static bool del(Device_Name_Can_Be_Read dev, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en = 0);
    // delete IEC
    static bool del(Device_Name_Can_Be_Read dev, Device_IEC_Structure d);
    static bool del(Device_Name_Can_Be_Read dev, char *ID, uint8_t com, uint16_t primary, uint16_t secondary);

    static String toString(Device_Info_Structure *g, int x);
    static String toString();

    static int size();
    static String print();
    static void print(Device_Info_Structure d);

private:
    static String name_partition;
    static DeviceInfo info;
    static void (*sync_fnc)(Device_Info_Structure *, int);
    static void (*flag_sync)(bool);

    static bool commit();
    static bool sync(bool f_sync = true);
    static int exist(Device_Info_Structure in);
};

#endif
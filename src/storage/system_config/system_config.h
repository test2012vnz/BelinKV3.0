#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include "../../type.h"
#include "../../driver/memory/NVS/NVS.h"

#define SYSTEM_PARTITION "config_system"
#define NVS_SYSEM_FILE "NVS_SYSTEM"
#define NVS_SERIAL "NVS_SERIAL_NAME"

class SystemConfig
{
public:
    SystemConfig(String name = SYSTEM_PARTITION)
    {
        name_partition = name;
        
    }
    ~SystemConfig()
    {
    }

    void begin()
    {
        sync();
        update_reset_count();
        print();
    }

    bool update_serial_number(String serial);
    bool update_life_time(uint32_t t);
    bool update_reset_count();

    String get_serial_number(){
        return String(sysIf.Serial_Number);
    }
    uint32_t get_life_time()
    {
        return sysIf.Life_Time;
    }
    uint32_t get_reset_count()
    {
        return sysIf.Reset_Count;
    }
    System_Info_Structure get_sysif()
    {
        return sysIf;
    }

private:
    static String name_partition;
    static System_Info_Structure sysIf;

    static bool sync();
    static bool commit();

    static String print(){
        log_d("Serial:      %s", sysIf.Serial_Number);
        log_d("Reset count: %d", sysIf.Reset_Count);
        log_d("Life time:   %d", sysIf.Life_Time);
        return "";
    }
};

#endif
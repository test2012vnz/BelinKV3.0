#include "system_config.h"

String SystemConfig::name_partition = "";
System_Info_Structure SystemConfig::sysIf;

bool SystemConfig::update_serial_number(String serial)
{
    memset(sysIf.Serial_Number, 0, 20);
    memcpy(sysIf.Serial_Number, serial.c_str(), 20);
    return commit();
}

bool SystemConfig::update_life_time(uint32_t t)
{
    sysIf.Life_Time = t;
    return commit();
}

bool SystemConfig::update_reset_count()
{
    sysIf.Reset_Count +=1;
    return commit();
}

bool SystemConfig::sync()
{
    size_t len = NVS.getBytesLength(name_partition.c_str());
    log_d("-- len: %d %d", len, sizeof(System_Info_Structure));
    if (len == sizeof(System_Info_Structure))
    {
        NVS.getBytes(name_partition.c_str(), &sysIf, len);
        return true;
    }
    else if (len == 0 || len != sizeof(System_Info_Structure) || sysIf.status != 0x0A)
    {
        log_d("commit new sysif nvs");
        memset(&sysIf, 0, sizeof(System_Info_Structure));
        sysIf.status = 0x0A;
        return commit();
    }
}

bool SystemConfig::commit()
{
    size_t len = NVS.putBytes(name_partition.c_str(), &sysIf, sizeof(System_Info_Structure));
    if (!(len % sizeof(System_Info_Structure)))
    {
        return sync();
    }
    return false;
}
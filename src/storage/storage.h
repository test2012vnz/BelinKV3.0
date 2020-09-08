#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "system_config/system_config.h"
#include "net_config/net_config.h"
#include "devices_list/device_list.h"

class Storage
{
public:
    Storage()
    {
        xSemaphoreStorage = xSemaphoreCreateMutex();
        xSemaphoreGive(xSemaphoreStorage);

        sys = new SystemConfig();
        net = new NetConfig();
        dev = new DeviceList();
    }
    ~Storage()
    {
    }

    void begin()
    {
        net->begin();
        sys->begin();
        dev->begin();
    }

    // VARRIABLES
    SystemConfig *sys = nullptr;
    NetConfig *net = nullptr;
    DeviceList *dev =  nullptr;

private:
    SemaphoreHandle_t xSemaphoreStorage = nullptr;
};

#endif
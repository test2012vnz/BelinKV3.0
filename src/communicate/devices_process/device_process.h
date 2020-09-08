#ifndef _DEVICE_PROCESS_H_
#define _DEVICE_PROCESS_H_

#include "../../type.h"
#include "../../driver/driver.h"
#include "application/device_application.h"

class DeviceProcess
{
public:
    DeviceProcess(/* args */)
    {
    }
    ~DeviceProcess()
    {
    }

    void begin()
    {
        if (force_storage_sync != nullptr)
        {
            force_storage_sync();
        }
    }

    static bool hasSyncPoll()
    {
        return syncPoll;
    }
    static void setSyncPoll(bool i)
    {
        syncPoll = i;
    }
    // get function
    static uint8_t get_devices_read_OK()
    {
        return deviceReadOK;
    }
    // sync from storage
    static void add_handle(bool (*fn)())
    {
        force_storage_sync = fn;
    }
    static void sync_from_storage(Device_Info_Structure *in, int size);
    void load();
    static String devices_status();
    static uint32_t last_read_routine()
    {
        return last_routine;
    }
    static uint8_t total_devices()
    {
        return size;
    }

    static uint8_t total_devices_connected()
    {
        return deviceReadOK;
    }

private:
    static bool (*force_storage_sync)();
    static bool syncPoll; // flag to sync memory
    static DeviceBase_LinkedList list;
    static Device_Modbus_Mode RS485_Mode;
    static uint8_t deviceSendFailed;
    static uint8_t deviceReadOK;
    static uint32_t last_routine;
    static uint8_t size;
};

#endif
#ifndef _SYSTEM_PROCESS_H_
#define _SYSTEM_PROCESS_H_



#include "../../type.h"
#include "../../storage/system_config/system_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

	uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

class SystemProcess
{
public:
    SystemProcess()
    {
        start_up_time = seconds();
    }
    ~SystemProcess()
    {
    }

    void set_storage_ptr(SystemConfig *s)
    {
        sysConf = s;
    }

    void begin()
    {
        xTaskCreatePinnedToCore(this->system_task, "system task", 2084, this, 12, NULL, 1);
    }

    static uint32_t get_running_time(){
        return running_time;
    }

    static SystemConfig *sysConf;
    static String j_device();

private:
    static void system_task(void *arg);
    static uint32_t start_up_time;
    static uint32_t running_time;
};

#endif
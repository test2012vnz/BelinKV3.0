#include "system.h"

uint32_t SystemProcess::start_up_time = 0;
uint32_t SystemProcess::running_time = 0;
SystemConfig *SystemProcess::sysConf = nullptr;
void SystemProcess::system_task(void *arg)
{
    uint32_t life_count = seconds();
    for (;;)
    {
        uint32_t tsec = seconds() - life_count;
        if (tsec > 100)
        {
            sysConf->update_life_time(sysConf->get_life_time() + tsec);
            life_count = seconds();
        }
        running_time = seconds() - start_up_time;
        log_d("----------- Free RAM: %d Run time seconds: %d", ESP.getFreeHeap(), running_time);
        vTaskDelay(10000);
    }
}

String SystemProcess::j_device()
{
    String data = "\"uuid\":\"" +String("") + "\"";
    data += ",\"type\": \"" + String(DEVICE_MODEL) + "\"";
    data += ",\"serial\": \"" + sysConf->get_serial_number() + "\"";
    data += ",\"hardware_version\": \"" + String(DEVICE_HARDWARE) + "\"";
    data += ",\"firmware_version\": \"" + String(DEVICE_FIRMWARE) + "\"";
    data += ",\"boot\": \"" + String(0) + "\"";
    // status
    data += ",\"status\":{\"state\":\"" + String("normal") + "\"";
    data += ",\"fault_code\":" + String(0);
    data += ",\"waring_code\":[" + String(0) + "]}";
    // cpu
    data += ",\"cpu\":{\"type\":\"ESP32\"";
    data += ",\"usage\":\"\"}";
    // ram
    data += ",\"ram\":{\"type\":\"\"";
    data += ",\"usage\":" + String(ESP.getFreeHeap());
    data += ",\"total\":" + String(ESP.getHeapSize());
    data += ",\"type\": \"DDR3\"}";
    // temperature
    data += ",\"temp\":{\"cpu\":" + String((temprature_sens_read() - 32) / 1.8);
    data += ",\"main\": " + String(0) + "}";
    // storage
    data += ",\"storage\":{\"internal\":{\"usage\":" + String(0);
    data += ",\"total\":" + String(0) + "}";
    data += ",\"external\":{\"usage\":" + String(0);
    data += ",\"total\":24}}";
    return data;
}
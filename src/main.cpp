#include <Arduino.h>

#include "storage/storage.h"
#include "driver/driver.h"
#include "communicate/communicate.h"

Storage storage;
Driver driver;
Communicate com;

void set_handle()
{
    com.web->set_driver_ptr(&driver);
    com.web->set_json_ptr(&driver.jsonapi);

    driver.set_storage_ptr(&storage);
    driver.set_websocket_send_ptr(&com.web->websocket_send);
    driver.add_handle_total_devices(&com.process->total_devices);
    driver.add_handle_total_devices_connected(&com.process->total_devices_connected);

    storage.net->set_net_sync_handle(&driver.net->NetIF_set);
    storage.dev->set_flag_process_ptr(&com.process->setSyncPoll);
    storage.dev->set_sync_process_ptr(&com.process->sync_from_storage);

    com.process->add_handle(&storage.dev->sync_to_process);
}

void setup()
{
    Serial.begin(115200);
    // Set handle
    set_handle();

    // start components

    storage.begin();
    driver.begin();
    com.begin();
}

void loop()
{
    static uint32_t read_device = 0;
    if (seconds() - com.process->last_read_routine() > 180)
    {
        read_device = seconds();
        if (com.process->hasSyncPoll())
        {
            log_d("================Poll sync flag================1");
            com.process->setSyncPoll(false);
            storage.dev->sync_to_process();
        }
        com.process->load();
    }
    if (seconds() > 86400)
    {
        log_d("---- ESP RESTART ---- time");
        ESP.restart();
    }
    vTaskDelay(1000);
}


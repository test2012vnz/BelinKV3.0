#ifndef _COMMUNICATE_H_
#define _COMMUNICATE_H_

#include "webserver_app/webserverapp.h"
#include "devices_process/device_process.h"
#include "GoogleClient/GoogleClient.h"

class Communicate
{
public:
    Communicate()
    {
        web = new WebserverApplication();
        process = new DeviceProcess();
        google->begin();
    }
    ~Communicate()
    {
    }

    void begin()
    {
        web->begin();
        process->begin();
    }
    
    WebserverApplication *web = nullptr;
    DeviceProcess *process = nullptr;
    GoogleClient *google = nullptr;

private:
    /* data */
};

#endif
#ifndef _COMMUNICATE_H_
#define _COMMUNICATE_H_

#include "webserver_app/webserverapp.h"
#include "devices_process/device_process.h"

class Communicate
{
public:
    Communicate()
    {
        web = new WebserverApplication();
        process = new DeviceProcess();
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

private:
    /* data */
};

#endif
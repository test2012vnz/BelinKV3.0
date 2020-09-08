#ifndef _GOOGLECLIENT_H_
#define _GOOGLECLIENT_H_

#include <WiFiClientSecure.h>
#include "core/iot/src/CloudIoTCore.h"
#include "core/iot/src/CloudIoTCoreMqtt.h"

#define MAX_MQTT_LENGTH 40960

class GoogleClient
{

public:
    GoogleClient(/* args */)
    {
    }
    ~GoogleClient()
    {
    }

    void begin();
    void setup();
    void disconnect();

private:
    static Client *netClient;
    static CloudIoTCoreMqtt *mqttIotCore;
    static CloudIoTCoreDevice *device;
    static MQTTClient *mqttClient;
    const char *jwt;

    static TaskHandle_t TaskGoogle_t;
    static void GoogleIotCore_Task(void *arg);
};

#endif
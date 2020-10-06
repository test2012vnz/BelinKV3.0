#ifndef _GOOGLEIOTCORE_H_
#define _GOOGLEIOTCORE_H_

#include <WiFiClientSecure.h>
#include "mqtt/src/MQTT.h"
#include "jwt/jwt.h"

#define MQTT_MAXIMUM_LENGTH 4096
#define CLOUD_IOT_CORE_MQTT_HOST "mqtt.googleapis.com"
#define CLOUD_IOT_CORE_MQTT_PORT 8883

void messageReceived(String &topic, String &payload);

class GoogleIotCore
{
public:
    GoogleIotCore(/* args */)
    {
        netClient = new WiFiClientSecure();
        mqttClient = new MQTTClient(MQTT_MAXIMUM_LENGTH);
    }
    ~GoogleIotCore()
    {
    }

    void begin()
    {
        mqttClient->setOptions(180, true, 10000); // keepAlive, cleanSession, timeout
        mqttClient->begin(CLOUD_IOT_CORE_MQTT_HOST, CLOUD_IOT_CORE_MQTT_PORT, *netClient);
        mqttClient->onMessage(mssRecieveHandle);
        // xTaskCreate(this->MQTT_Task, "Google Task", 8096, this, 12, &GGC_Task_t);
    }

    void setup(const char *project_id, const char *location, const char *registry_id, const char *device_id, const char *private_key)
    {
        this->project_id = project_id;
        this->location = location;
        this->registry_id = registry_id;
        this->device_id = device_id;
        this->private_key = private_key;

        log_d("%s", this->project_id);
        log_d("%s", this->location);
        log_d("%s", this->registry_id);
        log_d("%s", this->private_key);
        fillPrivateKey();
    }
    //
    bool connected(){
        return mqttClient->connected();
    }

    // topic info function
    static String getClientId();
    static String getEventsTopic();
    static String getStateTopic();
    static String getCommandsTopic();
    static String getConfigTopic();

    static bool subscribe(String topic, uint8_t Qos);
    static bool publishTelemetry(String data);
    static bool publishTelemetry(String subtopic, String data);

    void add_handle_check(bool (*fn)())
    {
        check = fn;
    }
    void add_handle_mss(void (*fn)(String&, String&)){
        mssRecieveHandle = fn;
    }

private:
    //
    WiFiClientSecure *netClient = nullptr;
    static MQTTClient *mqttClient;
    static TaskHandle_t GGC_Task_t;
    static void MQTT_Task(void *arg);
    static bool (*check)();
    static void (*mssRecieveHandle)(String&, String&);
    // parameters
    static const char *project_id;
    static const char *location;
    static const char *registry_id;
    static const char *device_id;
    static const char *private_key;

    static NN_DIGIT priv_key[9];
    void fillPrivateKey()
    {
        memset(priv_key, 0, 8);
        priv_key[8] = 0;
        for (int i = 7; i >= 0; i--)
        {
            priv_key[i] = 0;
            for (int byte_num = 0; byte_num < 4; byte_num++)
            {
                priv_key[i] = (priv_key[i] << 8) + strtoul(private_key, NULL, 16);
                private_key += 3;
            }
        }
    }
};

#endif
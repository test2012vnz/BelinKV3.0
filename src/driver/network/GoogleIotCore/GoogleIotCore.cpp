#include "GoogleIotCore.h"

TaskHandle_t GoogleIotCore::GGC_Task_t = NULL;
MQTTClient *GoogleIotCore::mqttClient = nullptr;
bool (*GoogleIotCore::check)() = nullptr;
void (*GoogleIotCore::mssRecieveHandle)(String &, String &) = nullptr;

const char *GoogleIotCore::project_id;
const char *GoogleIotCore::location;
const char *GoogleIotCore::registry_id;
const char *GoogleIotCore::device_id;
const char *GoogleIotCore::private_key;

NN_DIGIT GoogleIotCore::priv_key[9] = {};

void messageReceived(String &topic, String &payload)
{
}

void GoogleIotCore::MQTT_Task(void *arg)
{
    uint32_t backoff = 5000;
    vTaskDelay(10000);
    log_d("-----------Task MQTT start-----------");

    for (;;)
    {
        if (check != nullptr && check())
        {
            if (mqttClient->connected())
            {
                mqttClient->loop();
                vTaskDelay(10);
            }
            else
            {

                log_d("---- reconnecting ggc mqtt %s", getClientId().c_str());
                String jwt = CreateJwt(project_id, time(nullptr), priv_key, 12 * 3600);
                log_d("jwt: %s", jwt.c_str());
                if (mqttClient->connect(getClientId().c_str(), "unused", jwt.c_str()))
                {
                    backoff = 5000;
                    // subscribe(getEventsTopic(), 0);
                    mqttClient->subscribe(getConfigTopic(), 1);
                    // QoS 0 (no ack) for commands
                    mqttClient->subscribe(getCommandsTopic(), 0);
                }
                else
                {
                    backoff += 5000;
                    if (backoff > 54000)
                    { // max 15 min
                        backoff = 54000;
                    }
                }
                vTaskDelay(backoff);
            }
        }
        else
        {
            log_d("check notification before connect mqtt");
            vTaskDelay(5000);
        }
    }
}

// TOPICS STORE
String GoogleIotCore::getClientId()
{
    return String("projects/") + project_id + "/locations/" + location +
           "/registries/" + registry_id + "/devices/" + device_id;
}
String GoogleIotCore::getEventsTopic()
{
    return String("/devices/") + device_id + "/events";
}
String GoogleIotCore::getStateTopic()
{
    return String("/devices/") + device_id + "/state";
}
String GoogleIotCore::getCommandsTopic()
{
    return String("/devices/") + device_id + "/commands/#";
}
String GoogleIotCore::getConfigTopic()
{
    return String("/devices/") + device_id + "/config";
}

// PUBLISH FUNCTIONS
bool GoogleIotCore::subscribe(String topic, uint8_t Qos)
{
    return mqttClient->subscribe(topic, Qos);
}

bool GoogleIotCore::publishTelemetry(String data)
{
    return mqttClient->publish(getEventsTopic(), data);
}

bool GoogleIotCore::publishTelemetry(String subtopic, String data)
{
    return mqttClient->publish(getEventsTopic() + subtopic, data);
}
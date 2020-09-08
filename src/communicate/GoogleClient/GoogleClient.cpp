#include "GoogleClient.h"

TaskHandle_t GoogleClient::TaskGoogle_t = NULL;

Client *GoogleClient::netClient = nullptr;
CloudIoTCoreMqtt *GoogleClient::mqttIotCore = nullptr;
CloudIoTCoreDevice *GoogleClient::device = nullptr;
MQTTClient *GoogleClient::mqttClient = nullptr;

void GoogleClient::begin()
{
    if (TaskGoogle_t == NULL)
    {
        xTaskCreate(this->GoogleIotCore_Task, "SWH Task", 8096, this, 12, &TaskGoogle_t);
    }
}
void GoogleClient::disconnect()
{
    vTaskDelete(TaskGoogle_t);
    TaskGoogle_t = NULL;
}
void GoogleClient::setup()
{
    if (device != nullptr)
    {
        delete device;
    }
    if (mqttClient != nullptr)
    {
        delete mqttClient;
    }
    if (mqttIotCore != nullptr)
    {
        delete mqttIotCore;
    }
    if (netClient != nullptr)
    {
        delete netClient;
    }
    device = new CloudIoTCoreDevice("", "", "", "", "");
    netClient = new WiFiClientSecure();
    mqttClient = new MQTTClient(MAX_MQTT_LENGTH);
    mqttClient->setOptions(180, true, 10000);
    mqttIotCore = new CloudIoTCoreMqtt(mqttClient, netClient, device);
    mqttIotCore->setUseLts(false);
    mqttIotCore->startMQTT();
}

void GoogleClient::GoogleIotCore_Task(void *arg)
{
    for (;;)
    {
        vTaskDelay(1000);
    }
}
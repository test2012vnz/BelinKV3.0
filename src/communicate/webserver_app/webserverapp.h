#ifndef _WEBSERVERAPP_H_
#define _WEBSERVERAPP_H_

#include "../../driver/driver.h"

#include "../../type.h"
#include <SPIFFS.h>
#include <WebServer.h>

#include "../../../lib/WebSockets/src/WebSocketsServer.h"
#include "../../../lib/ArduinoJson/ArduinoJson.h"
#include "../../../lib/Update/src/Update.h"

class WebserverApplication
{
public:
    WebserverApplication(/* args */)
    {
        server = new WebServer(80);
        webSocket = new WebSocketsServer(81);
    }
    ~WebserverApplication()
    {
    }
    void init();
    void begin()
    {
        init();
        SPIFFS.begin();
        server->begin();
        webSocket->begin();
        webSocket->onEvent(Websocket_Handler);
        xTaskCreatePinnedToCore(web_task, "WebServer Task", 10240, NULL, 2, NULL, 1);
    }

    //
    void set_driver_ptr(Driver *dr)
    {
        driver = dr;
    }
    void set_json_ptr(DynamicJsonBuffer *js)
    {
        jsonapi = js;
    }
    static bool websocket_send(String data)
    {
        if (WebSocketsStatus)
        {
            return webSocket->broadcastTXT(data);
        }
        return false;
    }

private:
    String getContentType(String filename);
    bool handleFileRead(String path);

    static WebServer *server;
    static WebSocketsServer *webSocket;
    static DynamicJsonBuffer *jsonapi;
    static bool WebSocketsStatus;

    static void Webserver_Handler();
    static void Websocket_Handler(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    static void web_task(void *arg);
    static String DeviceStatus(bool ret);

    static ACCOUNT_ENUM account;

    static Driver *driver;
};

#endif
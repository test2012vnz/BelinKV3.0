#include "network.h"

EventGroupHandle_t NetworkClass::NetIF_EventGroup = nullptr;
SemaphoreHandle_t NetworkClass::xSemaphoreGSM = nullptr;
NetIF_Task_Structure NetworkClass::netif = {};
WifiDriver *NetworkClass::wifi = nullptr;
Ethernet *NetworkClass::eth = nullptr;
GoogleIotCore *NetworkClass::gg = nullptr;

HTTPClient *NetworkClass::http = nullptr;

HardwareSerial *NetworkClass::sim_serial = nullptr;
TinyGsm *NetworkClass::modem = nullptr;
void (*NetworkClass::gsm_reset_fnc)() = nullptr;

uint8_t NetworkClass::state = 0;
uint32_t NetworkClass::Last_SSOC_Send = 0;
uint32_t NetworkClass::Last_SSOC_Send_Unix = 0;
Network_Time_Struct NetworkClass::Duration = {};

RTCClass *NetworkClass::rtc = nullptr;
uint32_t RTCClass::gmtOffset_sec = 0;

NetIF_Task_Structure NetworkClass::get_netif()
{
    return netif;
}

uint8_t NetworkClass::network_state()
{
    return state;
}
void NetworkClass::set_eth_reset_fnc(void (*rs)())
{
    eth->set_reset_fnc(rs);
}
void NetworkClass::set_eth_disable_fnc(void (*dis)())
{
    eth->set_disable_fnc(dis);
}

esp_err_t NetworkClass::setDefaultIf(tcpip_adapter_if_t tcpip_if)
{
    struct netif *_netif;
    esp_err_t ret = tcpip_adapter_get_netif(tcpip_if, (void **)&_netif);
    if (ret != ESP_OK)
    {
        return ret;
    }
    netif_set_default(_netif);
    return ESP_OK;
}

void NetworkClass::NetIF_set(NetIF_Task_Structure in)
{
    uint8_t e = 0;
    if (!(in.wifi == netif.wifi))
    {
        netif.wifi = in.wifi;
        e |= EVENT_STA_DISCONNECTED; //EVENT_STA_DISCONNECTED
    }

    if (!(in.eth == netif.eth))
    {
        netif.eth = in.eth;
        e |= EVENT_ETH_DISCONNECTED;
    }

    if (!(in.gsm == netif.gsm))
    {
        netif.gsm = in.gsm;
        e |= EVENT_GSM_DISCONNECTED;
    }

    netif.gg = in.gg;
    gg->setup(netif.gg.ProjectID, netif.gg.Location, netif.gg.Registry_id, netif.gg.Device_id, netif.gg.Private_Key);

    log_d("WIFI      -EN:%d  -SSID: %s -PASS: %s", netif.wifi.ENABLE, netif.wifi.SSID, netif.wifi.PASS);
    log_d("Ethernet  -EN:%d ", netif.eth.ENABLE);
    log_d("GSM       -EN:%d ", netif.gsm.ENABLE);

    xEventGroupSetBits(NetIF_EventGroup, e);
}

void NetworkClass::begin()
{
    modem = new TinyGsm(*sim_serial);
    gg->add_handle_check(ggc_check);
    gg->begin();
    xTaskCreate(this->Net_Task, "NetIF Task", 8192, this, 12, NULL);
}

void NetworkClass::wifi_init()
{
    log_d("--- wifi init: %d", netif.wifi.ENABLE);
    if (netif.wifi.ENABLE)
    {
        wifi->connect_sta(String(netif.wifi.SSID), String(netif.wifi.PASS));
    }
    else
    {
        wifi->disconnect();
    }
}

void NetworkClass::eth_int()
{
    log_d("---eth init %d", netif.eth.ENABLE);
    if (netif.eth.ENABLE == 1)
    {
        if (netif.eth.DHCP == 1)
        {
            eth->set_dhcp(1);
        }
        else
        {
            eth->set_dhcp(0, netif.eth.ip, netif.eth.subnet, netif.eth.gateway);
        }
        eth->init();
    }
    else
    {
        netif.eth.STATUS = 0;
        eth->disable();
    }
}

uint8_t NetworkClass::gsm_reset()
{
    uint16_t baud = sim_serial->baudRate();
    if (baud == 0)
    {
        sim_serial->begin(115200, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
    }
    else
    {
        sim_serial->updateBaudRate(115200, SERIAL_8N1);
    }
    uint8_t ret = 0;
    for (int i = 0; i < 3; i++)
    {
        if (i != 0)
        {
            log_d("--- gsm reinit %d", i);
        }
        if (gsm_reset_fnc != nullptr)
        {
            gsm_reset_fnc();
        }
        ret = modem->wait_RDY();
        if (ret)
        {
            break;
        }
    }
    return ret;
}

void NetworkClass::gsm_init()
{
    log_d("---gsm init %d", netif.gsm.ENABLE);
    if (xSemaphoreTake(xSemaphoreGSM, EVENT_TIME_WAIT_MAXIMUM))
    {
        if (netif.gsm.ENABLE)
        {
            if (gsm_reset() == 1)
            {
                log_d("--- gsm enable");
                uint8_t res = modem->initGPRS();
                if (res == 0)
                {
                    netif.gsm.STATUS = 0;
                    res = 0;
                    log_d("--- gsm init FAILED");
                }
                else
                {
                    netif.gsm.STATUS = 1;
                    res = 1;
                    log_d("--- gsm init OK");
                }
            }
        }
        else
        {
            netif.gsm.STATUS = 0;
            if (netif.gsm.STATUS)
            {
                bool res = modem->gprsDisconnect();
                if (res)
                {
                    log_d(" GSM Disconnected");
                }
            }
        }
        xSemaphoreGive(xSemaphoreGSM);
    }
}

bool ggc_init(){

}

void NetworkClass::Net_Task(void *arg)
{
    uint32_t WIFI_TIMEOUT = 300;
    uint32_t ETH_TIMEOUT = 300;
    uint32_t GSM_TIMEOUT = 300;

    uint64_t WIFI_DURATION_COUNT = 0;
    uint64_t ETH_DURATION_COUNT = 0;
    uint64_t GSM_DURATION_COUNT = 0;
    static EventBits_t connectionEvent;
    log_d("--- NetTask begin");
    for (;;)
    {
        connectionEvent = xEventGroupWaitBits(NetIF_EventGroup, 0xFF, pdTRUE, pdFALSE, EVENT_TIME_WAIT_MAXIMUM);

        if ((connectionEvent & EVENT_ETH_DISCONNECTED) != 0)
        {
            eth_int();
            vTaskDelay(10000);
        }

        if ((connectionEvent & EVENT_STA_DISCONNECTED) != 0)
        {
            wifi_init();
        }
        if ((connectionEvent & EVENT_GSM_DISCONNECTED) != 0)
        {
            gsm_init();
        }
        // WIFI TIME
        if (netif.wifi.ENABLE)
        {
            if (netif.wifi.STATUS)
            {
                Duration.Wifi_Connected_Duration += (seconds() - WIFI_DURATION_COUNT);
                Duration.Wifi_Disconnected_Duration = 0;
                WIFI_TIMEOUT = 300;
            }
            else
            {
                Duration.Wifi_Connected_Duration = 0;
                Duration.Wifi_Disconnected_Duration += (seconds() - WIFI_DURATION_COUNT);
                if (Duration.Wifi_Disconnected_Duration > WIFI_TIMEOUT)
                {
                    WIFI_TIMEOUT += 300;
                    xEventGroupSetBits(NetIF_EventGroup, EVENT_STA_DISCONNECTED);
                }
            }
        }
        else
        {
            WIFI_TIMEOUT = 300;
            WIFI_DURATION_COUNT = 0;
        }
        WIFI_DURATION_COUNT = seconds();
        // ETH TIME
        if (netif.eth.ENABLE)
        {
            if (netif.eth.STATUS)
            {
                Duration.Eth_Connected_Duration += (seconds() - ETH_DURATION_COUNT);
                Duration.Eth_Disconnected_Duration = 0;
            }
            else
            {
                Duration.Eth_Connected_Duration = 0;
                Duration.Eth_Disconnected_Duration += (seconds() - ETH_DURATION_COUNT);
            }
        }
        else
        {
            ETH_TIMEOUT = 0;
            ETH_DURATION_COUNT = 0;
        }
        ETH_DURATION_COUNT = seconds();
        // GSM TIME
        if (netif.gsm.ENABLE)
        {
            if (netif.gsm.STATUS)
            {
                Duration.Gsm_Connected_Duration += (seconds() - GSM_DURATION_COUNT);
                Duration.Gsm_Disconnected_Duration = 0;
            }
            else
            {
                Duration.Gsm_Connected_Duration = 0;
                Duration.Gsm_Disconnected_Duration += (seconds() - GSM_DURATION_COUNT);
            }
        }
        else
        {
            GSM_TIMEOUT = 0;
            GSM_DURATION_COUNT = 0;
        }
        GSM_DURATION_COUNT = seconds();
    }
    vTaskDelay(10);
}
void NetworkClass::WiFiEvent(WiFiEvent_t event)
{
    switch ((int)event)
    {
    case SYSTEM_EVENT_SCAN_DONE:
    {
    }
    break;
    case SYSTEM_EVENT_WIFI_READY:
    {
    }
    break;
    case SYSTEM_EVENT_STA_CONNECTED:
    {
    }
    break;
    case SYSTEM_EVENT_STA_GOT_IP:
    {
        setDefaultIf(TCPIP_ADAPTER_IF_STA);
        if (Ping.ping("google.com"))
        {
            netif.wifi.STATUS = 1;
            rtc->sync();
        }
        else
        {
            netif.wifi.STATUS = 0;
        }
    }
    break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    case SYSTEM_EVENT_STA_LOST_IP:
    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
        netif.wifi.STATUS = 0;
        xEventGroupSetBits(NetIF_EventGroup, EVENT_STA_DISCONNECTED);
    }
    break;
    case SYSTEM_EVENT_ETH_START:
    {
    }
    break;
    case SYSTEM_EVENT_ETH_GOT_IP:
    {
        setDefaultIf(TCPIP_ADAPTER_IF_ETH);
        if (Ping.ping("google.com"))
        {
            netif.eth.STATUS = 1;
            rtc->sync();
        }
        else
        {
            netif.eth.STATUS = 0;
        }
    }
    break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
    case SYSTEM_EVENT_ETH_STOP:
    {
        netif.eth.STATUS = 0;
    }
    break;
    }
}

bool NetworkClass::API_Send_Message(const String &api, bool is_swh_system)
{
    bool res = false;
#if API_VERSION == 1

#elif API_VERSION == 0
    if (SSOC_HTTP_Send(api, is_swh_system))
    {
        res = true;
        Last_SSOC_Send = seconds();
        Last_SSOC_Send_Unix = rtc->unixtimestamp();
    }
#endif
    return res;
}

bool NetworkClass::SSOC_HTTP_Send(const String &api, bool is_swh_system)
{
    bool ret = false;
    if (netif.wifi.STATUS == 1 || netif.eth.STATUS == 1)
    {
        if (netif.wifi.STATUS == 1)
        {
            // log_d("SET TCPIP_ADAPTER_IF_STA");
            setDefaultIf(TCPIP_ADAPTER_IF_STA);
        }
        else if (netif.eth.STATUS == 1)
        {
            // log_d("SET TCPIP_ADAPTER_IF_ETH");
            setDefaultIf(TCPIP_ADAPTER_IF_ETH);
        }
        for (uint8_t resend = 0; resend < 3; resend++)
        {
            if (!http->connected())
            {
                http->begin(SERVER_DOMAIN, SERVER_PORT, is_swh_system ? SWH_SECURE_URL : SECURE_URL);
            }
            http->addHeader("Content-Type", "application/json");
            int httpCode = http->POST(api);
            if (httpCode > 0)
            {
                if (httpCode == 200 || httpCode == 500)
                {
                    ret = true;
                }
            }
            http->end();
            if (ret)
            {
                resend = 10;
                break;
            }
        }
    }
    else if (netif.gsm.STATUS == 1)
    {
        for (uint8_t resend = 0; resend < 3; resend++)
        {
            if (resend != 0)
            {
                log_d("GSM Resend: %d", resend);
            }
            if (xSemaphoreTake(xSemaphoreGSM, 90000UL) == pdTRUE)
            {
                setDefaultIf(TCPIP_ADAPTER_IF_STA);
                uint32_t t = millis();
                TinyGsmClient client_(*modem);
                HttpClient GSMClient(client_, SERVER_DOMAIN, SERVER_PORT);
                GSMClient.connect(SERVER_DOMAIN, SERVER_PORT); //!importan timeout (second)
                log_d("--- Time to established: %d", (millis() - t));
                int err = GSMClient.post(is_swh_system ? SWH_SECURE_URL : SECURE_URL, "application/json", api);
                if (err == 0)
                {
                    int stat = GSMClient.responseStatusCode();
                    log_d("--- GSM: errr: %d     response code: %d", err, stat);
                    if (stat == 200 || stat == 500)
                    {
                        ret = true;
                    }
                    else
                    {
                        log_d("SSOC GSM|  Send ERR");
                    }
                }
                GSMClient.stop();
                xSemaphoreGive(xSemaphoreGSM);
                if (ret)
                {
                    resend = 10;
                    break;
                }
            }
        }
    }
    return ret;
}

bool NetworkClass::GGCP_MQTT_Send(const String &api){

}

bool RTCClass::sync(long gmtOffset_sec, int daylightOffset_sec, const char *server1, const char *server2, const char *server3)
{
    configTime(gmtOffset_sec, daylightOffset_sec, server1, server2, server3);
    time_t this_second = 0;
    uint32_t t = millis();
    while ((!this_second) && (millis() - t < 5000))
    {
        time(&this_second);
        vTaskDelay(100);
    }
    return check();
}
bool RTCClass::sync(tm tm)
{
    time_t t = mktime(&tm);
    struct timeval now = {.tv_sec = t};
    struct timezone tz = {};
    tz.tz_minuteswest = 25200 / 60;
    settimeofday(&now, &tz);
    return check();
}

bool RTCClass::check()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return false;
    }
    // log_d("internal rtc: %d:%d:%d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    // log_d("internal rtc: %d/%d/%d", timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
    log_d("%s", timeFormated().c_str());
    return true;
}

uint32_t RTCClass::unixtimestamp()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        return unixtimestamp(timeinfo);
    }
    return 0;
}

uint32_t RTCClass::unixtimestamp(tm ti)
{
    uint32_t t;
    byte m, d, h, min, second;
    uint16_t y;
    y = ti.tm_year + 1900;
    // y+=2000;
    m = ti.tm_mon + 1;
    d = ti.tm_mday;
    h = ti.tm_hour;
    min = ti.tm_min;
    second = ti.tm_sec;

    if (m <= 2)
    {
        m += 12;
        y -= 1;
    }
    //Convert years to days
    t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
    //Convert months to days
    t += (30 * m) + (3 * (m + 1) / 5) + d;
    //Unix time starts on January 1st, 1970
    t -= 719561;
    //Convert days to seconds
    t *= 86400;
    //Add hours, minutes and seconds
    t += (3600 * h) + (60 * min) + second;

    //Return Unix time
    return t;
}

String NetworkClass::j_get_network_info()
{
    String data = ",\"network\":{";
    data += "\"current\":" + String(0);
    // wifi data
    data += ",\"wifi\": {";
    data += "\"mac\": \"" + wifi->macAddress() + "\",";
    data += "\"dhcp\": \"" + String(0) + "\",";
    data += "\"ip\": \"" + wifi->localIP().toString() + "\",";
    data += "\"subnet\": \"" + wifi->subnetMask().toString() + "\",";
    data += "\"gateway\": \"" + wifi->gatewayIP().toString() + "\",";
    data += "\"dns\": \"" + wifi->dnsIP().toString() + "\",";
    data += "\"signal\": \"" + String(wifi->signal()) + "\",";
    data += "\"latency\": " + String(0);
    // eth data
    data += " },\"eth\": {";
    data += "\"mac\": \"" + eth->macAddress() + "\",";
    data += "\"dhcp\": \"" + String(eth->dhcp()) + "\",";
    data += "\"ip\": \"" + eth->localIP().toString() + "\",";
    data += "\"subnet\": \"" + eth->subnetMask().toString() + "\",";
    data += "\"gateway\": \"" + eth->gatewayIP().toString() + "\",";
    data += "\"dns\": \"" + eth->dnsIP().toString() + "\",";
    data += "\"speed\": \"" + String(eth->linKSpeed()) + "\",";
    data += "\"latency\": " + String(0);
    // gsm data
    data += "},\"gsm\": {";
    data += "\"service\": \"" + sim_service() + "\",";
    data += "\"imei\": \"15253135445\",";
    data += "\"number\": \"84969347205\",";
    data += "\"service\": " + String(sim_signal());
    data += "}}";
    return data;
}
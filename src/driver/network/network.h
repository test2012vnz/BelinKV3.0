#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "../../type.h"
#include "wifi/wifi.h"
#include "ethernet/ethernet.h"
#include "GoogleIotCore/GoogleIotCore.h"
#include "HTTPClient.h"

#include "../../../lib/ESP32Ping/ESP32Ping.h"
#define TINY_GSM_MODEM_BG96
#include "../../../lib/ArduinoHttpClient/src/ArduinoHttpClient.h"
#include "../../../lib/TinyGSM/src/TinyGsmClient.h"

extern "C"
{
#include "lwip/inet.h"
#include "lwip/udp.h"
#include <esp_err.h>
}

#define SIM_TX_PIN 13
#define SIM_RX_PIN 34

#define EVENT_TIME_WAIT_MAXIMUM 10000

#define EVENT_STA_CONNECTED (1 << 1)
#define EVENT_STA_DISCONNECTED (1 << 2)
#define EVENT_ETH_CONNECTED (1 << 3)
#define EVENT_ETH_DISCONNECTED (1 << 4)
#define EVENT_GSM_CONNECTED (1 << 5)
#define EVENT_GSM_DISCONNECTED (1 << 6)
#define EVENT_AP_DISCONNECTED (1 << 7)

typedef struct
{
    uint32_t Wifi_Connected_Duration;
    uint32_t Wifi_Disconnected_Duration;
    uint32_t Eth_Connected_Duration;
    uint32_t Eth_Disconnected_Duration;
    uint32_t Gsm_Connected_Duration;
    uint32_t Gsm_Disconnected_Duration;
} Network_Time_Struct;

class RTCClass
{
public:
    RTCClass(uint32_t offset = 25200)
    {
        gmtOffset_sec = offset;
    }
    ~RTCClass()
    {
    }

    void begin()
    {
    }
    bool sync(long gmtOffset_sec = gmtOffset_sec, int daylightOffset_sec = 0, const char *server1 = "asia.pool.ntp.org", const char *server2 = "asia.pool.ntp.org", const char *server3 = "sg.pool.ntp.org");
    bool sync(tm tm);
    bool check();
    uint32_t unixtimestamp();
    uint32_t unixtimestamp(tm ti);

    // time string
    String timeFormated()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
        }
        return timeFormated(timeinfo);
    }
    String timeFormated(tm timeinfo)
    {
        char buff[30] = {};
        sprintf(buff, "20%02d-%02d-%02d %02d:%02d:%02d", timeinfo.tm_year % 100, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        return String(buff);
    }

private:
    static uint32_t gmtOffset_sec;
};

class NetworkClass
{
public:
    NetworkClass(/* args */)
    {
        NetIF_EventGroup = xEventGroupCreate();
        xSemaphoreGSM = xSemaphoreCreateMutex();
        xSemaphoreGive(xSemaphoreGSM);
        eth = new Ethernet(&ETH, 1, -1, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN);
        WiFi.onEvent(WiFiEvent);
        wifi = new WifiDriver(&WiFi);
        http = new HTTPClient();
        rtc = new RTCClass(0);
        gg = new GoogleIotCore();
    }
    ~NetworkClass()
    {
    }

    // FUNCTION
    void begin();
    static uint8_t gsm_reset();
    static void wifi_init();
    static void eth_int();
    static void gsm_init();
    static bool ggc_init();
    // HANDLE FUNCTIONS
    static void NetIF_set(NetIF_Task_Structure in);
    static bool connect_ap(String ssid, String pass)
    {
        return wifi->connect_ap(ssid, pass);
    }

    // VARIABLES
    static NetIF_Task_Structure get_netif();
    static uint8_t network_state();
    void set_eth_reset_fnc(void (*rs)());
    void set_eth_disable_fnc(void (*dis)());
    void set_gsm_serial_ptr(HardwareSerial *s)
    {
        sim_serial = s;
    }
    void set_gsm_reset_fnc(void (*fnc)())
    {
        gsm_reset_fnc = fnc;
    }
    void set_gg_handle_fnc(void (*fn)(String&, String&)){
        gg->add_handle_mss(fn);
    }

    static bool network_connected()
    {
        bool i = netif.wifi.STATUS == 1 || netif.eth.STATUS == 1 || netif.gsm.STATUS == 1;
        if (!i)
        {
            Last_SSOC_Send = 0;
            Last_SSOC_Send_Unix = 0;
        }
        return i;
    }
    static bool ggc_check()
    {
        if (!network_connected())
        {
            return false;
        }
        if (String(netif.gg.ProjectID) == "" ||
            String(netif.gg.Location) == "" ||
            String(netif.gg.Registry_id) == "" ||
            String(netif.gg.Private_Key) == "")
        {
            log_d("GG Check False");
            return false;
        }
        return true;
    }
    int wifi_signal()
    {
        int8_t rssi = WiFi.RSSI();
        if (rssi == 0)
        {
            return 0;
        }
        return map(rssi, (-90), (-30), 0, 100);
    }
    int gsm_signal()
    {
        if (modem != nullptr && netif.gsm.STATUS == 1)
        {
            return map(modem->getSignalQuality(), 0, 31, 0, 100);
        }
        return 0;
    }
    static String getSimServiceProvider()
    {
        if (modem != nullptr && netif.gsm.STATUS == 1)
        {
            return modem->getServiceProvider();
        }
        return "";
    }
    static esp_err_t setDefaultIf(tcpip_adapter_if_t tcpip_if);
    static bool API_Send_Message(const String &api, bool is_swh_system = false);
    static bool SSOC_HTTP_Send(const String &api, bool is_swh_system = false);
    static bool GGCP_MQTT_Send(const String &api);
    static uint32_t Last_SSOC()
    {
        return Last_SSOC_Send;
    }
    static uint32_t Last_SSOC_Unix()
    {
        return Last_SSOC_Send_Unix;
    }

    static String api_ready_frame(String device, String data)
    {
        String api = "{";
        api += "\"time\": \""+rtc->timeFormated()+"\",";
        api += "\"packet_count\": 1,";
        api += "\"packet_total\": 1,";
        api += "\"packet_uuid\": \"hyshhay17719suyaaju1u1jaoo112\",";
        api += "\"manage_device_count\": 3,";
        api += device;
        api += j_get_network_info();
        api += ",\"manage_device_data\":[";
        api += data;
        api += "],\"ambient\":{";
        api += "},\"external_data\":{";
        api += "}}";
        return api;
    }
    // ETH FUNCTION
    static String eth_macAddress()
    {
        return eth->macAddress();
    }

    static IPAddress eth_localIP()
    {
        return eth->localIP();
    }
    static IPAddress eth_subnetMask()
    {
        return eth->subnetMask();
    }
    static IPAddress eth_gatewayIP()
    {
        return eth->gatewayIP();
    }

private:
    static void WiFiEvent(WiFiEvent_t event);
    static void Net_Task(void *arg);

    // VARIABLES
    static HardwareSerial *sim_serial;
    static TinyGsm *modem;
    static void (*gsm_reset_fnc)();
    static RTCClass *rtc;

    static EventGroupHandle_t NetIF_EventGroup;
    static SemaphoreHandle_t xSemaphoreGSM;
    static NetIF_Task_Structure netif;
    static uint8_t state;

    static WifiDriver *wifi;
    static Ethernet *eth;
    static GoogleIotCore *gg;
    static HTTPClient *http;

    static uint32_t Last_SSOC_Send;
    static uint32_t Last_SSOC_Send_Unix;

    static Network_Time_Struct Duration;

    static String sim_service()
    {
        if (modem != nullptr && netif.gsm.STATUS == 1)
        {
            return modem->getServiceProvider();
        }
        return "";
    }
    static int sim_signal()
    {
        if (modem != nullptr && netif.gsm.STATUS == 1)
        {
            return map(modem->getSignalQuality(), 0, 31, 0, 100);
        }
        return 0;
    }

    static String j_get_network_info();
};

#endif
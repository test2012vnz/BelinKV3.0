#ifndef _WIFI_H_
#define _WIFI_H_

#include <WiFi.h>

class WifiDriver
{
public:
    WifiDriver(WiFiClass *wf)
    {
        wifi = wf;
        wifi->mode(WIFI_AP_STA);
    }
    ~WifiDriver()
    {
    }

    void begin()
    {
        log_d("--- DEBUG: Wifi driver begin ---");
        wifi->setAutoReconnect(false);
    }
    bool connect_ap(String ap_ssid = "", String ap_pwd = "")
    {
        String ssid = ap_ssid;
        String pwd = ap_pwd;

        if (String(ssid) == String(""))
        {
            log_d("AP ssid is missing, change to default ssid");
            ssid = "BelinK_V2_" + String(wifi->macAddress());
        }
        if (pwd.length() < 8)
        {
            log_d("softAP password too short, change to default pwd");
            pwd = "12345678";
        }
        if (wifi == nullptr)
        {
            log_d("wifi null");
            return false;
        }
        return wifi->softAP(ssid.c_str(), pwd.c_str());
    }
    wl_status_t connect_sta(String sta_ssid, String sta_pwd)
    {
        if (wifi->isConnected())
        {
            return WL_CONNECTED;
        }

        if (String(sta_ssid).length() == 0)
        {
            return WL_NO_SSID_AVAIL;
        }
        int encryptionType = -1;
        int8_t nn = WiFi.scanNetworks(false, true);
        for (uint8_t i = 0; i < nn; i++)
        {
            String name = wifi->SSID(i);
            if (name == String(sta_ssid))
            {
                encryptionType = (wifi->encryptionType(i) == 0) ? 0 : 1;
                break;
            }
        }
        if (encryptionType < 0)
        {
            return WL_NO_SSID_AVAIL;
        }
        if (String(sta_pwd).length() < 8 && encryptionType != 0)
        {
            return WL_DISCONNECTED;
        }
        if (encryptionType == 0)
        {
            wifi->begin(sta_ssid.c_str());
        }
        else
        {
            wifi->begin(sta_ssid.c_str(), sta_pwd.c_str());
        }
        uint32_t time_start = millis();
        while (WL_CONNECTED != wifi->status() && (millis() - time_start < 30000))
        {
        }
        return wifi->status();
    }

    bool disconnect()
    {
        return wifi->disconnect();
    }
    String macAddress()
    {
        return wifi->macAddress();
    }
    IPAddress localIP()
    {
        return wifi->localIP();
    }
    IPAddress subnetMask()
    {
        return wifi->subnetMask();
    }
    IPAddress gatewayIP()
    {
        return wifi->gatewayIP();
    }
    IPAddress dnsIP()
    {
        return wifi->dnsIP(0);
    }

    int signal()
    {
        int s = wifi->RSSI();
        if(s == 0){
            return 0;
        }
        return map(s, (-90), (-30), 0, 100);
    }

private:
    WiFiClass *wifi = nullptr;
};

#endif
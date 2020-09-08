#include "device_base.h"

ModbusIP *DeviceBase::mIP = nullptr;

String DeviceBase::APIHeader(bool is_swh_system)
{
    System_Info_Structure sysif = Driver::get_sysif();
    NetIF_Task_Structure netif = Driver::get_netif();
    String s = "";
    if (is_swh_system)
    {
    }
    else
    {
        s += "{\"Serial\":\"" + String(sysif.Serial_Number) + "\",";
        s += "\"Reset_Count\":\"" + String(sysif.Reset_Count) + "\",";
        s += "\"Free_Ram\":\"" + String(ESP.getFreeHeap()) + "\",";
        s += "Communication:[{WIFI:" + String(Driver::wifi_signal());
        s += ",Ethernet:" + String(netif.eth.STATUS);
        s += ",GSM: " + String(Driver::gsm_signal()) + "}]";
        s += ",Version:\"" + String(DEVICE_FIRMWARE) + "\"";
        s += ",INTemp:" + String((temprature_sens_read() - 32) / 1.8);
    }
    return s;
}
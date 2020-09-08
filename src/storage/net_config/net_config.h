#ifndef _NET_CONFIG_H_
#define _NET_CONFIG_H_

#include "../../type.h"
#include "../../driver/memory/NVS/NVS.h"

#define NET_PARTITION "config_net"

class NetConfig
{

public:
    NetConfig(String name = NET_PARTITION)
    {
        name_partition = name;
    }
    ~NetConfig()
    {
    }

    void begin();

    static bool set(NetIF_Task_Structure i);
    static bool set(Wifi_Structure i);
    static bool set(Ethernet_Structure i);
    static bool set(GSM_3G_Structure i);

    static bool set_wifi(uint8_t e, char *ssid, char *pass);
    static bool set_eth(uint8_t e, uint8_t dhcp, uint32_t local_ip = 0, uint32_t gateway = 0, uint32_t subnet = 0);
    static bool set_gsm(uint8_t e);

    void set_net_sync_handle(void (*fn)(NetIF_Task_Structure))
    {
        sync_net = fn;
    }
    static NetIF_Task_Structure get_netif()
    {
        return netif;
    }

private:
    static String name_partition;
    static NetIF_Task_Structure netif;
    static void (*sync_net)(NetIF_Task_Structure);

    static bool sync();
    static bool commit();
};

#endif
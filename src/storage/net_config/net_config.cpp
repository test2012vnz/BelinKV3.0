#include "net_config.h"

String NetConfig::name_partition = "";
NetIF_Task_Structure NetConfig::netif = {};
void (*NetConfig::sync_net)(NetIF_Task_Structure) = nullptr;

void NetConfig::begin()
{
    sync();
}

bool NetConfig::sync()
{
    size_t len = NVS.getBytesLength(name_partition.c_str());
    log_d("-- len: %d %d", len, sizeof(NetIF_Task_Structure));
    if (len == sizeof(NetIF_Task_Structure))
    {
        NVS.getBytes(name_partition.c_str(), &netif, len);
        // sync netif
        if(sync_net != nullptr){
            sync_net(netif);
        }
        return true;
    }
    else if (len == 0 || len != sizeof(NetIF_Task_Structure) || netif.status != 0x0A)
    {
        log_d("commit new netif nvs");
        memset(&netif, 0, sizeof(NetIF_Task_Structure));
        netif.eth.DHCP = 1;
        netif.status = 0x0A;
        return commit();
    }
}

bool NetConfig::commit()
{
    size_t len = NVS.putBytes(name_partition.c_str(), &netif, sizeof(NetIF_Task_Structure));
    if (!(len % sizeof(NetIF_Task_Structure)))
    {
        return sync();
    }
    return false;
}

//*******************************************************
bool NetConfig::set(NetIF_Task_Structure i)
{
    netif = i;
    return commit();
}
bool NetConfig::set(Wifi_Structure i)
{
    netif.wifi = i;
    return set(netif);
}
bool NetConfig::set(Ethernet_Structure i)
{
    netif.eth = i;
    return set(netif);
}
bool NetConfig::set(GSM_3G_Structure i)
{
    netif.gsm = i;
    return set(netif);
}

//*******************************************************
bool NetConfig::set_wifi(uint8_t e, char *ssid, char *pass)
{
    Wifi_Structure i;
    memset(&i, 0, sizeof(Wifi_Structure));
    i.ENABLE = e;
    memcpy(i.SSID, ssid, String(ssid).length());
    memcpy(i.PASS, pass, String(pass).length());
    return set(i);
}

bool NetConfig::set_eth(uint8_t e, uint8_t dhcp, uint32_t local_ip, uint32_t gateway, uint32_t subnet)
{
    Ethernet_Structure i;
    memset(&i, 0, sizeof(Ethernet_Structure));
    i.ENABLE = e;
    i.DHCP = dhcp;
    i.ip = local_ip;
    i.gateway = gateway;
    i.subnet = subnet;
    return set(i);
}

bool NetConfig::set_gsm(uint8_t e)
{
    GSM_3G_Structure i;
    memset(&i, 0, sizeof(GSM_3G_Structure));
    i.ENABLE = e;
    return set(i);
}
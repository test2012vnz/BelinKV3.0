#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <ETH.h>
#include <HTTPClient.h>

class Ethernet
{

public:
    Ethernet(ETHClass *ETH, uint8_t phy_addr = ETH_PHY_ADDR, int power = ETH_PHY_POWER, int mdc = ETH_PHY_MDC, int mdio = ETH_PHY_MDIO, eth_phy_type_t type = ETH_PHY_TYPE, eth_clock_mode_t clk_mode = ETH_CLK_MODE)
    {
        _phy = phy_addr;
        _pow = power;
        _mdc = mdc;
        _mdio = mdio;
        _phy_type = type;
        _clk_mode = clk_mode;
        eth = ETH;
        httpClient = new HTTPClient;
    }
    ~Ethernet();

    void set_reset_fnc(void (*rs)())
    {
        resetfnc = rs;
    }
    void set_disable_fnc(void (*dis)())
    {
        disablefnc = dis;
    }
    void set_dhcp(uint8_t en, IPAddress IP = (uint32_t)0x00000000, IPAddress sub = (uint32_t)0x00000000, IPAddress gateway = (uint32_t)0x00000000)
    {
        dhcp_enable = en;
        if (!dhcp_enable)
        {
            static_ip = IP;
            static_subnet = sub;
            static_gateway = gateway;
        }
    }

    bool init()
    {
        // disable func
        disable();
        // reset module
        if (resetfnc)
        {
            resetfnc();
        }
        eth->begin(_phy, _pow, _mdc, _mdio, _phy_type, _clk_mode);
        if (dhcp_enable)
        {
            if (!eth->config({0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}))
            {
                log_d("STA DHCP Failed to configure");
                return false;
            }
        }
        else
        {
            if (!eth->config(static_ip, static_gateway, static_subnet, static_gateway))
            {
                log_d("STA Static IP Failed to configure");
                return false;
            }
        }
    }
    bool disable()
    {
        esp_eth_disable();
        disablefnc();
    }

    String macAddress()
    {
        return eth->macAddress();
    }

    IPAddress localIP()
    {
        return eth->localIP();
    }
    IPAddress subnetMask()
    {
        return eth->subnetMask();
    }
    IPAddress gatewayIP()
    {
        return eth->gatewayIP();
    }
    IPAddress dnsIP(){
        return eth->dnsIP();
    }
    uint8_t linKSpeed(){
        return eth->linkSpeed();
    }

    uint8_t dhcp(){
        return dhcp_enable;
    }

private:
    uint8_t _phy;
    int _pow;
    int _mdc;
    int _mdio;
    eth_phy_type_t _phy_type;
    eth_clock_mode_t _clk_mode;

    ETHClass *eth;
    HTTPClient *httpClient;

    void (*resetfnc)();
    void (*disablefnc)();

    uint8_t dhcp_enable;
    IPAddress static_ip;
    IPAddress static_subnet;
    IPAddress static_gateway;
};

#endif
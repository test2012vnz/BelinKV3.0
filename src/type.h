#ifndef _TYPE_H_
#define _TYPE_H_

#include <Arduino.h>

#define DEVICE_MODEL "BelinK_V2"
#define DEVICE_FIRMWARE "3.0.0B"
#define DEVICE_HARDWARE "2.0.0"
#define API_VERSION 0           // 0: fw 2.0, 1: google cloud

//
#define HTTP_DOMAIN "http://ssoc.solarbk.vn"
#define SERVER_DOMAIN "ssoc.solarbk.vn" //**0923
#define SERVER_PORT 80

#define CONTROL_URL "/api/pvsofar/control/endpoint"
#define SECURE_URL "/api/pvsofar/log"
#define OUTBACK_SECURE_URL "/api/pvsofar/outback/log"
#define SWH_SECURE_URL "/api/ProxyAPI/endpoint"
//
typedef enum
{
    Account_None = 0,
    Account_Admin,
    Account_Manufacturer,
    Account_Installer,
    Account_User
} ACCOUNT_ENUM;

//
typedef enum
{
    Modbus_Master = 0,
    Modbus_Slave = 0,
} Device_Modbus_Mode;
//

typedef enum
{
    Device_NULL = 0,
    Micro_DPM380,                   //1	RTU
    Selec_MFM_383,                  //2	RTU
    Vinasino_VSE3T_5B,              //3	IEC 62056
    Vinasino_VSE1T_10100B,          //4	IEC 62056
    SolarBK_Grid_Tie,               //5	RTU
    SolarBK_Hybrid,                 //6	RTU
    SMA_Grid_Tie,                   //7	TCP 		ss
    SMA_Battery_Inverter,           //8	TCP
    SMA_SolidQ,                     //9
    ABB_Grid_Tie,                   //10s	RTU 	ss
    Fronius_Grid_Tie,               //11	RTU 	ss
    SolarEdge_Grid_Tie,             //12	RTU 	ss
    Sungrow_Grid_Tie,               //13	RTU 	ss
    Huawei_Grid_Tie,                //14	RTU
    Huawei_Grid_Tie_V3,             //15    RTU
    Growat_TL3X,                    //16    RTU
    Kipp_and_Zonen_Solar_Radiation, //17    RTU
    IMT_Solar_Radiation,            //18    RTU
    SWH_System,                     //19    RTU     Master
    SMA_RTU,                        //20 rtu
    SMA_TCP,                        //21
    Device_List_Maximum
    // kaco                 ss
} Device_Name_Can_Be_Read;

//
typedef enum
{
    Communitcation_None = 0,
    Device_Modbus_RTU,
    Device_Modbus_TCP,
    Device_IEC62056,
    Device_Modbus_RTU_SunSpec,
    Device_Modbus_TCP_SunSpec,
    Device_Modbus_Max
} Device_Communitcation_Type;

//
struct Device_Modbus_RTU_Struture
{
    uint8_t ID;
    uint8_t com;
    uint8_t reservered[22];
    bool operator==(Device_Modbus_RTU_Struture &a)
    {
        return (ID == a.ID && com == a.com);
    }
    Device_Modbus_RTU_Struture &operator=(Device_Modbus_RTU_Struture &i)
    {
        ID = i.ID;
        com = i.com;
        return *this;
    }
};

struct Device_Modbus_TCP_Struture
{
    uint32_t IP;
    uint32_t port;
    uint8_t ID;
    uint8_t reservered[12];
    bool operator==(Device_Modbus_TCP_Struture &a)
    {
        return (ID == a.ID && IP == a.IP);
    }
    Device_Modbus_TCP_Struture &operator=(Device_Modbus_TCP_Struture &i)
    {
        ID = i.ID;
        IP = i.IP;
        port = i.port;
        return *this;
    }
};

struct Device_IEC_Structure
{
    char ID[18];
    uint16_t primary;
    uint16_t secondary;
    uint8_t com;
    uint8_t reserve;
    bool operator==(Device_IEC_Structure &a)
    {
        for (int i = 0; i < String(ID).length(); i++)
        {
            if (ID[i] != a.ID[i])
            {
                return false;
            }
        }
        return true;
    }
    Device_IEC_Structure &operator=(Device_IEC_Structure &i)
    {
        memset(ID, 0, 20);
        memcpy(ID, i.ID, 20);
        primary = i.primary;
        secondary = i.secondary;
        com = i.com;
        return *this;
    }
};

union Device_Protocol_Info {
    Device_Modbus_RTU_Struture RTU;
    Device_Modbus_TCP_Struture TCP;
    Device_IEC_Structure IEC;
    // char str[22];
}; // 22

struct Device_Info_Structure
{
    Device_Name_Can_Be_Read dev;
    Device_Communitcation_Type type;
    Device_Protocol_Info info;
    uint16_t SunSpec_Address;
    Device_Modbus_Mode mode;
    bool operator==(Device_Info_Structure &a)
    {
        switch ((int)type)
        {
        case Device_Modbus_RTU:
        case Device_Modbus_RTU_SunSpec:
        {
            return (info.RTU == a.info.RTU);
        }
        break;
        case Device_Modbus_TCP:
        case Device_Modbus_TCP_SunSpec:
        {
            return (info.TCP == a.info.TCP);
        }
        break;
        case Device_IEC62056:
        {
            return (info.IEC == a.info.IEC);
        }
        break;
        }
        return false;
    }
    Device_Info_Structure &operator=(Device_Info_Structure &i)
    {
        dev = i.dev;
        type = i.type;
        switch ((int)type)
        {

        case Device_Modbus_RTU:
        case Device_Modbus_RTU_SunSpec:
        {
            info.RTU = i.info.RTU;
        }
        break;
        case Device_Modbus_TCP:
        case Device_Modbus_TCP_SunSpec:
        {
            info.TCP = i.info.TCP;
        }
        break;
        case Device_IEC62056:
        {
            info.IEC = i.info.IEC;
        }
        break;
        }
        SunSpec_Address = i.SunSpec_Address;
        return *this;
    }
};

//-----------------------------------------------------------------------
struct Wifi_Structure
{
    uint8_t ENABLE;
    uint8_t STATUS;
    char SSID[64];
    char PASS[64];
    uint8_t DHCP = 1;
    uint32_t ip;
    uint32_t gateway;
    uint32_t subnet;
    bool operator==(Wifi_Structure &a)
    {
        return (ENABLE == a.ENABLE && String(SSID) == String(a.SSID) && String(PASS) == String(a.PASS) && DHCP == a.DHCP && ip == a.ip && gateway == a.gateway && subnet == a.subnet);
    }
    Wifi_Structure &operator=(Wifi_Structure &a)
    {
        memset(this, 0, sizeof(Wifi_Structure));
        ENABLE = a.ENABLE;
        DHCP = a.DHCP;
        ip = a.ip;
        gateway = a.gateway;
        subnet = a.subnet;
        memcpy(SSID, a.SSID, String(a.SSID).length());
        memcpy(PASS, a.PASS, String(a.PASS).length());
        return *this;
    }
};

struct Ethernet_Structure
{
    uint8_t ENABLE;
    uint8_t STATUS;
    uint8_t DHCP = 1;
    uint32_t ip;
    uint32_t gateway;
    uint32_t subnet;
    bool operator==(Ethernet_Structure &a)
    {
        return (ENABLE == a.ENABLE && DHCP == a.DHCP && ip == a.ip && gateway == a.gateway && subnet == a.subnet);
    }
    Ethernet_Structure &operator=(Ethernet_Structure &a)
    {
        ENABLE = a.ENABLE;
        DHCP = a.DHCP;
        ip = a.ip;
        gateway = a.gateway;
        subnet = a.subnet;
        return *this;
    }
};

struct GSM_3G_Structure
{
    uint8_t ENABLE;
    uint8_t STATUS;
    bool operator==(GSM_3G_Structure &a)
    {
        return (ENABLE == a.ENABLE);
    }
    GSM_3G_Structure operator=(GSM_3G_Structure &a)
    {
        ENABLE = a.ENABLE;
        return *this;
    }
};

struct Google_Registry_Structure
{
    uint8_t STATUS;
    char ProjectID[30];
    char Location[20];
    char Registry_id[30];
    char Device_id[30];
    char Private_Key[300];
    Google_Registry_Structure &operator=(Google_Registry_Structure &a)
    {
        memset(this, 0, sizeof(Google_Registry_Structure));
        STATUS = a.STATUS;
        memcpy(ProjectID, a.ProjectID, String(a.ProjectID).length());
        memcpy(Location, a.Location, String(a.Location).length());
        memcpy(Registry_id, a.Registry_id, String(a.Registry_id).length());
        memcpy(Device_id, a.Device_id, String(a.Device_id).length());
        memcpy(Private_Key, a.Private_Key, String(a.Private_Key).length());
        return *this;
    }
    
};

typedef enum
{
    Internet_Connection_None = 0,
    Internet_Connection_Wifi,
    Internet_Connection_Ethernet,
    Internet_Connection_GSM,
} Internet_Connection_Type;

typedef struct
{
    int status;
    Wifi_Structure wifi;
    Ethernet_Structure eth;
    GSM_3G_Structure gsm;
    Google_Registry_Structure gg;
} NetIF_Task_Structure;

typedef struct
{
    int status;
    char Serial_Number[20];
    uint32_t Reset_Count;
    uint32_t Life_Time;
} System_Info_Structure;

unsigned long IRAM_ATTR seconds();
extern const char *Device_TYPENAME[Device_Modbus_Max];
extern const char *DeviceLISTNAME[Device_List_Maximum];

#endif
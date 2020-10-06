#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "../../lib/ArduinoJson/ArduinoJson.h"

#include "connection/rs485/rs485.h"

#include "../storage/storage.h"
// #include "../communicate/communicate.h"
#include "network/network.h"
#include "system/system.h"
#include "memory/sdcard/sdcard.h"

/*HARDWARE PINS*/
#define HC595_DATA_PIN 33
#define HC595_LOAD_PIN 12
#define HC595_CLOCK_PIN 5
#define ETHERNET_RESET_PIN 4
#define ADC_VOLT_PIN 36
#define ADC_CURRENT_PIN 39

#define RS485_TX_PIN 32
#define RS485_RX_PIN 35

#define SDCARD_MISO_PIN 2
#define SDCARD_MOSI_PIN 15
#define SDCARD_CLOCK_PIN 14
/*END OF HARDWARE PINS*/

typedef enum
{
    Q0_WDT = 0,
    Q1_RELAY,
    Q2_EN3G,
    Q3_LEDCON,
    Q4_EN_OSC,
    Q5_LEDRUN,
    Q7_PWR_KEY_3G,
    Q6_RST_3G
} BIT_SET_595;

class Driver
{
public:
    Driver(/* args */)
    {
        net = new NetworkClass();
        sysDriver = new SystemProcess();
        PORT = new RS485PORT();
        sd = new SDCardCLass(SDCARD_CLOCK_PIN, SDCARD_MISO_PIN, SDCARD_MOSI_PIN);
    }
    ~Driver()
    {
    }

    void begin()
    {
        initPinOut();
        PORT->push(1, 1, RS485_RX_PIN, RS485_TX_PIN); //RS485

        net->connect_ap(storage->sys->get_serial_number(), "");
        simSerial = new HardwareSerial(2);
        net->set_gsm_serial_ptr(simSerial);
        net->set_gsm_reset_fnc(gsm_reset);
        net->set_eth_reset_fnc(ethernet_reset);
        net->set_eth_disable_fnc(ethernet_disable);
        net->set_gg_handle_fnc(google_mess_recieved_handle);
        net->begin();

        sysDriver->set_storage_ptr(storage->sys);
        sysDriver->begin();

        xTaskCreatePinnedToCore(this->WDT_Task, "WDT Task", 4096, this, 12, NULL, 1);
    }

    //set handle
    static void set_storage_ptr(Storage *st)
    {
        storage = st;
    }

    static void set_websocket_send_ptr(bool (*fnc)(String))
    {
        websocket_send_fnc = fnc;
    }
    // Hardware functions
    static void initPinOut();
    static uint32_t adc_read_radiation();
    static uint32_t adc_read_temperature();
    static void PrintSegment(uint8_t seg);
    static void ethernet_reset();
    static void ethernet_disable();
    static void gsm_reset();
    static RS485Driver *getPortDriver(int port)
    {
        return PORT->getPortDriver(port);
    }
    static int wifi_signal()
    {
        return net->wifi_signal();
    }
    static int gsm_signal()
    {
        return net->gsm_signal();
    }
    static String gsm_service()
    {
        return net->getSimServiceProvider();
    }
    // storage net config
    bool set_wifi(uint8_t e, char *ssid, char *pass);
    bool set_eth(uint8_t e, uint8_t dhcp, uint32_t local_ip = 0, uint32_t gateway = 0, uint32_t subnet = 0);
    bool set_gsm(uint8_t e);
    bool set_ggc(char *ProjectID, char* Location, char* Registry_id, char* Device_id, char* Private_Key);
    static NetIF_Task_Structure get_netif();
    static bool API_Send_Message(String api, bool is_swh_system = false);
    static bool websocket_send(String data)
    {
        if (websocket_send_fnc != nullptr)
        {
            return websocket_send_fnc(data);
        }
        return false;
    }

    // storage system config
    bool update_serial_number(String serial);
    static System_Info_Structure get_sysif();

    static int devices_size()
    {
        return storage->dev->size();
    }

    // communicate
    static uint8_t (*total_devices)();
    static uint8_t (*total_devices_connected)();

    void add_handle_total_devices(uint8_t (*fn)())
    {
        total_devices = fn;
    }
    void add_handle_total_devices_connected(uint8_t (*fn)())
    {
        total_devices_connected = fn;
    }
    // json
    String j_get_ambient();

    // storage devices config
    bool add(Device_Name_Can_Be_Read device, uint8_t id, uint8_t com, uint8_t ss_en, uint16_t SS_Address);
    bool add(Device_Name_Can_Be_Read device, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en = 0, uint16_t SS_Address = 0);
    bool add(Device_Name_Can_Be_Read device, char *ID, uint8_t com, uint16_t primary, uint16_t secondary);
    bool delRTU(Device_Name_Can_Be_Read device, uint8_t id, uint8_t com, uint8_t ss_en = 0);
    bool delTCP(Device_Name_Can_Be_Read device, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en = 0);
    bool delIEC(Device_Name_Can_Be_Read device, char *ID, uint8_t com, uint16_t primary, uint16_t secondary);
    String devicesListString();

    // GGC MQTT HANDLE
    static void google_mess_recieved_handle(String &topic, String& payload);

    //
    static NetworkClass *net;
    static SystemProcess *sysDriver;
    static DynamicJsonBuffer jsonapi;
    static SDCardCLass *sd;

private:
    static HardwareSerial *simSerial;
    static Storage *storage;
    static uint8_t HC595_Segment;
    static uint8_t Last_HC595_Segment;

    static RS485PORT *PORT;

    static void WDT_Task(void *arg);
    static bool (*websocket_send_fnc)(String);
};

#endif
#include "driver.h"

Storage *Driver::storage = nullptr;
NetworkClass *Driver::net = nullptr;
SystemProcess *Driver::sysDriver = nullptr;
DynamicJsonBuffer Driver::jsonapi;
SDCardCLass *Driver::sd = nullptr;

bool (*Driver::websocket_send_fnc)(String) = nullptr;

uint8_t Driver::HC595_Segment = 0;
uint8_t Driver::Last_HC595_Segment = 0;

RS485PORT *Driver::PORT = nullptr;

HardwareSerial *Driver::simSerial = nullptr;

uint8_t (*Driver::total_devices)() = nullptr;
uint8_t (*Driver::total_devices_connected)() = nullptr;

// STORAGE DRIVER NET FUNCTIONS
bool Driver::set_wifi(uint8_t e, char *ssid, char *pass)
{
    storage->net->set_wifi(e, ssid, pass);
}

bool Driver::set_eth(uint8_t e, uint8_t dhcp, uint32_t local_ip, uint32_t gateway, uint32_t subnet)
{
    storage->net->set_eth(e, dhcp, local_ip, gateway, subnet);
}

bool Driver::set_gsm(uint8_t e)
{
    storage->net->set_gsm(e);
}

bool Driver::set_ggc(char *ProjectID, char* Location, char* Registry_id, char* Device_id, char* Private_Key){
    storage->net->set_gg(ProjectID, Location, Registry_id, Device_id, Private_Key);
}

NetIF_Task_Structure Driver::get_netif()
{
    return net->get_netif();
}

bool Driver::update_serial_number(String serial)
{
    storage->sys->update_serial_number(serial);
}
System_Info_Structure Driver::get_sysif()
{
    return storage->sys->get_sysif();
}

bool Driver::API_Send_Message(String api, bool is_swh_system)
{
    bool res = false;

#if API_VERSION == 1
    log_d("api: %s", net->api_ready_frame(sysDriver->j_device(), api).c_str());
#elif API_VERSION == 0

    res = net->API_Send_Message(api, is_swh_system);
    if (res)
    {
        log_d("HTTP Send OK");
    }
    else
    {
        log_d("HTTP Send Failed");
    }
#endif
    jsonapi.clear();
    return res;
}

//=============== DEVICES STORAGE CONFIG ===============
bool Driver::add(Device_Name_Can_Be_Read device, uint8_t id, uint8_t com, uint8_t ss_en, uint16_t SS_Address)
{
    return storage->dev->add(device, id, com, ss_en, SS_Address);
}
bool Driver::add(Device_Name_Can_Be_Read device, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en, uint16_t SS_Address)
{
    return storage->dev->add(device, ID, IP, port, ss_en, SS_Address);
}
bool Driver::add(Device_Name_Can_Be_Read device, char *ID, uint8_t com, uint16_t primary, uint16_t secondary)
{
    return storage->dev->add(device, ID, com, primary, secondary);
}

bool Driver::delRTU(Device_Name_Can_Be_Read device, uint8_t id, uint8_t com, uint8_t ss_en)
{
    return storage->dev->del(device, id, com, ss_en);
}
bool Driver::delTCP(Device_Name_Can_Be_Read device, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en)
{
    return storage->dev->del(device, ID, IP, port, ss_en);
}
bool Driver::delIEC(Device_Name_Can_Be_Read device, char *ID, uint8_t com, uint16_t primary, uint16_t secondary)
{
    return storage->dev->del(device, ID, com, primary, secondary);
}
String Driver::devicesListString()
{
    return storage->dev->toString();
}

//
void Driver::google_mess_recieved_handle(String &topic, String& payload){
    log_d("incoming: %s - %s", String(topic).c_str(), String(payload).c_str());
}
//======================= DRIVER =======================
void Driver::initPinOut()
{
    pinMode(HC595_DATA_PIN, OUTPUT);
    pinMode(HC595_LOAD_PIN, OUTPUT);
    pinMode(HC595_CLOCK_PIN, OUTPUT);
    pinMode(ETHERNET_RESET_PIN, OUTPUT);
    // digitalWrite(ETHERNET_RESET_PIN, HIGH); //----------------------
    adcAttachPin(ADC_VOLT_PIN);
    adcAttachPin(ADC_CURRENT_PIN);
    adcStart(ADC_VOLT_PIN);
    adcStart(ADC_CURRENT_PIN);
    analogReadResolution(12);
}
// DRIVER FUNCTIONS
uint32_t Driver::adc_read_radiation()
{
    uint16_t vol = analogRead(ADC_VOLT_PIN);
    return vol;
    uint32_t result_radiation = map(vol, 0, 4096, 0, 1976);
    if (result_radiation > 300)
    {
        result_radiation += 80;
    }
    return result_radiation;
}
uint32_t Driver::adc_read_temperature()
{
    uint16_t vol = analogRead(ADC_VOLT_PIN);
    return vol;
    uint32_t tem = map(vol, 0, 4096, 0, 330) + 13;
    if (tem < 100)
    {
        tem = map(tem, 0, 100, 0, 10);
    }
    else if (tem >= 100 && tem < 120)
    {
        tem = map(tem, 100, 120, 10, 15);
    }
    else if (tem < 142)
    {
        tem = map(tem, 120, 142, 15, 20);
    }
    else if (tem < 166)
    {
        tem = map(tem, 142, 166, 20, 25);
    }
    else if (tem < 192)
    {
        tem = map(tem, 166, 192, 25, 30);
    }
    else if (tem < 218)
    {
        tem = map(tem, 192, 218, 30, 35);
    }
    else if (tem < 245)
    {
        tem = map(tem, 218, 245, 35, 40);
    }
    else if (tem < 270)
    {
        tem = map(tem, 245, 270, 40, 45);
    }
    else if (tem < 295)
    {
        tem = map(tem, 270, 295, 45, 50);
    }
    else if (tem < 319)
    {
        tem = map(tem, 295, 319, 50, 55);
    }
    else if (tem < 340)
    {
        tem = map(tem, 319, 340, 55, 60);
    }
    else
    {
        tem = 0;
    }
    return tem;
}

void Driver::PrintSegment(uint8_t seg)
{
    if (seg != Last_HC595_Segment)
    {
        Last_HC595_Segment = seg;
        digitalWrite(HC595_LOAD_PIN, HIGH);
        delayMicroseconds(10);
        shiftOut(HC595_DATA_PIN, HC595_CLOCK_PIN, MSBFIRST, seg);
        delayMicroseconds(10);
        digitalWrite(HC595_LOAD_PIN, LOW);
    }
}

// reset ethernet hardware
void Driver::ethernet_reset()
{
    digitalWrite(ETHERNET_RESET_PIN, HIGH);
    delay(2000);
    digitalWrite(ETHERNET_RESET_PIN, LOW);
    bitSet(HC595_Segment, Q4_EN_OSC);
    delay(2000);
}
//
void Driver::ethernet_disable()
{
    digitalWrite(ETHERNET_RESET_PIN, HIGH);
    bitClear(HC595_Segment, Q4_EN_OSC);
}

void Driver::gsm_reset()
{
    bitSet(HC595_Segment, Q2_EN3G);         //  Q2_EN3G         HIGH
    vTaskDelay(100);                        //
    bitClear(HC595_Segment, Q2_EN3G);       //  Q2_EN3G         LOW
    vTaskDelay(100);                        //
    bitSet(HC595_Segment, Q7_PWR_KEY_3G);   //  Q7_PWR_KEY_3G   HIGH
    bitClear(HC595_Segment, Q6_RST_3G);     //  Q6_RST_3G       LOW
    vTaskDelay(100);                        //
    bitClear(HC595_Segment, Q7_PWR_KEY_3G); //  Q7_PWR_KEY_3G   LOW
    vTaskDelay(300);                        //
}

void Driver::WDT_Task(void *arg)
{
    uint32_t wtd_stt = 0;
    uint16_t wtd_time = 0;

    bool led_run_stt = false;
    bool led_con_stt = false;
    uint32_t led_run_time = 0;
    uint32_t led_con_time = 0;
    log_d("WDT START");
    for (;;)
    {
        if (net->network_connected())
        {
            bitSet(HC595_Segment, Q5_LEDRUN);
            led_run_stt = true;
        }
        else
        {
            if (millis() - led_run_time > 500)
            {
                led_run_time = millis();
                led_run_stt = !led_run_stt;

                if (led_run_stt)
                {
                    bitSet(HC595_Segment, Q5_LEDRUN);
                }
                else
                {
                    bitClear(HC595_Segment, Q5_LEDRUN);
                }
            }
        }
        // CON LED
        uint32_t Last_SSOC = net->Last_SSOC();
        if (net->network_connected() && Last_SSOC != 0 && seconds() - Last_SSOC < 1200)
        {
            if (millis() - led_con_time > 500)
            {
                led_con_time = millis();
                led_con_stt = !led_con_stt;

                if (led_con_stt)
                {
                    bitSet(HC595_Segment, Q3_LEDCON);
                }
                else
                {
                    bitClear(HC595_Segment, Q3_LEDCON);
                }
            }
        }
        else
        {
            bitClear(HC595_Segment, Q3_LEDCON);
            led_con_stt = false;
        }
        //WTD
        if (seconds() - wtd_time > 5)
        {
            wtd_time = seconds();
            if (wtd_stt > 0)
            {
                wtd_stt = 0;
                bitClear(HC595_Segment, Q0_WDT);
            }
            else
            {
                wtd_stt = 1;
                bitSet(HC595_Segment, Q0_WDT);
            }
        }

        PrintSegment(HC595_Segment);
        vTaskDelay(10);
    }
}
String Driver::j_get_ambient()
{
    String data = "{";
    data += "\"temp\":" + String(0);
    data += ",\",humidity\":" + String(0);
    data += ",\"dust\":" + String(0);
    data += ",\"radian\":" + String(0);
    data += ",\"altitude\":" + String(0);
    data += ",\"gps\":{\"sensor\":\"\"";
    data += ",\"set\":\"\"}";
    data += "}";
    return data;
}
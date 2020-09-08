#ifndef IEC_62056_21_H
#define IEC_62056_21_H
#include "Arduino.h"
#include "../../../../../type.h"
#include "../../../device_base.h"

/* SIGN ON*/
#define REQUEST_MESSAGE "/?%s!\x0D\x0A"
#define INDENTIFICATION_MESSAGE "/%c%c%c%c\\%c%s\x0D\x0A"

#define ACK_OR_OPTION_SELECT "\x06%c%c%c\x0A"

#define ACK_OPTION_MESSAGE "\x06%c%c%c\x0D\x0A" /* Only used in protocol mode C and E.*/
/* SIGN ON*/

/* ACK MESSAGE*/
#define ACK_MESSAGE "\x06"
#define REPEAT_REQUEST_MESSAGE "\x15"
/* ACK MESSAGE*/

/* DATA MESSAGE */
#define DATA_MESSAGE_except_in_program_mode "\x02%s!\x0D\x0A\x03%c" /* Normal respone of a tariff device, not used in protocol mode E */
#define DATA_MESSAGE_in_program_mode "\x02%s\x03%c"
#define DATA_MESSAGE_in_program_mode_using_optional_partial_blocks "\x02%s\x04%c" /* Only in protocol mode C*/
/* DATA MESSAGE */

/* PROGRAMMING MESSAGE*/
#define PROGRAMMING_COMMAND_MESSAGE "\x01%c%c\x02%s\x03%c"
#define PROGRAMMING_COMMAND_MESSAGE_using_optional_partial_blocks "\x01%c%c\x02\x04%c"
#define ERROR_MESSAGE "\x02%s\x03%c"
#define BREAK_MESSAGE "\x01%s\x03%c"
/* PROGRAMMING MESSAGE*/

#define HHU_R2 "\x01R2\x02%s()\x03%c"
#define HHU_R1 "\x01R1.%s()\x03%c"

typedef enum
{
    Protocol_Error = 0,
    Protocol_Mode_A,
    Protocol_Mode_B,
    Protocol_Mode_C,
    Protocol_Mode_D,
    Protocol_Mode_E
} IEC_62056_21_Protocol_Mode;

typedef struct
{
    String Device_Address;
    char ManufacturerID[4];
    char Identification[17];
    uint8_t baudrate;
    String Command;
    uint8_t status;
    char Data[100];
    char C;
    uint8_t D;
    IEC_62056_21_Protocol_Mode protocol_mode;
    uint8_t annex_mode;
} IEC_62056_21_Structure;

class IEC_62056_21 : public DeviceBase
{
public:
    IEC_62056_21(Device_Info_Structure t, RS485Driver *r) : DeviceBase(r)
    {
        _serial = r;
    }
    ~IEC_62056_21()
    {
    }
    void begin(RS485Driver *r)
    {
        _serial = r;

        // int64_t i64_StartTime = esp_timer_get_time();
        // while((_serial->read() != -1) &&((esp_timer_get_time() - i64_StartTime) < (200 * 1000))){

        // }
        // log_d("---1");
        // setBaudrate(9600);
        // log_d("---2");
        // break_message();
        // log_d("---3");
        // setBaudrate(baudrate[0]);
        // log_d("---4");
        // break_message();
    }

protected:
    RS485Driver *_serial;

    const uint32_t baudrate[7] = {300, 600, 1200, 2400, 4800, 9600, 19200};
    char Device_address[20];

    // virtual float Read_Data(char *c);
    // virtual bool Read_Data(char *c, char *o);
    // virtual IEC_62056_21_Structure ACK(char V, char Z, char Y);
    // virtual IEC_62056_21_Structure Command(char C, char D, char *str);

    // virtual bool Write_Data();
    void setBaudrate(uint32_t br, uint32_t cf = SERIAL_7E1)
    {
        // log_d("Set Baudrate 1");

        _serial->config(br, cf);
        // log_d("Set Baudrate 2");
        int64_t i64_StartTime = esp_timer_get_time();
        while ((_serial->read() != -1) && ((esp_timer_get_time() - i64_StartTime) < (200 * 1000)))
        {
        }
        log_d("Set Baudrate: %d", br);
        vTaskDelay(1);
    }

    IEC_62056_21_Structure Sign_On(String Dev_A)
    {
        // Device_address = Dev_A;
        memset(Device_address, 0, sizeof(Device_address));
        memcpy(Device_address, Dev_A.c_str(), Dev_A.length());
        return Sign_On();
    }

    IEC_62056_21_Structure Sign_On()
    {
        IEC_62056_21_Structure res;
        setBaudrate(baudrate[0]);
        send_Request(Device_address);
        res = check_Sign_On();
        return res;
    }

    String receive(uint32_t timeout, String r1)
    {
        String str = "";
        int64_t i64_StartTime = esp_timer_get_time();
        int64_t i64_CycleTime = esp_timer_get_time();
        while ((esp_timer_get_time() - i64_StartTime) < (timeout * 1000))
        {
            while (_serial->available() > 0)
            {
                char d = _serial->read();
                // Serial.print(d,HEX);Serial.print(" ");
                // Serial.print(d);
                str += d;
                if (str.endsWith(r1))
                {

                    return str;
                }
            }
            if ((esp_timer_get_time() - i64_CycleTime) < (10000))
            {
                vTaskDelay(1);
                i64_CycleTime = esp_timer_get_time();
            }
        }
        str = "";
        return str;
    }

    uint8_t CheckBBC(uint8_t *in)
    {
        uint8_t i, result;
        uint8_t stt;
        //Serial.write("BBC:");
        for (i = 0, stt = 0, result = 0; i < 100; i++)
        {
            //Serial.print((char)in[i],HEX);Serial.write(" ");
            switch (stt)
            {
            case 0:
            {
                if (in[i] == 0x01 || in[i] == 0x02)
                {
                    stt = 1;
                }
            }
            break;
            case 1:
            {
                result = result ^ in[i];
                if (in[i] == 0x03)
                {
                    //            Serial.println("");
                    return result;
                }
            }
            break;
            }
        }
        return 0;
    }

    bool CheckBBC(uint8_t bbc, String str)
    {
        uint8_t s[str.length() + 1];
        str.toCharArray((char *)s, str.length() + 1);
        if (bbc == CheckBBC(s))
        {
            return 1;
        }
        return 0;
    }

    bool CheckBBC(String str)
    {
        uint8_t bbc = str.charAt(str.length() - 1);
        return CheckBBC(bbc, str);
    }

    void send(char *d, bool en_bbc = 0)
    {
        if (_serial->de_pin > 0)
        {
            _serial->preTransmission();
        }
        if (en_bbc == 1)
        {
            uint8_t bbc = CheckBBC((uint8_t *)d);
            d[String(d).length() - 1] = bbc;
            d[String(d).length()] = 0;
        }
        int64_t i64_StartTime = esp_timer_get_time();
        while ((_serial->read() != -1) && ((esp_timer_get_time() - i64_StartTime) < (200 * 1000)))
        {
        }
        _serial->write(d);
        // Serial.write(d);
        _serial->flush();
        if (_serial->de_pin > 0)
        {
            _serial->postTransmission();
        }
    }

    void send_ACK(char V, char Z, char Y)
    {
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, ACK_OR_OPTION_SELECT, V, Z, Y);
        send(req_mes, 0);
    }
    void send_Program_Command_Message(char C, char D, char *DataSet)
    {
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, PROGRAMMING_COMMAND_MESSAGE, C, D, DataSet);
        send(req_mes, 1);
    }

protected:
    void send_HHU_R1(String ID)
    {
        delay(50);
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, HHU_R1, ID.c_str());
        send(req_mes, 1);
        log_v("Send: %s", ID.c_str());
        delay(10);
    }

    String receive_HHU_R1()
    {
        String res;
        String str = receive(2000, "\x03");
        uint8_t bbc = _serial->read();
        // Serial.write(bbc);
        log_v("Receive: %s", str.c_str());
        if (CheckBBC(bbc, str))
        {

            res = str.substring(str.indexOf('(') + 1, str.lastIndexOf(')'));

            // res.C = str.charAt(1);
            // res.D = str.substring(2, 3).toInt();
            // str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).toCharArray(res.Data_Set, str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).length() + 1);
            // res.Data_Set = str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03'));
        }
        else
        {
            res = "";
        }
        return res;
    }

    int iget_HHU_R1(String ID)
    {
        int res;

        send_HHU_R1(ID);
        String str = receive_HHU_R1();
        if (str.indexOf("ER:NOT SUPPORT") > 0)
        {
            return 0;
        }
        res = str.substring(str.indexOf('(') + 1, str.indexOf('*')).toInt();
        return res;
    }

    float fget_HHU_R1(String ID)
    {
        float res;

        send_HHU_R1(ID);
        String str = receive_HHU_R1();
        if (str.indexOf("ER:NOT SUPPORT") > 0)
        {
            return 0;
        }
        res = str.substring(str.indexOf('(') + 1, str.indexOf('*')).toFloat();
        return res;
    }

    String pget_HHU_R1(String ID, char *o, size_t size = 20)
    {

        send_HHU_R1(ID);
        String str = receive_HHU_R1();
        if (str.indexOf("ER:NOT SUPPORT") > 0)
        {
            log_d("ER:NOT SUPPORT");
            return String("");
        }
        memset(o, 0, size);
        // str =  str.substring(str.indexOf('(')+1, str.indexOf(')'));
        // log_d("--------->: %s", str.c_str());
        // memcpy(o, str.c_str(), str.length());
        str.substring(str.indexOf('(') + 1, str.indexOf(')')).toCharArray(o, str.substring(str.indexOf('(') + 1, str.indexOf(')')).length() + 1);
        return String(o);
    }

    void send_HHU_R2(String ID)
    {
        delay(50);
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, HHU_R2, ID.c_str());
        send(req_mes, 1);
        log_v("Send: %s", ID.c_str());
        delay(10);
    }

    String receive_HHU_R2()
    {
        String res;
        String str = receive(2000, "\x03");
        uint8_t bbc = _serial->read();
        // Serial.write(bbc);
        // if(CheckBBC(bbc, str))
        // {
        // log_d("Receive: %s", str.c_str());
        if (str.indexOf("ER:NOT SUPPORT") > 0)
        {
            return "";
        }
        res = str.substring(str.indexOf('(') + 1, str.lastIndexOf(')'));
        // log_d("res->: %s", res.c_str());

        // res.C = str.charAt(1);
        // res.D = str.substring(2, 3).toInt();
        // str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).toCharArray(res.Data_Set, str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).length() + 1);
        // res.Data_Set = str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03'));
        // }
        // else
        // {
        //     res = "";
        // }
        return res;
    }

    void break_message()
    {
        delay(50);
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, BREAK_MESSAGE, "B0");
        send(req_mes, 1);
        log_v("Break Message");
        delay(10);
    }

    String printdata(const char *key, float in, bool dot = 1)
    {
        String res = "";
        if (in == 0)
        {
            // log_d("Debug: %s: %f",key,in);
            // return String("");
            res += String(",") + String(key) + String(":") + String("0.00");
            return res;
        }
        if (dot == 1)
        {
            res += ",";
        }
        if ((!isnan(in)) || (!isinf(in)))
        {
            res += String(key) + String(":") + String(in);
        }
        else
        {
            res += String(key) + String(":0.00");
        }
        return res;
    }

    String printdata(const char *key, char *in, bool dot = 1)
    {
        String res = "";
        if (String(in) == "")
        {
            // return String("");
            res += String(",") + String(key) + String(":\"\"");
            return res;
        }
        if (dot == 1)
        {
            res += ",";
        }

        res += String(key) + String(":\"") + String(in) + String("\"");
        return res;
    }

    String printdata(const char *key, int in, bool dot = 1)
    {
        String res = "";
        if (in == 0)
        {
            // return String("");
            res += String(",") + String(key) + String(":") + String("0");
            return res;
        }
        if (dot == 1)
        {
            res += ",";
        }
        res += String(key) + String(":") + String(in);
        return res;
    }

private:
    void send_Request(String Device_address)
    {
        char req_mes[50];
        memset(req_mes, 0, 50);
        Device_address.toCharArray(req_mes, Device_address.length() + 1);
        send_Request((const char *)req_mes);
    }

    void send_Request(const char *Device_address)
    {
        char req_mes[50];
        memset(req_mes, 0, 50);
        sprintf(req_mes, REQUEST_MESSAGE, Device_address);
        send(req_mes, 0);
    }

    IEC_62056_21_Structure check_Sign_On()
    {
        IEC_62056_21_Structure res;
        String str = "";
        uint8_t stt = 0;
        uint8_t c_count = 0;
        int64_t i64_StartTime = esp_timer_get_time();
        int64_t i64_CycleTime = esp_timer_get_time();
        bool is_mode_A = false;
        bool is_mode_E = false;
        res.status = false;
        res.protocol_mode = Protocol_Error;
        res.annex_mode = 0;
        while ((esp_timer_get_time() - i64_StartTime) < (1000000))
        {
            while (_serial->available() > 0)
            {
                char d = _serial->read();

                str += d;
                // log_d("case %d: %c", stt, d);
                switch (stt)
                {
                case 0:
                {

                    if (d == '/')
                    {
                        stt++;
                    }
                }
                break;
                case 1:
                {

                    res.ManufacturerID[c_count++] = d;
                    if (c_count == 3)
                    {
                        stt++;
                        c_count = 0;
                    }
                }
                break;
                case 2:
                {
                    if (d >= '0' && d <= '9')
                    {
                        // log_d("-----------Baud: %d", (d));
                        res.baudrate = d - '0';
                        stt++;
                    }
                    else
                    {
                        res.status = 0;
                        return res;
                    }
                }
                break;
                case 3:
                {
                    if (d == '\\' && c_count == 0)
                    {
                        stt = 4;
                    }
                    else if (d >= 'A' && d <= 'z')
                    {
                        res.Identification[c_count++] = d;
                        stt = 5;
                    }
                    else if (d == 0x0D)
                    {
                        stt = 6;
                    }
                    else
                    {
                    }
                }
                break;
                case 4:
                {
                    if (d >= '0' && d <= '9')
                    {
                        res.annex_mode = d - '0';
                        stt = 5;
                        c_count = 0;
                        is_mode_E = true;
                    }
                    else
                    {
                        res.status = 0;
                        return res;
                    }
                }
                break;
                case 5:
                {
                    if ((d >= '*' && d <= '9') || (d >= 'A' && d <= 'z'))
                    {
                        res.Identification[c_count++] = d;
                        if (c_count > 16)
                        {
                            res.status = 0;
                            c_count = 0;
                            return res;
                        }
                    }
                    else if (d == 0x0D)
                    {
                        c_count = 0;
                        stt = 6;
                    }
                    else
                    {
                    }
                }
                break;
                case 6:
                {
                    if (d == 0x0A)
                    {
                        stt = 7;
                    }
                }
                break;
                case 7:
                {
                    if (d == 0x02)
                    {
                        stt = 9;
                        is_mode_A = true;
                    }
                    else if (d == 0x0D)
                    {
                        stt = 8;
                    }
                }
                break;
                case 8:
                {
                    if (d == 0x0A)
                    {
                        stt = 9;
                    }
                }
                break;
                case 9:
                {
                    if ((d >= '(' && d <= '9') || (d >= 'A' && d <= 'z'))
                    {
                        res.Data[c_count++] = d;
                    }
                    else if (d == '!')
                    {
                        stt = 10;
                    }
                }
                break;
                case 10:
                {
                    if (d == 0x0D)
                    {
                        stt = 11;
                    }
                }
                break;
                case 11:
                {
                    if (d = 0x0A)
                    {
                        if (is_mode_A == true)
                        {
                            stt = 12;
                        }
                        else
                        {
                            res.protocol_mode = Protocol_Mode_D;
                            res.status = true;
                            return res;
                        }
                    }
                }
                break;
                case 12:
                {
                    if (d == 0x03)
                    {
                        stt = 13;
                    }
                    else
                    {
                        res.status = 0;
                        return res;
                    }
                }
                break;
                case 13:
                {
                    if (CheckBBC(str))
                    {
                        res.status = 0;
                        res.protocol_mode = Protocol_Mode_A;
                        return res;
                    }
                }
                break;
                }
            }
            if ((esp_timer_get_time() - i64_CycleTime) < (10000))
            {
                vTaskDelay(1);
                i64_CycleTime = esp_timer_get_time();
            }
        }
        log_d("Stt %d:,status: %d", stt, res.status);
        if (stt == 7)
        {
            res.status = true;
            if (res.annex_mode > 0)
            {
                res.protocol_mode = Protocol_Mode_E;
            }
            else
            {
                res.protocol_mode = Protocol_Mode_A;
            }
            return res;
        }
        else
        {
            return res;
        }
    }
};

#endif

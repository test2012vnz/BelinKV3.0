#ifndef _RS485_H_
#define _RS485_H_

#include <HardwareSerial.h>

class RS485Driver : public HardwareSerial
{

public:
    RS485Driver(uint8_t ser_num, uint8_t rx = 35, uint8_t tx = 32, int de = -1) : HardwareSerial(ser_num)
    {
        rx_pin = rx;
        tx_pin = tx;
        de_pin = de;
        begin(9600, SERIAL_8N1, rx_pin, tx_pin);
        pinMode(de_pin, OUTPUT);
    }
    ~RS485Driver()
    {
    }

    void config(uint32_t b, uint32_t cf)
    {
        if (baudrate != b || conf != cf)
        {
            baudrate = b;
            conf = cf;
            updateBaudRate(baudrate, conf);
        }
    }
    void preTransmission()
    {
        digitalWrite(de_pin, HIGH);
        vTaskDelay(10);
    }

    void postTransmission()
    {
        vTaskDelay(50);
        digitalWrite(de_pin, LOW);
    }
    int de_pin;

private:
    uint32_t baudrate = 9600;
    uint32_t conf = SERIAL_8N1;
    uint8_t rx_pin;
    uint8_t tx_pin;
};

struct nodeRS485
{
    uint8_t id;
    RS485Driver *rs;
    struct nodeRS485 *pNext;
};
typedef struct nodeRS485 NODERS485;

struct listRS485
{
    int capacity;
    NODERS485 *pHead;
};
typedef struct listRS485 LISTRS485;

class RS485PORT
{

public:
    RS485PORT(/* args */)
    {
        list.capacity = 0;
        list.pHead = nullptr;
    }
    ~RS485PORT()
    {
    }

    bool isEmpty()
    {
        return list.pHead == nullptr;
    }

    void push(NODERS485 *p)
    {
        list.capacity++;
        if (list.pHead == nullptr)
        {
            list.pHead = p;
        }
        else
        {
            p->pNext = list.pHead;
            list.pHead = p;
        }
    }

    void push(int portID, uint8_t serial_num, uint8_t rx = 35, uint8_t tx = 32, int de = -1)
    {
        RS485Driver *rs = new RS485Driver(serial_num, rx, tx, de);
        NODERS485 *node = new NODERS485;
        node->id = portID;
        node->rs = rs;
        node->pNext = nullptr;
        push(node);
        info();
    }

    uint8_t pop(int id)
    {
        NODERS485 *p = list.pHead;
        NODERS485 *pl;
        if (isEmpty())
        {
            return 1;
        }
        while (p != nullptr)
        {

            pl = p;
            p = p->pNext;
            if (p->id == id)
            {
                list.capacity--;
                Serial.println("id:" + String(pl->id));
                pl->pNext = p->pNext;
                delete p;
                return 1;
            }
        }
        return 0;
    }
    RS485Driver *getPortDriver(int portID)
    {
        NODERS485 *p = list.pHead;
        while (p != nullptr)
        {
            if (p->id = portID)
            {
                return p->rs;
            }
            p = p->pNext;
        }
        return nullptr;
    }
    void info()
    {
        log_d("------ RS485 List Port ------");
        NODERS485 *p = list.pHead;
        while (p != nullptr)
        {
            log_d("-PortID: %d", p->id);
            p = p->pNext;
        }
    }

private:
    LISTRS485 list;
};

#endif
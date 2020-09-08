#ifndef _DEVICE_BASE_H_
#define _DEVICE_BASE_H_

#include "../../type.h"
#include "../../driver/driver.h"
#include "modbus/RTU/ModbusRTU.h"
#include "modbus/TCP/ModbusTCP.h"

#ifdef __cplusplus
extern "C"
{
#endif

    uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

class DeviceBase
{
public:
    DeviceBase(HardwareSerial *rs)
    {
        if (mIP == nullptr)
        {
            mIP = new ModbusIP();
        }
        rtu = new ModbusRTU(rs);
    }
    ~DeviceBase()
    {
        if (del != nullptr)
        {
            del();
        }

        del = nullptr;
        delete rtu;
        rtu = nullptr;
    }

    virtual bool load()
    {
    }
    virtual String toString()
    {
    }
    // device func
    Device_Info_Structure devInfo(){
        log_d("%d", (int )type.type);
        return type;
    }
    Device_Communitcation_Type connectionType()
    {
        return type.type;
    }
    Device_Name_Can_Be_Read devType()
    {
        return type.dev;
    }
    // API header
    String printLocal(); // use for new API version
    String APIHeader(bool is_swh_system = false);

protected:
    Device_Info_Structure type;

    ModbusRTU *rtu;
    static ModbusIP *mIP;

    // variable indentify
    int id;
    void (*del)();

    // ul
    uint16_t to_u16(uint16_t i)
    {
        return __bswap_16(i);
    }
    int16_t to_i16(int16_t i)
    {
        return (int16_t)__bswap_16((uint16_t)i);
    }
    uint32_t to_u32(uint32_t *i)
    {
        return __bswap_32(*(uint32_t *)i);
    }
    float to_float(uint16_t i, uint16_t div = 1)
    {
        return __bswap_16(i) / (float)div;
    }
    float to_float(int16_t i, uint16_t div = 1)
    {
        return to_i16(i) / (float)div;
    }
    float to_float(uint32_t *i, uint16_t div = 1)
    {
        return __bswap_32(*(uint32_t *)i) / (float)div;
    }
private:
};

struct DeviceBase_Node
{
    DeviceBase *data;
    DeviceBase_Node *pNext;
};

struct DeviceBase_List
{
    DeviceBase_Node *pHead;
    DeviceBase_Node *pTail;
};

class DeviceBase_LinkedList
{

public:
    DeviceBase_LinkedList()
    {
        list.pHead = nullptr;
        list.pTail = nullptr;
    }
    ~DeviceBase_LinkedList()
    {
    }
    DeviceBase_Node *add(DeviceBase *d)
    {
        DeviceBase_Node *p = createNode(d);
        if (list.pHead == nullptr)
        {
            list.pHead = p;
            list.pHead->pNext = list.pTail;
            list.pTail = p;
        }
        else
        {
            list.pTail->pNext = p;
            list.pTail = p;
        }
        return p;
    }
    void delete_all()
    {
        DeviceBase_Node *p = list.pHead;
        DeviceBase_Node *pre = nullptr;
        while (p != nullptr)
        {
            pre = p;
            p = p->pNext;
            if (pre->data != nullptr)
            {
                delete pre->data;
            }
            delete pre;
        }
        list.pHead = list.pTail = nullptr;
    }
    DeviceBase_Node *pHead()
    {
        return list.pHead;
    }

private:
    DeviceBase_List list;
    DeviceBase_Node *createNode(DeviceBase *d)
    {
        DeviceBase_Node *p = new DeviceBase_Node;
        p->data = d;
        p->pNext = nullptr;
        return p;
    }
};

#endif
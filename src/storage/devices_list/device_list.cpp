#include "device_list.h"

DeviceInfo DeviceList::info = {};
String DeviceList::name_partition = {};
void (*DeviceList::sync_fnc)(Device_Info_Structure *, int) = nullptr;
void (*DeviceList::flag_sync)(bool) = nullptr;

void DeviceList::begin()
{
    sync(false);
    // print();
}

bool DeviceList::commit()
{
    size_t len = NVS.putBytes(name_partition.c_str(), &info, sizeof(DeviceInfo));
    if (!(len % sizeof(DeviceInfo)))
    {
        return sync(true);
    }
    return false;
}

bool DeviceList::sync(bool f_sync)
{
    size_t len = NVS.getBytesLength(name_partition.c_str());
    log_d("---dev len: %d %d", len, sizeof(DeviceInfo));

    if( len == sizeof(DeviceInfo)){
        NVS.getBytes(name_partition.c_str(), &info, len);
        if (flag_sync != nullptr)
        {
            flag_sync(f_sync);
        }
        return true;
    }else if(len == 0 || len != sizeof(DeviceInfo) || info.status != 0x0A){
        memset(&info, 0, sizeof(DeviceInfo));
        info.status = 0x0A;
        info.size = 0;
        return commit();
    }
}

int DeviceList::exist(Device_Info_Structure in)
{
    log_d("--- del size: %d", info.size);
    for (int i = 0; i < info.size; i++)
    {
        if (info.info[i] == in)
        {
            return i;
        }
    }
    return -1;
}

bool DeviceList::add(Device_Info_Structure i)
{
    if (info.size >= DEVICES_MAXIMUM - 1)
    {
        log_e("Device List is Full!!!");
        return false;
    }
    if (exist(i) >= 0)
    {
        log_e("Device is Exist!!!");
        return false;
    }

    info.info[info.size++] = i;
    return commit();
}

// add RTU
bool DeviceList::add(Device_Name_Can_Be_Read dev, Device_Modbus_RTU_Struture d, uint8_t ss_en, uint16_t SS_Address)
{
    Device_Info_Structure i;
    i.dev = dev;
    if (ss_en == 1)
    {
        i.type = Device_Modbus_RTU_SunSpec;
    }
    else
    {
        i.type = Device_Modbus_RTU;
    }
    i.SunSpec_Address = SS_Address;
    i.info.RTU = d;
    return add(i);
}
bool DeviceList::add(Device_Name_Can_Be_Read dev, uint8_t id, uint8_t com, uint8_t ss_en, uint16_t SS_Address)
{
    Device_Modbus_RTU_Struture i;
    i.ID = id;
    i.com = com;
    return add(dev, i, ss_en, SS_Address);
}
// add TCP
bool DeviceList::add(Device_Name_Can_Be_Read dev, Device_Modbus_TCP_Struture d, uint8_t ss_en, uint16_t SS_Address)
{
    Device_Info_Structure i;
    i.dev = dev;
    if (ss_en == 1)
    {
        i.type = Device_Modbus_TCP_SunSpec;
    }
    else
    {
        i.type = Device_Modbus_TCP;
    }
    i.SunSpec_Address = SS_Address;
    i.info.TCP = d;
    return add(i);
}
bool DeviceList::add(Device_Name_Can_Be_Read dev, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en, uint16_t SS_Address)
{
    log_d("%d %s %d %d %d", ID, IPAddress(IP).toString().c_str(), port, ss_en, SS_Address);
    Device_Modbus_TCP_Struture i;
    i.ID = ID;
    i.IP = IP;
    i.port = port;
    return add(dev, i, ss_en, SS_Address);
}
// add IEC

bool DeviceList::add(Device_Name_Can_Be_Read dev, Device_IEC_Structure d)
{
    Device_Info_Structure i;
    i.dev = dev;
    i.type = Device_IEC62056;
    i.info.IEC = d;
    return add(i);
}
bool DeviceList::add(Device_Name_Can_Be_Read dev, char *ID, uint8_t com, uint16_t primary, uint16_t secondary)
{
    Device_IEC_Structure i;
    int length = String(ID).length();
    if (length >= 20)
    {
        log_e("ID is so long character...");
        return false;
    }
    memset(i.ID, 0, sizeof(char) * 20);
    memcpy(i.ID, ID, (length) * sizeof(char));
    i.com = com;
    i.primary = primary;
    i.secondary = secondary;
    return add(dev, i);
}

// del
bool DeviceList::deleteAll()
{
    info.size = 0;
    return commit();
}
bool DeviceList::del(Device_Info_Structure i)
{
    int c = exist(i);
    if (c < 0)
    {
        log_e("Device is not Exist!!!");
        return false;
    }
    log_d("stt: %d-%d", c, info.size);
    log_d("p1: %p- %p", &info.info[c], &info.info[c + 1]);
    memcpy(&info.info[c].dev, &info.info[c + 1].dev, (info.size - c) * sizeof(Device_Info_Structure));
    // memcpy(&info.info[c],&info.info[info.size-1],sizeof(Device_Info_Structure));
    // info.info[c]=info.info[info.size-1];
    info.size--;

    return commit();
}
// delete RTU
bool DeviceList::del(Device_Name_Can_Be_Read dev, Device_Modbus_RTU_Struture d, uint8_t ss_en)
{
    Device_Info_Structure i;
    i.dev = dev;
    if (ss_en == 1)
    {
        i.type = Device_Modbus_RTU_SunSpec;
    }
    else
    {
        i.type = Device_Modbus_RTU;
    }

    i.info.RTU = d;
    return del(i);
}
bool DeviceList::del(Device_Name_Can_Be_Read dev, uint8_t id, uint8_t com, uint8_t ss_en)
{
    Device_Modbus_RTU_Struture i;
    i.ID = id;
    i.com = com;
    log_d("dev: %s, id: %d", DeviceLISTNAME[dev], id);
    return del(dev, i, ss_en);
}
bool DeviceList::del(Device_Name_Can_Be_Read dev, Device_Modbus_TCP_Struture d, uint8_t ss_en)
{
    Device_Info_Structure i;
    i.dev = dev;
    if (ss_en == 1)
    {
        i.type = Device_Modbus_TCP_SunSpec;
    }
    else
    {
        i.type = Device_Modbus_TCP;
    }

    i.info.TCP = d;
    return del(i);
}
bool DeviceList::del(Device_Name_Can_Be_Read dev, uint8_t ID, uint32_t IP, int32_t port, uint8_t ss_en)
{
    Device_Modbus_TCP_Struture i;
    i.ID = ID;
    i.IP = IP;
    i.port = port;
    return del(dev, i, ss_en);
}
// delete IEC
bool DeviceList::del(Device_Name_Can_Be_Read dev, Device_IEC_Structure d)
{
    Device_Info_Structure i;
    i.dev = dev;
    i.type = Device_IEC62056;
    i.info.IEC = d;
    return del(i);
}
bool DeviceList::del(Device_Name_Can_Be_Read dev, char *ID, uint8_t com,  uint16_t primary, uint16_t secondary)
{
    Device_IEC_Structure i;
    int length = String(ID).length();
    if (length >= 20)
    {
        log_e("ID is so long character...");
        return false;
    }
    memset(i.ID, 0, sizeof(char) * 20);
    memcpy(i.ID, ID, (length) * sizeof(char));
    i.com = com;
    i.primary = primary;
    i.secondary = secondary;
    return del(dev, i);
}

// To String
String DeviceList::toString(Device_Info_Structure *g, int x)
{
    String s = "[";
    for (int i = 0; i < x; i++)
    {
        String ele = "[";
        ele += String(g[i].type) + String(",");
        ele += String(g[i].dev) + String(",");

        switch ((int)g[i].type)
        {
        case Device_Modbus_RTU:
        case Device_Modbus_RTU_SunSpec:
        {
            ele += String(g[i].info.RTU.ID) + String(",");
            ele += "0,0";
        }
        break;
        case Device_Modbus_TCP:
        case Device_Modbus_TCP_SunSpec:
        {
            ele += String(g[i].info.TCP.ID) + String(",");
            ele += String(g[i].info.TCP.port) + String(",");
            ele += String(__bswap_32(g[i].info.TCP.IP));
        }
        break;
        case Device_IEC62056:
        {
            ele += "\"" + String(g[i].info.IEC.ID) + String("\",");
            ele += "0,0,";
            ele += String(g[i].info.IEC.primary) + String(",");
            ele += String(g[i].info.IEC.secondary);
        }
        break;
        }
        ele += "]";
        s += ele + String(",");
    }
    if (x > 0)
    {
        s = s.substring(0, s.length() - 1);
    }
    s += "]";
    return s;
}
String DeviceList::toString()
{
    return toString(info.info, info.size);
}

int DeviceList::size()
{
    return info.size;
}
String DeviceList::print()
{
    log_d("-------------------------------------");
    for (int i = 0; i < info.size; i++)
    {
        print(info.info[i]);
    }
    log_d("-------------------------------------");
    return "";
}

void DeviceList::print(Device_Info_Structure d)
{
    log_d("----\\");
    switch ((int)d.type)
    {

    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        if (d.type == Device_Modbus_RTU)
        {
            log_d("Device_Modbus_RTU | dev: %d -Com: %d", (int)d.dev, d.info.RTU.com);
        }
        else
        {
            log_d("Device_Modbus_RTU_SunSpec| dev: %d -Com: %d", (int)d.dev, d.info.RTU.com);
        }
        log_d("-mode: %d -ID %d", d.mode, d.info.RTU.ID);
    }
    break;
    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        if (d.type == Device_Modbus_TCP)
        {
            log_d("Device_Modbus_TCP| dev: %d", (int)d.dev);
        }
        else
        {
            log_d("Device_Modbus_TCP_SunSpec");
        }
        log_d("-mode: %d -ID: %d -IP: %s -PORT: %d", d.mode, d.info.TCP.ID, IPAddress(d.info.TCP.IP).toString().c_str(), d.info.TCP.port);
    }
    break;
    case Device_IEC62056:
    {
        log_d("Device_IEC62056| dev: %d", (int)d.dev);
        log_d("-ID: %s -Pri: %d -Sec: %d -Com: %d ", String(d.info.IEC.ID).c_str(), d.info.IEC.primary, d.info.IEC.secondary, d.info.IEC.com);
    }
    break;
    }
}
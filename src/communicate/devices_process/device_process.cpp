#include "device_process.h"

bool DeviceProcess::syncPoll = false;
DeviceBase_LinkedList DeviceProcess::list;
Device_Modbus_Mode DeviceProcess::RS485_Mode = Modbus_Master;
uint8_t DeviceProcess::deviceSendFailed = 0;
uint8_t DeviceProcess::deviceReadOK = 0;
uint8_t DeviceProcess::size = 0;
uint32_t DeviceProcess::last_routine = 0;

bool (*DeviceProcess::force_storage_sync)() = nullptr;

void DeviceProcess::sync_from_storage(Device_Info_Structure *in, int x)
{
    size = x;
    log_d("--------sync_size %d", size);
    RS485_Mode = Modbus_Master;
    list.delete_all();
    for (int i = 0; i < size; i++)
    {
        if (in[i].type == Device_Modbus_RTU_SunSpec || in[i].type == Device_Modbus_TCP_SunSpec)
        {
            list.add(new SunSpec(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
        }
        else
        {
            switch ((int)in[i].dev)
            {
            case Vinasino_VSE3T_5B:
            {
                list.add(new Vinasino_VSE3T(in[i], Driver::getPortDriver(in[i].info.IEC.com)));
            }
            break;
            case Micro_DPM380:
            {
                // list.add(new DPM(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
                //EPSolarTracer
                list.add(new EPSolarTracer(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case Selec_MFM_383:
            {
                list.add(new MFM(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;

            case SolarBK_Grid_Tie:
            {
                list.add(new Sofar(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;

            case SolarBK_Hybrid:
            {
                list.add(new Hybrid(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;

            case SMA_SolidQ:
            {
                list.add(new SolidQ50(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;

            case Sungrow_Grid_Tie:
            {
                list.add(new Sungrow(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;

            case Kipp_and_Zonen_Solar_Radiation:
            {
                list.add(new KippZonen(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case SWH_System:
            {
                list.add(new SWH(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case Huawei_Grid_Tie:
            {
                list.add(new HuaweiV2(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case Huawei_Grid_Tie_V3:
            {
                list.add(new HuaweiV3(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case SMA_RTU:
            {
                list.add(new SMARTUClass(in[i], Driver::getPortDriver(in[i].info.RTU.com)));
            }
            break;
            case SMA_TCP:
            {
                list.add(new SMARTUClass(in[i], Driver::getPortDriver(in[i].info.RTU.com), true));
            }
            break;
            }
        }
    }
}

void DeviceProcess::load()
{
    uint8_t cnt = 0;
    DeviceBase_Node *p = list.pHead();
    String stt = "[";
#if API_VERSION == 1
    String api = "";
    uint8_t i = 0;
    while (p != nullptr)
    {
        if (RS485_Mode == Modbus_Master || p->data->devType() == SWH_System)
        {
            // Change to type ETH if device using ETH
            if (p->data->connectionType() == Device_Modbus_TCP ||
                p->data->connectionType() == Device_Modbus_TCP_SunSpec)
            {
                Driver::net->setDefaultIf(TCPIP_ADAPTER_IF_ETH);
            }
            //Device Read Data
            if (p->data->load())
            {
                stt += "1";
                cnt++;
            }
            else
            {
                stt += "0";
            }
            if (i != 0)
            {
                api += ",";
                stt += ",";
            }

            api += p->data->toString();
        }
        p = p->pNext;
        delay(100);
    }
    // Send API
    Driver::API_Send_Message(api, false);

#elif API_VERSION == 0
    uint8_t i = 0;
    while (p != nullptr)
    {
        if (i != 0)
        {
            stt += ",";
        }
        if (RS485_Mode == Modbus_Master || p->data->devType() == SWH_System)
        {
            // Change to type ETH if device using ETH
            if (p->data->connectionType() == Device_Modbus_TCP ||
                p->data->connectionType() == Device_Modbus_TCP_SunSpec)
            {
                Driver::net->setDefaultIf(TCPIP_ADAPTER_IF_ETH);
            }
            //Device Read Data
            if (p->data->load())
            {
                stt += "1";
                cnt++;
            }
            else
            {
                stt += "0";
            }

            String s = p->data->toString();
            log_d("load: %s", s.c_str());
            // Driver::sd->write_log(s);
            //Check SWH System to Send API
            bool is_swh_system = (p->data->devType() == SWH_System) ? true : false;
            if (Driver::API_Send_Message(s, is_swh_system))
            {
                deviceSendFailed = 0;
            }
            else
            {
                deviceSendFailed++;
            }
        }
        i++;
        p = p->pNext;
        delay(100);
    }
#endif
    stt += "]";
    p = nullptr;
    delete p;
    deviceReadOK = cnt;
    last_routine = seconds();

    if (deviceSendFailed > 100)
    {
        log_d("---- ESP RESTART ---- send fault");
        ESP.restart();
    }
}

String DeviceProcess::devices_status()
{
}
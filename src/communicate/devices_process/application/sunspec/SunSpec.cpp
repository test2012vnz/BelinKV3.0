#include "Sunspec.h"
int SunSpec::id_count = 1;

bool SunSpec::load()
{
    status = false;
    deleteList();
    if (SunSpecCheck(type.SunSpec_Address) == 0)
    {
    }
    else
    {
        SunSpecLoad(type.SunSpec_Address + 2);
    }

    // log_d("%s", getString().c_str());
    // getString();
    //TODO: disconnect of TCP socket for SMA
    if (type.type == Device_Modbus_TCP_SunSpec ||
        type.type == Device_Modbus_TCP)
    {
        tcp_disconect();
    }
    log_d("sunspec detect %d", ESP.getFreeHeap());
    return status;
}

String SunSpec::toString()
{
    String s = "";
#if API_VERSION == 1
    
#elif API_VERSION == 0
    s += APIHeader();
    s += ",Sunspec_List:[{";
    s += "InvID:" + String(id);
    if (status == false)
    {
        s += ",Status:0";
    }
    else
    {
        s += ",Status:1,";
        s += getString();
    }
    s += "}]}";

#endif
    return s;
}
bool SunSpec::SunSpecCheck(uint16_t address)
{
    uint16_t d[2] = {};
    if (readHreg(address, (uint16_t *)d, 2) == 0)
    {
        uint32_t ss_check = *(uint32_t *)d;
        if (ss_check == 0x536e7553) // 0x536e7553 =  Suns
        {
            status = true;
            return status;
        }
    }
    log_d("sunspec not detect");
    status = false;
    return status;
}

void SunSpec::SunSpecLoad(SunSpec_Repeating_Structure **u, uint16_t address, uint16_t ssid, uint16_t N)
{
    if (N == 0)
    {
        return;
    }
    switch (ssid)
    {
    case 160:
    {
        SunSpec_Repeating_Union datain = {};
        if (readHreg(address, (uint16_t *)&datain, 20) == 0)
        {
            *u = addNode(u, datain);
            SunSpecLoad(u, address + 20, ssid, N - 1);
        }
    }
    break;
    case 401:
    {
        SunSpec_Repeating_Union datain = {};
        if (readHreg(address, (uint16_t *)&datain, 8) == 0)
        {
            *u = addNode(u, datain);
            // log_d("----- %d", *(&(*u)->u401.inID));
            SunSpecLoad(u, address + 8, ssid, N - 1);
        }
    }
    break;
    }
}

void SunSpec::SunSpecLoad(uint16_t address)
{
    if (d != nullptr)
    {
        free(d);
        d = nullptr;
    }
    d = (uint16_t *)calloc(2, sizeof(uint16_t));
    if (readHreg(address, (uint16_t *)d, 2) == 0)
    {
        uint16_t ssid = *(uint16_t *)d;
        ssid = __bswap_16(ssid);
        uint16_t length = *(uint16_t *)(d + 2);
        length = __bswap_16(length) + 2;
        if (ssid == 0xFFFF)
        {
            free(d);
            d = nullptr;
            return;
        }
        switch (ssid)
        {
        case 1:
        case 103:
        case 102:
        case 101:
        case 111:
        case 112:
        case 113:
        {
            scanList += String(ssid) + String(",");
            SunSpec_Data_Structure_Union datain = {};
            if (readHreg(address, (uint16_t *)&datain, length) == 0)
            {
                addNode(datain, ssid);
            }
            SunSpecLoad(address + length);
        }
        break;
        case 160:
        {
            scanList += String(ssid) + String(",");
            SunSpec_Data_Structure_Union datain = {};
            if (readHreg(address, (uint16_t *)&datain, 10) == 0)
            {
                Sunspec_Structure *u = addNode(datain, ssid);
                SunSpecLoad(&(u->repeat), address + 10, ssid, __bswap_16(datain.mppt.N));
            }
            SunSpecLoad(address + length);
        }
        break;
        case 401:
        {
            scanList += String(ssid) + String(",");
            SunSpec_Data_Structure_Union datain = {};
            if (readHreg(address, (uint16_t *)&datain, 16) == 0)
            {
                Sunspec_Structure *u = addNode(datain, ssid);
                SunSpecLoad(&(u->repeat), address + 16, ssid, __bswap_16(datain.uuid401.N));
            }
            SunSpecLoad(address + length);
        }
        break;
        case 402:
        {
            scanList += String(ssid) + String(",");
            SunSpecLoad(address + length);
        }
        break;
        case 403:
        {
            scanList += String(ssid) + String(",");
            SunSpecLoad(address + length);
        }
        break;
        case 404:
        {
            scanList += String(ssid) + String(",");
            SunSpecLoad(address + length);
        }
        break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 120:
        case 121:
        case 122:
        case 123:
        case 124:
        case 125:
        case 126:
        case 127:
        case 128:
        case 129:
        case 130:
        case 131:
        case 132:
        case 133:
        case 134:
        case 135:
        case 136:
        case 137:
        case 138:
        case 139:
        case 140:
        case 141:
        case 142:
        case 143:
        case 144:
        case 145:
        case 201:
        case 202:
        case 203:
        case 204:
        case 211:
        case 212:
        case 213:
        case 214:
        case 220:
        case 302:
        case 303:
        case 304:
        case 305:
        case 306:
        case 307:
        case 308:
        case 501:
        case 502:
        case 601:
        case 801:
        case 802:
        case 803:
        case 804:
        case 805:
        case 806:
        case 807:
        case 808:
        case 809:
        case 63001:
        case 63002:
        case 64001:
        case 64020:
        case 64101:
        case 64110:
        case 64111:
        case 64112:
        {
            scanList += String(ssid) + String(",");
            SunSpecLoad(address + length);
        }
        break;
        }
    }
    free(d);
    d = nullptr;
}
// =====================================================================================================================
// node unit
Sunspec_Structure *SunSpec::addNode(SunSpec_Data_Structure_Union u, uint16_t ssid)
{
    Sunspec_Structure *p = new Sunspec_Structure(u, ssid);
    if (pHead == nullptr)
    {
        pHead = p;
    }
    else
    {
        p->pNext = pHead;
        pHead = p;
    }
    return p;
}

SunSpec_Repeating_Structure *SunSpec::addNode(SunSpec_Repeating_Structure **h, SunSpec_Repeating_Union in)
{
    SunSpec_Repeating_Structure *p = new SunSpec_Repeating_Structure(in);
    if (*h == nullptr)
    {
        *h = p;
    }
    else
    {
        p->pNext = *h;
        *h = p;
    }
    return *h;
}

// =====================================================================================================================
void SunSpec::deleteList()
{
    Sunspec_Structure *p = pHead;
    while (p != nullptr)
    {
        Sunspec_Structure *pre = p;
        p = p->pNext;
        SunSpec_Repeating_Structure *uni = pre->repeat;
        while (uni != nullptr)
        {
            SunSpec_Repeating_Structure *tmp = uni;
            uni = uni->pNext;
            delete tmp;
        }
        pre->repeat = nullptr;
        delete pre;
    }
    scanList = "";
    pHead = nullptr;
}
// common
String SunSpec::getString(SunSpec_Common_Model_Structure info)
{
    String res = "{";
    char st[33];
    memset(st, 0, 33);
    memcpy(st, info.Mn, sizeof(info.Mn));
    res += "\"Mn\":\"" + String(st) + "\"";
    memset(st, 0, 33);
    memcpy(st, info.Md, sizeof(info.Md));
    res += ",\"Md\":\"" + String(st) + "\"";
    memset(st, 0, 33);
    memcpy(st, info.Opt, sizeof(info.Opt));
    res += ",\"Opt\":\"" + String(st) + "\"";
    memset(st, 0, 33);
    memcpy(st, info.Vr, sizeof(info.Vr));
    res += ",\"Vr\":\"" + String(st) + "\"";
    memset(st, 0, 33);
    memcpy(st, info.SN, sizeof(info.SN));
    res += ",\"SN\":\"" + String(st) + "\"";
    res += ",\"DA\":" + String(__bswap_16(info.DA));
    res += ",\"Pad\":" + String(__bswap_16(info.PA));
    res += "}";
    // log_d("UUID_1: %s", res.c_str());
    return res;
}
// 101============================================================================================
String SunSpec::getString(Sunspec_Inverter_Modbus_Register info)
{
    String res = "{";
    res += "id:" + String(convert(info.ID));
    res += FloatString(",A1:", sf_convert<uint16_t>(convert(info.A), convert(info.A_SF))); //
    res += FloatString(",A2:", sf_convert<uint16_t>(convert(info.AphaA), convert(info.A_SF)));
    res += FloatString(",A3:", sf_convert<uint16_t>(convert(info.AphaB), convert(info.A_SF)));
    res += FloatString(",A4:", sf_convert<uint16_t>(convert(info.AphaC), convert(info.A_SF)));
    res += FloatString(",A5:", sf_convert<uint16_t>(convert(info.PPVphAB), convert(info.V_SF)));
    res += FloatString(",A6:", sf_convert<uint16_t>(convert(info.PPVphBC), convert(info.V_SF)));
    res += FloatString(",A7:", sf_convert<uint16_t>(convert(info.PPVphCA), convert(info.V_SF)));
    res += FloatString(",A8:", sf_convert<uint16_t>(convert(info.PhVphA), convert(info.V_SF)));
    res += FloatString(",A9:", sf_convert<uint16_t>(convert(info.PhVphB), convert(info.V_SF)));
    res += FloatString(",A10:", sf_convert<uint16_t>(convert(info.PhVphC), convert(info.V_SF)));
    res += FloatString(",A11:", sf_convert<uint16_t>(convert(info.W), convert(info.W_SF)));
    res += FloatString(",A12:", sf_convert<uint16_t>(convert(info.Hz), convert(info.Hz_SF)));
    res += FloatString(",A13:", sf_convert<uint16_t>(convert(info.VA), convert(info.VA_SF)));
    res += FloatString(",A14:", sf_convert<uint16_t>(convert(info.VAr), convert(info.VAr_SF)));
    res += FloatString(",A15:", sf_convert<uint16_t>(convert(info.PF), convert(info.PF_SF)));
    res += FloatString(",A16:", sf_convert<uint32_t>((convert(info.WH_HIGH) << 16) | convert(info.WH_LOW), convert(info.WH_SF)));
    res += FloatString(",A17:", sf_convert<uint16_t>(convert(info.DCA), convert(info.DCA_SF)));
    res += FloatString(",A18:", sf_convert<uint16_t>(convert(info.DCV), convert(info.DCV_SF)));
    res += FloatString(",A19:", sf_convert<uint16_t>(convert(info.DCW), convert(info.DCW_SF)));
    res += FloatString(",A20:", sf_convert<int16_t>(convert(info.TmpCab), convert(info.Tmp_SF)));
    res += FloatString(",A21:", sf_convert<int16_t>(convert(info.TmpSnk), convert(info.Tmp_SF)));
    res += FloatString(",A22:", sf_convert<int16_t>(convert(info.TmpTrns), convert(info.Tmp_SF)));
    res += FloatString(",A23:", sf_convert<int16_t>(convert(info.TmpOt), convert(info.Tmp_SF)));

    res += ValueString<uint16_t>(",A24:", convert(info.St));
    res += ValueString<uint16_t>(",A25:", convert(info.StVnd));
    res += ValueString<uint32_t>(",A26:", convert(info.Evt1));
    res += ValueString<uint32_t>(",A27:", convert(info.Evt2));
    res += ValueString<uint32_t>(",A28:", convert(info.EvtVnd1));
    res += ValueString<uint32_t>(",A29:", convert(info.EvtVnd2));
    res += ValueString<uint32_t>(",A30:", convert(info.EvtVnd3));
    res += ValueString<uint32_t>(",A31:", convert(info.EvtVnd4));
    res += "}";
    // log_d("UUID 101: %s", res.c_str());
    return res;
}
String SunSpec::getString(Sunspec_Inverter_Float_Modbus_Register info)
{
    String res = "{";
    res += "id:" + String(convert(info.ID));
    res += FloatString(",A1:", convert(&info.A));
    res += FloatString(",A2:", convert(&info.AphaA));
    res += FloatString(",A3:", convert(&info.AphaB));
    res += FloatString(",A4:", convert(&info.AphaC));
    res += FloatString(",A5:", convert(&info.PPVphAB));
    res += FloatString(",A6:", convert(&info.PPVphBC));
    res += FloatString(",A7:", convert(&info.PPVphCA));
    res += FloatString(",A8:", convert(&info.PhVphA));
    res += FloatString(",A9:", convert(&info.PhVphB));
    res += FloatString(",A10:", convert(&info.PhVphC));
    res += FloatString(",A11:", convert(&info.W));
    res += FloatString(",A12:", convert(&info.Hz));
    res += FloatString(",A13:", convert(&info.VA));
    res += FloatString(",A14:", convert(&info.VAr));
    res += FloatString(",A15:", convert(&info.PF));
    res += FloatString(",A16:", convert(&info.WH));
    res += FloatString(",A17:", convert(&info.DCA));
    res += FloatString(",A18:", convert(&info.DCV));
    res += FloatString(",A19:", convert(&info.DCW));
    res += FloatString(",A20:", convert(&info.TmpCab));
    res += FloatString(",A21:", convert(&info.TmpSnk));
    res += FloatString(",A22:", convert(&info.TmpTrns));
    res += FloatString(",A23:", convert(&info.TmpOt));
    res += ValueString<uint16_t>(",A24:", convert(info.St));
    res += ValueString<uint16_t>(",A25:", convert(info.StVnd));
    res += ValueString<uint32_t>(",A26:", convert(info.Evt1));
    res += ValueString<uint32_t>(",A27:", convert(info.Evt2));
    res += ValueString<uint32_t>(",A28:", convert(info.EvtVnd1));
    res += ValueString<uint32_t>(",A29:", convert(info.EvtVnd2));
    res += ValueString<uint32_t>(",A30:", convert(info.EvtVnd3));
    res += ValueString<uint32_t>(",A31:", convert(info.EvtVnd4));
    res += "}";
    return res;
}
// 160============================================================================================
String SunSpec::getString(Sunspec_Multiple_MPPT_Inverter_Register info, SunSpec_Repeating_Structure *u) // get multiple mppt
{
    String res = "{";
    res += "Evt:" + String(convert(info.Evt));
    res += ValueString<uint16_t>(",N:", convert(info.N));
    res += ValueString<uint16_t>(",TmsPer:", convert(info.TmsPer));
    res += ",StringList:";
    SunSpec_Repeating_Structure *p = u;
    String s = "[";
    while (p != nullptr)
    {
        if (s[s.length() - 1] != '[')
        {
            s += ",";
        }
        s += getString(p->data.u160, info);
        p = p->pNext;
    }
    res += s;
    res += "]}";
    // log_d("UUDI_160: %s", res.c_str());
    return res;
}
String SunSpec::getString(Sunspec_Multiple_MPPT_Inverter_Register_Repeating info, Sunspec_Multiple_MPPT_Inverter_Register sf)
{
    String res = "{";
    res += "ID:" + String(convert(info.ID));
    char st[17];
    memset(st, 0, 17);
    memcpy(st, info.IDStr, sizeof(info.IDStr));
    res += ",IDStr:\"" + String(st) + String("\"");
    res += FloatString(",A1:", sf_convert<uint16_t>(convert(info.DCA), convert(sf.DCA_SF)));
    res += FloatString(",A2:", sf_convert<uint16_t>(convert(info.DCV), convert(sf.DCV_SF)));
    res += FloatString(",A3:", sf_convert<uint16_t>(convert(info.DCW), convert(sf.DCW_SF)));
    res += FloatString(",A4:", sf_convert<uint32_t>((convert(info.DCWH_H) << 16) | (convert(info.DCWH_L)), convert(sf.DCWH_SF)));
    res += ValueString<uint32_t>(",A5:", convert(info.Tms));
    res += ValueString<uint16_t>(",A6:", convert(info.Tmp));
    res += ValueString<uint16_t>(",A7:", convert(info.DCSt));
    res += ValueString<uint32_t>(",A8:", convert(info.DCEvt));
    res += "}";
    return res;
}
// 401============================================================================================
String SunSpec::getString(Sunspec_String_Combiner_UUID_401 info, SunSpec_Repeating_Structure *u)
{
    String res = "{";
    res += "N:" + String(convert(info.N));
    res += FloatString(",DCAMAX:", sf_convert<uint16_t>(convert(info.DCAMAX), convert(info.DCA_SF)));
    res += ValueString<uint32_t>(",Evt:", (convert(info.Evt_High) << 16) | (convert(info.Evt_Low)));
    res += ValueString<uint32_t>(",EvtVnd:", (convert(info.EvtVnd_High) << 16) | (convert(info.EvtVnd_Low)));
    res += FloatString(",DCA:", sf_convert<int16_t>(convert(info.DCA), convert(info.DCA_SF)));
    res += FloatString(",DCAhr:", sf_convert<uint32_t>(convert(info.DCAhr), convert(info.DCAhr_SF)));
    res += FloatString(",DCV:", sf_convert<int16_t>(convert(info.DCV), convert(info.DCV_SF)));
    res += ValueString<int16_t>(",Tmp:", convert(info.Tmp));
    // res += ",StringList:" + Traversal(list.u401, info);
    res += ",StringList:";
    SunSpec_Repeating_Structure *p = u;
    String s = "[";
    while (p != nullptr)
    {
        if (s[s.length() - 1] != '[')
        {
            s += ",";
        }
        s += getString(p->data.u401, info);
        p = p->pNext;
    }
    res += s;
    res += "]}";
    return res;
}
String SunSpec::getString(Sunspec_String_Combiner_Array_UUID_401_Reapeating info, Sunspec_String_Combiner_UUID_401 sf)
{
    String res = "{";
    res += "ID:" + String(convert(info.inID));
    res += ValueString<uint32_t>(",A1:", (convert(info.inEvt_High) << 16) | (convert(info.inEvt_Low)));       //evtvnd
    res += ValueString<uint32_t>(",A2:", (convert(info.inEvtVnd_High) << 16) | (convert(info.inEvtVnd_Low))); //evtvnd
    res += FloatString(",A3:", sf_convert<uint16_t>(convert(info.inDCA), convert(sf.DCA_SF)));
    res += FloatString(",A4:", sf_convert<uint32_t>(convert(info.inDCAhr), convert(sf.DCAhr_SF)));
    res += "}";
    return res;
}
// ===============================================================================================
String SunSpec::getString()
{
    String res = "";
    res += "ScanList:[" + scanList.substring(0, scanList.length() - 1) + String("]");
    Sunspec_Structure *p = pHead;
    while (p != nullptr)
    {
        if (res[res.length() - 1] == ']' || res[res.length() - 1] == '}')
        {
            res += ",";
        }
        res += String("\"UUID_") + String(p->id) + String("\":");
        switch (p->id)
        {
        case 1:
        {
            res += getString(p->data.common);
        }
        break;
        case 101:
        case 102:
        case 103:
        {
            res += getString(p->data.inverter);
        }
        break;
        case 160:
        {
            res += getString(p->data.mppt, p->repeat);
        }
        break;
        case 401:
        {
            res += getString(p->data.uuid401, p->repeat);
        }
        break;
        }
        p = p->pNext;
    }
    // res += "}";
    // log_d("res: %s", res.c_str());
    return res;
}

void SunSpec::Print()
{
    Sunspec_Structure *p = pHead;
    while (p != nullptr)
    {
        log_d("--node is: %d", p->id);
        switch (p->id)
        {
        case 1:
        {
            getString(p->data.common);
        }
        break;
        case 101:
        case 102:
        case 103:
        {
            getString(p->data.inverter);
        }
        break;
        case 111:
        case 112:
        case 113:
        {
            getString(p->data.float_inverter);
        }
        break;
        case 160:
        {
            getString(p->data.mppt, p->repeat);
        }
        break;
        case 401:
        {
            getString(p->data.uuid401, p->repeat);
        }
        break;
        }
        p = p->pNext;
    }
}
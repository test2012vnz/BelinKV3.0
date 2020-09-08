#include "type.h"

const char *DeviceLISTNAME[Device_List_Maximum] = {"NULL", "Micro_DPM380", "Selec_MFM_383", "Vinasino_VSE3T_5B", "Vinasino_VSE1T_10100B", "SolarBK_Grid_Tie", "SolarBK_Hybrid", "SMA_Grid_Tie", "SMA_Battery_Inverter",
                                                   "SMA_SolidQ", "ABB_Grid_Tie", "Fronius_Grid_Tie", "SolarEdge_Grid_Tie", "Sungrow_Grid_Tie", "Huawei_Grid_Tie", "Huawei_Grid_Tie_V3",  "Growat_TL3X", "Kipp_and_Zonen_Solar_Radiation", "IMT_Solar_Radiation", "SWH_System"};

const char *Device_TYPENAME[Device_Modbus_Max] = {"NULL", "RTU", "TCP", "IEC", "RTU", "TCP"};

unsigned long IRAM_ATTR seconds()
{
    return (unsigned long)(millis() / 1000ULL);
}

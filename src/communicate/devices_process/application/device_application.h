#ifndef _DEVICE_APPLICATION_H_
#define _DEVICE_APPLICATION_H_

// Metters
#include "DPM/DPM.h"
#include "MFM/MFM.h"
// SolarBK Bigkilowatt
#include "Sofar/Sofar.h"
#include "Sofar/Hybrid.h"

// Huawei Inverter, support Modbus registers mapping V2 and V3
#include "Huawei/HuaweiV2.h"
#include "Huawei/HuaweiV3.h"

// Kipp & Zonen radiation sensor RT1
#include "KippZonen/KippZonen.h"

// SMA SolidQ Inverter
#include "SMA/SolidQ.h"
#include "SMA/SunnyIsland.h"

// Sungrow Inverter
#include "Sungrow/Sungrow.h"
// Sunspec devices:  SMA, ABB
#include "sunspec/SunSpec.h"

// IEC 62056-21 Devices
#include "IEC62056_21/Vinasino/VinasinoVSE3T.h"
//SolarBK SWH System
#include "SWH/SWH.h"

#endif
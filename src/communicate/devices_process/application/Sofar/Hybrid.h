#ifndef _HYBRID_H_
#define _HYBRID_H_

#include "../../mb.h"

typedef struct
{
    uint16_t Operation_Status;                  // 0x0200
    uint16_t Fault1;                            // 0x0201
    uint16_t Fault2;                            // 0x0202
    uint16_t Fault3;                            // 0x0203
    uint16_t Fault4;                            // 0x0204
    uint16_t Fault5;                            // 0x0205
    uint16_t Grid_A_Voltage;                    // 0x0206     // * 0.1
    uint16_t Grid_A_Current;                    // 0x0207     // * 0.01
    uint16_t Reservered1[4];                    // 0x0208-0x020B
    uint16_t Grid_Frequency;                    // 0x020C     // * 0.01
    int16_t Battery_Charge_Discharge_Power;     // 0x020D *0.01
    uint16_t Battery_Cell_Voltage;              // 0x020E     // * 0.1
    int16_t Battery_Charge_Discharge_Current;   // 0x020F     // * 0.01
    uint16_t Battery_Capacity_SOC;              // 0x0210
    int16_t Battery_Temperature;                // 0x0211
    int16_t Grid_Point_Power;                   // 0x0212     // * 0.01
    uint16_t Load_Power;                        // 0x0213     // * 0.01
    int16_t Hybrid_Inverter_Power;              // 0x0214     // * 0.01
    uint16_t PV_Generation_Power;               // 0x0215     // * 0.01
    int16_t EPS_Output_Voltage;                 // 0x0216     // * 0.1
    int16_t EPS_Output_Power;                   // 0x0217     // * 0.01
    uint16_t Today_Generation;                  // 0x0218     // * 0.01
    uint16_t Today_Export_Power;                // 0x0219     // * 0.01
    uint16_t Today_Import_Power;                // 0x021A     // * 0.01
    uint16_t Today_Load_Consuming;              // 0x021B     // * 0.01
    uint16_t Total_Generation[2];               // 0x021C-0x021D //***********
    uint16_t Total_Export_Power[2];             // 0x021E-0x021F //***********
    uint16_t Total_Import_Power[2];             // 0x0220-0x0221 //***********
    uint16_t Total_Load_Consuming_Power[2];     // 0x0222-0x0223 //***********
    uint16_t Battery_today_Charge_Power;        // 0x0224     // * 0.01
    uint16_t Battery_today_Discharge_Power;     // 0x0225     // * 0.01 //***********
    uint16_t Battery_Total_Charge_Power[2];     // 0x0226-0x0227//***********
    uint16_t Battery_Total_Discharge_Power[2];  // 0x0228-0x0229//***********
    uint16_t Countdown_Time;                    // 0x022A
    uint16_t Inverter_Alarm_Info;               // 0x022B
    uint16_t Battery_Cycle_Time;                // 0x022C
    int16_t Inverter_Bus_Voltage;               // 0x022D     // * 0.1
    int16_t LLC_Bus_Voltage;                    // 0x022E     // * 0.1
    int16_t Buckcurrent;                        // 0x022F     // * 0.01
    int16_t Grid_R_Voltage;                     // 0x0230     // * 0.1
    int16_t Grid_R_Current;                     // 0x0231     // * 0.01
    uint16_t Reservered2[5];                    // 0x0232-0x0236
    uint16_t Battery_Health;                    // 0x0237
    int16_t Hybrid_internal_temperature;        // 0x0238
    int16_t Heat_shrink_temperature;            // 0x0239
    uint16_t Country_code;                      // 0x023A
    int16_t DCI_Current;                        // 0x023B
    int16_t DCI_Voltage;                        // 0x023C     // * 0.1
    uint16_t Battery_Fault_List1;               // 0x023D
    uint16_t Battery_Fault_List2;               // 0x023E
    uint16_t Battery_Fault_List3;               // 0x023F
    uint16_t Battery_Fault_List4;               // 0x0240
    uint16_t Battery_Fault_List5;               // 0x0241
    uint16_t Communication_board_internal_info; // 0x0242
    uint16_t Today_Generation_time;             // 0x0243
    uint16_t Total_Generation_time[2];          // 0x0244-0x0245//***********
    uint16_t Iso_resistance_PV1_GND;            // 0x0246
    uint16_t Iso_resistance_PV2_GND;            // 0x0247
    uint16_t Iso_resistance_PV_GND;             // 0x0248
    uint16_t Reservered3[7];                    // 0x0249-0x024F
    uint16_t PV1_Voltage;                       // 0x0250     // * 0.1
    uint16_t PV1_Current;                       // 0x0251     // * 0.01
    uint16_t PV1_Power;                         // 0x0252     // * 0.01
    uint16_t PV2_Voltage;                       // 0x0253     // * 0.1
    uint16_t PV2_Current;                       // 0x0254     // * 0.01
    uint16_t PV2_Power;                         // 0x0255     // * 0.01

    uint16_t Battery_Type;                     // 0x10B0
    uint16_t Battery_Capacity;                 // 0x10B1
    uint16_t Energy_Manage_Mode;               // 0x10B2
    uint16_t Max_Charge_Voltage;               // 0x10B3     // * 0.1
    uint16_t Max_Charge_Current;               // 0x10B4     // * 0.01
    uint16_t Over_Voltage_Protection_Voltage;  // 0x10B5     // * 0.1
    uint16_t Min_Discharge_Voltage;            // 0x10B6     // * 0.1
    uint16_t Max_Discharge_Current;            // 0x10B7     // * 0.01
    uint16_t Battery_Under_Voltage_Protection; // 0x10B8     // * 0.1
    uint16_t Depth_of_Discharge;               // 0x10B9
    uint16_t Discharge_Time;                   // 0x10BA
    uint16_t Empty_Battery_Voltage;            // 0x10BB     // * 0.01
    uint16_t Full_Battery_Voltage;             // 0x10BC     // * 0.01

    uint16_t Product_Code_Name;    // 0x2000
    char SerialNumber[14];         // 0x2001-0x2007    // ASCII
    char ARM_Software_version[4];  // 0x2008-0x2009    // ASCII
    char Hardware_version[4];      // 0x200A-0x200B    // ASCII
    char DSPS_Software_version[4]; // 0x200C-0x200D    // ASCII
    char DSPM_Software_version[4]; // 0x200E-0x200F    // ASCII

} SofarHybrid_3k_6k_Modbus_Register;
class Hybrid : public MB
{
public:
    Hybrid(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        status = false;
        id = id_count++;
        del = Constructor;
        is_Real_Time_Data_OK = false;
        is_Battery_Read_OK = false;
        is_Product_Information_OK = false;
        local_id = t.info.RTU.ID;
    }
    ~Hybrid()
    {
        id_count--;
    }
    bool load();
    String toString()
    {
        return getString();
    }

private:
    bool status = false;
    uint8_t local_id = 0;
    SofarHybrid_3k_6k_Modbus_Register d = {};
    bool is_Real_Time_Data_OK;
    bool is_Battery_Read_OK;
    bool is_Product_Information_OK;
    static int id_count;
    static void Constructor()
    {
        id_count--;
    }
    String getString()
    {
        String SerialNumber = "";
        String ARM_Software_version = "";
        String Hardware_version = "";
        String DSPS_Software_version = "";
        String DSPM_Software_version = "";

        if (is_Product_Information_OK == true)
        {
            char str[15];
            memset(str, 0, 15);
            for (int i = 0; i < 14; i++)
            {
                str[i] = (i % 2) ? d.SerialNumber[i - 1] : d.SerialNumber[i + 1];
            }
            SerialNumber = String(str);
            memset(str, 0, 15);
            for (int i = 0; i < 4; i++)
            {
                str[i] = (i % 2) ? d.ARM_Software_version[i - 1] : d.ARM_Software_version[i + 1];
            }
            ARM_Software_version = String(str);
            memset(str, 0, 15);
            for (int i = 0; i < 4; i++)
            {
                str[i] = (i % 2) ? d.Hardware_version[i - 1] : d.Hardware_version[i + 1];
            }
            Hardware_version = String(str);
            memset(str, 0, 15);
            for (int i = 0; i < 4; i++)
            {
                str[i] = (i % 2) ? d.DSPS_Software_version[i - 1] : d.DSPS_Software_version[i + 1];
            }
            DSPS_Software_version = String(str);
            memset(str, 0, 15);
            for (int i = 0; i < 4; i++)
            {
                str[i] = (i % 2) ? d.DSPM_Software_version[i - 1] : d.DSPM_Software_version[i + 1];
            }
            DSPM_Software_version = String(str);
        }

        // data
        String data = "";
        if (is_Real_Time_Data_OK == true)
        {
            data += ",A0:" + String(d.Operation_Status); //Sua thanh ASCII
            data += ",A1:" + String(__bswap_16(d.Fault1), HEX);
            data += ",A2:" + String(__bswap_16(d.Fault2), HEX);
            data += ",A3:" + String(__bswap_16(d.Fault3), HEX);
            data += ",A4:" + String(__bswap_16(d.Fault4), HEX);
            data += ",A5:" + String(__bswap_16(d.Fault5), HEX);
            data += ",A6:" + String(__bswap_16(d.Grid_A_Voltage) * 0.1);
            data += ",A7:" + String(__bswap_16(d.Grid_A_Current) * 0.01);
            data += ",A8:" + String(__bswap_16(d.Grid_Frequency) * 0.01);
            data += ",A9:" + String(__bswap_16(d.Battery_Charge_Discharge_Power) * 0.01);
            data += ",A10:" + String(__bswap_16(d.Battery_Cell_Voltage) * 0.1);
            data += ",A11:" + String(__bswap_16(d.Battery_Charge_Discharge_Current) * 0.01);
            data += ",A12:" + String(__bswap_16(d.Battery_Capacity_SOC));
            data += ",A13:" + String(__bswap_16(d.Battery_Temperature));
            data += ",A14:" + String(__bswap_16(d.Grid_Point_Power) * 0.01);
            data += ",A15:" + String(__bswap_16(d.Load_Power) * 0.01);
            data += ",A16:" + String(__bswap_16(d.Hybrid_Inverter_Power) * 0.01);
            data += ",A17:" + String(__bswap_16(d.PV_Generation_Power) * 0.01);
            data += ",A18:" + String(__bswap_16(d.EPS_Output_Voltage) * 0.1);
            data += ",A19:" + String(__bswap_16(d.EPS_Output_Power) * 0.01);
            data += ",A20:" + String(__bswap_16(d.Today_Generation) * 0.01);
            data += ",A21:" + String(__bswap_16(d.Today_Export_Power) * 0.01);
            data += ",A22:" + String(__bswap_16(d.Today_Import_Power) * 0.01);
            data += ",A23:" + String(__bswap_16(d.Today_Load_Consuming) * 0.01);
            data += ",A24:" + String((uint32_t)__bswap_16(d.Total_Generation[0]) << 16 | __bswap_16(d.Total_Generation[1]));
            data += ",A25:" + String((uint32_t)__bswap_16(d.Total_Export_Power[0]) << 16 | __bswap_16(d.Total_Export_Power[1]));
            data += ",A26:" + String((uint32_t)__bswap_16(d.Total_Import_Power[0]) << 16 | __bswap_16(d.Total_Import_Power[1]));
            data += ",A27:" + String((uint32_t)__bswap_16(d.Total_Load_Consuming_Power[0]) << 16 | __bswap_16(d.Total_Load_Consuming_Power[1]));
            data += ",A28:" + String(__bswap_16(d.Battery_today_Charge_Power) * 0.01);
            data += ",A29:" + String(__bswap_16(d.Battery_today_Discharge_Power) * 0.01);
            data += ",A30:" + String((uint32_t)__bswap_16(d.Battery_Total_Charge_Power[0]) << 16 | __bswap_16(d.Battery_Total_Charge_Power[1]));
            data += ",A31:" + String((uint32_t)__bswap_16(d.Battery_Total_Discharge_Power[0]) << 16 | __bswap_16(d.Battery_Total_Discharge_Power[1]));
            data += ",A32:" + String(__bswap_16(d.Countdown_Time));
            data += ",A33:" + String(__bswap_16(d.Inverter_Alarm_Info));
            data += ",A34:" + String(__bswap_16(d.Battery_Cycle_Time));
            data += ",A35:" + String(__bswap_16(d.Inverter_Bus_Voltage) * 0.1);
            data += ",A36:" + String(__bswap_16(d.LLC_Bus_Voltage) * 0.1);
            data += ",A37:" + String(__bswap_16(d.Buckcurrent) * 0.1);
            data += ",A38:" + String(__bswap_16(d.Grid_R_Voltage) * 0.1);
            data += ",A39:" + String(__bswap_16(d.Grid_R_Current) * 0.01);
            data += ",A40:" + String(__bswap_16(d.Battery_Health));
            data += ",A41:" + String(__bswap_16(d.Hybrid_internal_temperature));
            data += ",A42:" + String(__bswap_16(d.Heat_shrink_temperature));
            data += ",A43:" + String(__bswap_16(d.Country_code));
            data += ",A44:" + String(__bswap_16(d.DCI_Current));
            data += ",A45:" + String(__bswap_16(d.DCI_Voltage) * 0.1);
            data += ",A46:" + String(__bswap_16(d.Battery_Fault_List1));
            data += ",A47:" + String(__bswap_16(d.Battery_Fault_List2));
            data += ",A48:" + String(__bswap_16(d.Battery_Fault_List3));
            data += ",A49:" + String(__bswap_16(d.Battery_Fault_List4));
            data += ",A50:" + String(__bswap_16(d.Battery_Fault_List5));
            data += ",A51:" + String(__bswap_16(d.Communication_board_internal_info));
            data += ",A52:" + String(__bswap_16(d.Today_Generation_time));
            data += ",A53:" + String((uint32_t)__bswap_16(d.Total_Generation_time[0]) << 16 | __bswap_16(d.Total_Generation_time[1])); //
            data += ",A54:" + String(__bswap_16(d.Iso_resistance_PV1_GND));
            data += ",A55:" + String(__bswap_16(d.Iso_resistance_PV2_GND));
            data += ",A56:" + String(__bswap_16(d.Iso_resistance_PV_GND));
            data += ",A57:" + String(__bswap_16(d.PV1_Voltage) * 0.1);
            data += ",A58:" + String(__bswap_16(d.PV1_Current) * 0.01);
            data += ",A59:" + String(__bswap_16(d.PV1_Power) * 0.01);
            data += ",A60:" + String(__bswap_16(d.PV2_Voltage) * 0.1);
            data += ",A61:" + String(__bswap_16(d.PV2_Current) * 0.01);
            data += ",A62:" + String(__bswap_16(d.PV2_Power) * 0.01);
        }
        if (is_Battery_Read_OK == true)
        {
            data += ",A63:" + String(__bswap_16(d.Battery_Type));
            data += ",A64:" + String(__bswap_16(d.Battery_Capacity));
            data += ",A65:" + String(__bswap_16(d.Energy_Manage_Mode));
            data += ",A66:" + String(__bswap_16(d.Max_Charge_Voltage) * 0.1);
            data += ",A67:" + String(__bswap_16(d.Max_Charge_Current) * 0.01);
            data += ",A68:" + String(__bswap_16(d.Over_Voltage_Protection_Voltage) * 0.1);
            data += ",A69:" + String(__bswap_16(d.Min_Discharge_Voltage) * 0.1);
            data += ",A70:" + String(__bswap_16(d.Max_Discharge_Current) * 0.01);
            data += ",A71:" + String(__bswap_16(d.Battery_Under_Voltage_Protection) * 0.1);
            data += ",A72:" + String(__bswap_16(d.Depth_of_Discharge));
            data += ",A73:" + String(__bswap_16(d.Discharge_Time));
            data += ",A74:" + String(__bswap_16(d.Empty_Battery_Voltage) * 0.01);
            data += ",A75:" + String(__bswap_16(d.Full_Battery_Voltage) * 0.01);
        }
        if (is_Product_Information_OK == true)
        {
            data += ",A76:" + String(d.Product_Code_Name);
            data += ",A77:\"" + String(SerialNumber) + String("\"");
            data += ",A78:\"" + String(ARM_Software_version) + String("\"");
            data += ",A79:\"" + String(Hardware_version) + String("\"");
            data += ",A80:\"" + String(DSPS_Software_version) + String("\"");
            data += ",A81:\"" + String(DSPM_Software_version) + String("\"");
        }

        //api
        String s = "";
#if API_VERSION == 1
        

#elif API_VERSION == 0
        s += APIHeader();
        s += ",SofarHybridList:[{";
        s += "ID:" + String(local_id);
        if (is_Real_Time_Data_OK == false)
        {
            s += ",Status:0";
        }
        else
        {
            s += ",Status:1";
            s += data;
        }
        s += "}]}";

#endif
        return s;
    }
};

#endif
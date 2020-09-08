#ifndef _SOFAR_H_
#define _SOFAR_H_
#include "../../mb.h"

typedef struct
{
    /*The Real time data */
    uint16_t Operating_state;                   //0
    uint16_t Fault1;                            //1
    uint16_t Fault2;                            //2
    uint16_t Fault3;                            //3
    uint16_t Fault4;                            //4
    uint16_t Fault5;                            //5
    uint16_t PV1_voltage;                       //6
    int16_t PV1_current;                        //7
    uint16_t PV2_voltage;                       //8
    int16_t PV2_current;                        //9
    uint16_t PV1_power;                         //10
    uint16_t PV2_power;                         //11
    uint16_t Output_active_power;               //12
    int16_t Output_reactive_power;              //13
    uint16_t Grid_frequency;                    //14
    uint16_t A_phase_voltage;                   //15
    uint16_t A_phase_current;                   //16
    uint16_t B_phase_voltage;                   //17
    uint16_t B_phase_current;                   //18
    uint16_t C_phase_voltage;                   //19
    uint16_t C_phase_current;                   //20
    uint16_t Total_production_high_byte;        //21
    uint16_t Total_production_low_byte;         //22
    uint16_t Total_generation_time_high_byte;   //23
    uint16_t Total_generation_time_low_byte;    //24
    uint16_t Today_production;                  //25
    uint16_t Today_generation_time;             //26
    int16_t Inverter_module_temperature;        //27
    int16_t Inverter_inner_temperature;         //28
    uint16_t Inverter_Bus_voltage;              //29
    uint16_t PV1_voltage_sample_by_slave_CPU;   //30
    uint16_t PV1_current_sample_by_slave_CPU;   //31
    uint16_t Count_down_time;                   //32
    uint16_t Inverter_alert_message;            //33
    uint16_t Input_mode;                        //34
    uint16_t Communication_board_inner_message; //35
    uint16_t Insulation_of_PV1H_to_ground;      //36
    uint16_t Insulation_of_PV1L_to_ground;      //37
    uint16_t Insulation_of_PV_to_ground;        //38
    uint16_t Country;                           //39
    /*The Real time data */

    /*Built-in combiner data address table */
    uint16_t Alarm_message_table1; //40
    uint16_t Alarm_message_table2; //41
    uint16_t Alarm_message_table3; //42
    uint16_t Alarm_message_table4; //43
    uint16_t Alarm_message_table5; //44
    uint16_t String_1_voltage;     //45
    int16_t String_1_current;      //46
    uint16_t String_2_voltage;     //47
    int16_t String_2_current;      //48
    uint16_t String_3_voltage;     //49
    int16_t String_3_current;      //50
    uint16_t String_4_voltage;     //51
    int16_t String_4_current;      //52
    uint16_t String_5_voltage;     //53
    int16_t String_5_current;      //54
    uint16_t String_6_voltage;     //55
    int16_t String_6_current;      //56
    uint16_t String_7_voltage;     //57
    int16_t String_7_current;      //58
    uint16_t String_8_voltage;     //59
    int16_t String_8_current;      //60
    /*Built-in combiner data address table */

    /*Product information*/
    uint16_t Product_code;               //61
    char Manufacturer_serial_number[14]; //62
    char Software_version_code[4];       //63
    char Hardware_version_code[4];       //64
    /*Product information*/

} Sofar_Modbus_Register;

typedef struct
{
    uint16_t EN_Active_Power_And_Remote; //bit
    uint16_t Percentage_of_Active_Power_Ouput;
    uint16_t Remote_Switching; // 55 = on 66 = off
    uint16_t reserverd[13];
    // uint16_t Starting_Point_of_pw_reduction_Volt;
    // uint16_t Ending_Point_of_pw_reduction_Volt;

} Sofar_Power_Control_Register;

class Sofar : public MB
{
public:
    Sofar(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
        id = id_count++;
        del = Constructor;
        status = false;
        is_Real_Time_Data_OK = false;
        is_Built_in_combiner_data_address_table_OK = false;
        is_Product_Information_OK = false;
        local_id = t.info.RTU.ID;
    }
    ~Sofar()
    {
        id_count--;
    }
    String toString()
    {
        return getString();
    }
    bool load();

private:
    static int id_count;
    bool status = false;
    uint8_t local_id = 0;
    Sofar_Modbus_Register d = {};
    Sofar_Power_Control_Register r = {};
    static void Constructor()
    {
        id_count--;
    }
    bool is_Real_Time_Data_OK = false;
    bool is_Built_in_combiner_data_address_table_OK = false;
    bool is_Product_Information_OK = false;

    String getString()
    {
        String ProductCode = "";
        String ManufacturerSN = "";
        String SWVersionCode = "";
        String HWVerisonCode = "";
        if (is_Product_Information_OK == true)
        {
            ProductCode = String(__bswap_16(d.Product_code));
            char str[15];
            memset(str, 0, 15);
            memcpy(str, d.Manufacturer_serial_number, 14);

            ManufacturerSN = String(str);
            memset(str, 0, 15);
            memcpy(str, d.Software_version_code, 4);

            SWVersionCode = String(str);
            memset(str, 0, 15);
            memcpy(str, d.Hardware_version_code, 4);

            HWVerisonCode = String(str);
        }

        // data

        String data = "";
        if (is_Real_Time_Data_OK == true)
        {
            data += ",A0:" + String(__bswap_16(d.Operating_state));
            data += ",A1:" + String(__bswap_16(d.Fault1));
            data += ",A2:" + String(__bswap_16(d.Fault2));
            data += ",A3:" + String(__bswap_16(d.Fault3));
            data += ",A4:" + String(__bswap_16(d.Fault4));
            data += ",A5:" + String(__bswap_16(d.Fault5));
            data += ",A6:" + String(__bswap_16(d.PV1_voltage) / (float)10);
            data += ",A7:" + String(((int16_t)__bswap_16(d.PV1_current)) / (float)100);
            data += ",A8:" + String(__bswap_16(d.PV2_voltage) / (float)10);
            data += ",A9:" + String(((int16_t)__bswap_16(d.PV2_current)) / (float)100);
            data += ",A10:" + String(__bswap_16(d.PV1_power) / (float)100);
            data += ",A11:" + String(__bswap_16(d.PV2_power) / (float)100);
            data += ",A12:" + String(__bswap_16(d.Output_active_power) / (float)100);
            data += ",A13:" + String(((int16_t)__bswap_16(d.Output_reactive_power)) / (float)100);
            data += ",A14:" + String(__bswap_16(d.Grid_frequency) / (float)100);
            data += ",A15:" + String(__bswap_16(d.A_phase_voltage) / (float)10);
            data += ",A16:" + String(__bswap_16(d.A_phase_current) / (float)100);
            data += ",A17:" + String(__bswap_16(d.B_phase_voltage) / (float)10);
            data += ",A18:" + String(__bswap_16(d.B_phase_current) / (float)100);
            data += ",A19:" + String(__bswap_16(d.C_phase_voltage) / (float)10);
            data += ",A20:" + String(__bswap_16(d.C_phase_current) / (float)100);
            data += ",A21:" + String((uint32_t)__bswap_16(d.Total_production_high_byte) << 16 | __bswap_16(d.Total_production_low_byte));
            data += ",A22:" + String((uint32_t)__bswap_16(d.Total_generation_time_high_byte) << 16 | __bswap_16(d.Total_generation_time_low_byte));

            //s += ",A21:" + String(__bswap_32(*(uint32_t*)&d.Total_production_high_byte));
            //s += ",A22:" + String(__bswap_32(*(uint32_t*)&d.Total_generation_time_high_byte));
            data += ",A23:" + String(__bswap_16(d.Today_production) / (float)100);
            data += ",A24:" + String(__bswap_16(d.Today_generation_time));
            data += ",A25:" + String(((int16_t)__bswap_16(d.Inverter_module_temperature)));
            data += ",A26:" + String(((int16_t)__bswap_16(d.Inverter_inner_temperature)));
            data += ",A27:" + String(__bswap_16(d.Inverter_alert_message));
            data += ",A28:" + String(__bswap_16(d.Input_mode));
            data += ",A29:" + String(__bswap_16(d.Communication_board_inner_message));
            data += ",A30:" + String(__bswap_16(d.Insulation_of_PV1H_to_ground));
            data += ",A31:" + String(__bswap_16(d.Insulation_of_PV1L_to_ground));
            data += ",A32:" + String(__bswap_16(d.Insulation_of_PV_to_ground));
            data += ",A33:" + String(__bswap_16(d.Country));
        }
        if (is_Built_in_combiner_data_address_table_OK == true)
        {
            data += ",A34:" + String(__bswap_16(d.Alarm_message_table1));
            data += ",A35:" + String(__bswap_16(d.Alarm_message_table2));
            data += ",A36:" + String(__bswap_16(d.Alarm_message_table3));
            data += ",A37:" + String(__bswap_16(d.Alarm_message_table4));
            data += ",A38:" + String(__bswap_16(d.Alarm_message_table5));
            data += ",A39:" + String(__bswap_16(d.String_1_voltage) / (float)10);
            data += ",A40:" + String(__bswap_16(d.String_1_current) / (float)100);
            data += ",A41:" + String(__bswap_16(d.String_2_voltage) / (float)10);
            data += ",A42:" + String(__bswap_16(d.String_2_current) / (float)100);
            data += ",A43:" + String(__bswap_16(d.String_3_voltage) / (float)10);
            data += ",A44:" + String(__bswap_16(d.String_3_current) / (float)100);
            data += ",A45:" + String(__bswap_16(d.String_4_voltage) / (float)10);
            data += ",A46:" + String(__bswap_16(d.String_4_current) / (float)100);
            data += ",A47:" + String(__bswap_16(d.String_5_voltage) / (float)10);
            data += ",A48:" + String(__bswap_16(d.String_5_current) / (float)100);
            data += ",A49:" + String(__bswap_16(d.String_6_voltage) / (float)10);
            data += ",A50:" + String(__bswap_16(d.String_6_current) / (float)100);
            data += ",A51:" + String(__bswap_16(d.String_7_voltage) / (float)10);
            data += ",A52:" + String(__bswap_16(d.String_7_current) / (float)100);
            data += ",A53:" + String(__bswap_16(d.String_8_voltage) / (float)10);
            data += ",A54:" + String(__bswap_16(d.String_8_current) / (float)100);
        }
        if (is_Product_Information_OK == true)
        {
            data += ",A55:\"" + ProductCode + String("\"");
            data += ",A56:\"" + ManufacturerSN + String("\"");
            data += ",A57:\"" + SWVersionCode + String("\"");
            data += ",A58:\"" + HWVerisonCode + String("\"");
        }

        // api
        String s = "";
#if API_VERSION == 1


#elif API_VERSION == 0
        s += APIHeader();
        s += ",listRecord:[{";
        s += "InverterID:" + String(local_id);
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
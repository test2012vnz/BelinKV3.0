#ifndef _SMA_RTU_H_
#define _SMA_RTU_H_
#include "../../mb.h"

typedef struct
{
    //  u8_to_u32 Current_event_number;                         // 30197
    //  u8_to_u32 RReservered0;                                 // 30199
    uint16_t Status_of_device[2];                        // 30201
    uint16_t Power_in_OK_Status[2];                      // 30203
    uint16_t Power_in_warning_status[2];                 // 30205
    uint16_t Power_in_error_status[2];                   // 30207
    uint16_t Reservered1[2];                             // 30209
    uint16_t Recommended_action[2];                      // 30211
    uint16_t Status_Message[2];                          // 30213
    uint16_t Status_Description[2];                      // 30215
    uint16_t Utility_grid_contactor[2];                  // 30217
    uint16_t Temperature_derating[2];                    // 30219
    uint16_t Reservered2[4];                             // 30221 30213
    uint16_t Insulation_resistance[2];                   // 30225
    uint16_t Status_of_key_switch[2];                    // 30227
    uint16_t Local_time_of_device[2];                    // 30229
    uint16_t Maximum_possible_Permanent_active_power[2]; // 30231
    uint16_t Permanent_active_power_limitation[2];       // 30233
    uint16_t Backup_Mode_Status[2];                      // 30235
    uint16_t Grid_type[2];                               // 30237
    uint16_t Status_of_the_GFDI_relay[2];                // 30239
    uint16_t Reservered3[6];                             // 30241 30243 30245
    uint16_t Current_event_number_for_manufacturer[2];   // 30247
    uint16_t Status_of_GFDI_Relay[2];                    // 30249
} SMA1_Structure;

typedef struct
{
    uint16_t Total_AC_energy_fed_in_on_all_line_conductors_kWh[2];           // 30531
    uint16_t Total_AC_energy_fed_in_on_all_line_conductors_MWh[2];           // 30533
    uint16_t Energy_fed_in_on_the_current_day_on_all_line_conductors_Wh[2];  // 30535
    uint16_t Energy_fed_in_on_the_current_day_on_all_line_conductors_kWh[2]; // 30537
    uint16_t Energy_fed_in_on_the_current_day_on_all_line_conductors_MWh[2]; // 30539
    uint16_t Operating_time[2];                                              // 30541
    uint16_t Feed_in_time[2];                                                // 30543
} SMA2_Structure;

typedef struct
{
    uint16_t DC_current_input_1[2];                    // 30769
    uint16_t DC_voltage_input_1[2];                    // 30771
    uint16_t DC_power_input_1[2];                      // 30773
    uint16_t Active_power_on_all_line_conductors[2];   // 30775
    uint16_t Active_power_of_line_conductor_L1[2];     // 30777
    uint16_t Active_power_of_line_conductor_L2[2];     // 30779
    uint16_t Active_power_of_line_conductor_L3[2];     // 30781
    uint16_t Line_voltage_line_conductor_L1_to_N[2];   // 30783
    uint16_t Line_voltage_line_conductor_L2_to_N[2];   // 30785
    uint16_t Line_voltage_line_conductor_L3_to_N[2];   // 30787
    uint16_t Line_voltage_line_conductor_L1_to_L2[2];  // 30789
    uint16_t Line_voltage_line_conductor_L2_to_L3[2];  // 30791
    uint16_t Line_voltage_line_conductor_L3_to_L1[2];  // 30793
    uint16_t Line_current_on_all_line_conductors[2];   // 30795
    uint16_t Line_current_of_line_conductor_L1[2];     // 30797
    uint16_t Line_current_of_line_conductor_L2[2];     // 30799
    uint16_t Line_current_of_line_conductor_L3[2];     // 30801
    uint16_t Power_frequency[2];                       // 30803
    uint16_t Reactive_power_on_all_line_conductors[2]; // 30805
} SMA3_Structure;

typedef struct
{
    uint16_t DC_current_input_2[2]; // 30957
    uint16_t DC_voltage_input_2[2]; // 30959
    uint16_t DC_power_input_2[2];   // 30961
} SMA4_Structure;

typedef struct
{
    uint16_t Number_of_the_current_event[2]; // 30197
} SMA5_Structure;

typedef struct
{
    uint16_t Grid_current_phase_L1[2]; // 30977
    uint16_t Grid_current_phase_L2[2]; // 30979
    uint16_t Grid_current_phase_L3[2]; // 30981
} SMA6_Structure;

typedef struct
{
    uint16_t SMA_Openrating_Status_Control[2]; // 40009
} SMA7_Structure;

class SMARTUClass : public MB
{
public:
    SMARTUClass(Device_Info_Structure t, RS485Driver *r, bool ssnew = false) : MB(t, r)
    {

        isNew = ssnew;
        id = id_count++;
        local_id = t.info.RTU.ID;
        status = false;
        del = destructor;
    }
    ~SMARTUClass()
    {
    }
    bool load(){
        if(isNew){
            return load2();
        }
        return load1();
    }
    bool load1();
    bool load2();
    String toString()
    {
        if(isNew){
            return getString2();
        }
    return getString1();        
    }
    bool connected()
    {
        return status;
    }

private:
    bool status;
    bool isNew = false;
    bool local_id = 0;
    SMA1_Structure d = {};
    SMA2_Structure d2 = {};
    SMA3_Structure d3 = {};
    SMA4_Structure d4 = {};
    SMA5_Structure d5 = {};
    SMA6_Structure d6 = {};
    SMA7_Structure d7 = {};
    static int id_count;
    static void destructor()
    {
        id_count--;
    }

    String getString1();
    String getString2(); // new 

};


#endif
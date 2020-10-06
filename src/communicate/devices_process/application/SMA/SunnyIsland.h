#ifndef _SUNNYISLAND_H_
#define _SUNNYISLAND_H_

#include "../../mb.h"

typedef struct
{
    uint32_t Status_of_device;   // 30201            A0  U32
    uint32_t Reservered[4];      //
    uint32_t Recommended_action; // 30211            A1  U32
    uint32_t Status_Message;     // 30213            A2  U32
    uint32_t Status_Description; // 30215            A3  U32
} SMA_Island_Structure_1;        // SMA1

typedef struct
{
    uint32_t Number_of_generator_starts;              // 30565            A51 U32
    uint32_t Amp_hours_counter_for_battery_charge;    // 30567            A4  U32
    uint32_t Amp_hours_counter_for_battery_discharge; // 30569            A5  U32
    uint32_t Meter_reading_consumption_meter;         // 30571
    uint32_t Reservered[2];                           // 30573 - 30576
    uint32_t Energy_consumed_from_grid_today;         // 30577            A6  U32
    uint32_t Energy_feed_into_grid_today;             // 30579            A7  U32
    uint32_t Grid_reference_counter_reading;          // 30581
    uint32_t Grid_feed_in_counter_reading;            // 30583
    uint32_t Power_outage;                            // 30585            A52 U32(s)
    uint32_t Reservered1[4];                          // 30587 - 30594
    uint32_t Absorbed_energy;                         // 30595            A8  U32
    uint32_t Released_energy;                         // 30597            A9  U32
} SMA_Island_Structure_2;                             // SMA2

typedef struct
{
    uint32_t Power;                 // 30775            A10 S32(W)
    uint32_t Power_L1;              // 30777            A11 S32(W)
    uint32_t Power_L2;              // 30779            A12 S32(W)
    uint32_t Power_L3;              // 30781            A13 S32(W)
    uint32_t Grid_voltage_phase_L1; // 30783            A14 U32/100(V)
    uint32_t Grid_voltage_phase_L2; // 30785            A15 U32/100(V)
    uint32_t Grid_voltage_phase_L3; // 30787            A16 U32/100(V)
    uint32_t Reservered[7];         // 30789 - 30802
    uint32_t Grid_frequency;        // 30803            A20 U32/100
    uint32_t Reactive_power;        // 30805            A53 S32
    uint32_t Reactive_power_L1;     // 30807            A21 S32
    uint32_t Reactive_power_L2;     // 30809            A22 S32
    uint32_t Reactive_power_L3;     // 30811            A23 S32
} SMA_Island_Structure_3;           //SMA3

typedef struct
{
    uint32_t Battery_current;                      // 30843            A24 S32/1000(A)
    uint32_t Current_battery_state_of_charge;      // 30845            A25 U32
    uint32_t Current_battery_capacity;             // 30847            A26 U32
    uint32_t Battery_temperature;                  // 30849            A27 S32
    uint32_t Battery_voltage;                      // 30851            A28 U32/100
    uint32_t Active_battery_charging_mode;         // 30853            A29 U32
    uint32_t Current_battery_charging_set_voltage; // 30855            A30 U32/100
    uint32_t Number_of_battery_charge_throughputs; // 30857            A31 S32
    uint32_t Battery_maint_state_of_charge;        // 30859            A32 U32
    uint32_t Load_power;                           // 30861            A33 S32
    uint32_t Reservered2[3];                       // 30863 - 30868
    uint32_t PV_power_generated;                   // 30869            A34 S32
    uint32_t Reservered1[2];                       // 30871 - 30874
    uint32_t Multifunction_relay_status;           // 30875            A54 U32
    uint32_t Power_supply_status;                  // 30877            A35 U32
    uint32_t Reason_for_generator_request;         // 30879            A36 U32
    uint32_t Reservered;                           // 30881
    uint32_t Status_of_utility_grid;               // 30883            A37 U32
} SMA_Island_Structure_4;                          // SMA4

typedef struct
{
    uint32_t Generator_status;                         // 30917            A38 U32
    uint32_t Reservered[29];                           // 30919 - 30976
    uint32_t Grid_current_phase_L1;                    // 30977            A17 S32(A)/1000
    uint32_t Grid_current_phase_L2;                    // 30979            A18 S32(A)/1000
    uint32_t Grid_current_phase_L3;                    // 30981            A19 S32(A)/1000
    uint32_t Reservered1[2];                           // 30983 - 30986
    uint32_t Fault_battery_state_of_charge;            // 30987            A39 U32/10
    uint32_t Reservered2[4];                           // 30989 - 30996
    uint32_t Lowest_measured_battery_temperature;      // 30997            A55 S32
    uint32_t Highest_measured_battery_temperature;     // 30999            A56 S32
    uint32_t Max_occurred_battery_voltage;             // 31001
    uint32_t Remaining_time_until_full_charge;         // 31003            A57 U32
    uint32_t Remaining_time_until_equalization_charge; // 31005            A58 U32
} SMA_Island_Structure_5;

typedef struct
{
    uint32_t Rated_battery_capacity;           // 40031            A40 U32
    uint32_t Max_battery_temperature;          // 40033
    uint32_t Battery_type;                     // 40035            A41 U32
    uint32_t Rated_battery_voltage;            // 40037            A42 U32
    uint32_t Battery_boost_charge_time;        // 40039
    uint32_t Battery_equalization_charge_time; // 40041
    uint32_t Battery_full_charge_time;         // 40043
    uint32_t Max_battery_charging_current;     // 40045            A43 U32/1000
    uint32_t Rated_generator_current;          // 40047            A44 U32/1000
    uint32_t Automatic_generator_start;        // 40049            A45 U32
    uint32_t Reservered[2];                    // 40051 - 40054
    uint32_t Manual_generator_control;         // 40055            A46 U32
    uint32_t Generator_request_via_power_on;   // 40057            A47 U32
    uint32_t Generator_shutdown_load_limit;    // 40059            A48 U32
    uint32_t Generator_startup_load_limit;     // 40061            A49 U32
} SMA_Island_Structure_6;

typedef struct
{
    uint32_t Acknowledge_generator_errors; // 40137            A59 U32
} SMA_Island_Structure_7;

typedef struct
{
    uint32_t Generator_request;                   // 40535            A50 U32
    uint32_t Reservered[13];                      // 40537 - 40562
    uint32_t Average_operating_time_of_generator; // 40563            A60 U32
} SMA_Island_Structure_8;

class SMASunnyIsland : public MB
{
public:
    SMASunnyIsland(Device_Info_Structure t, RS485Driver *r) : MB(t, r)
    {
    }
    ~SMASunnyIsland()
    {
    }

private:
    /* data */
};

#endif
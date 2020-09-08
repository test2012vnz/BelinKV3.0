#ifndef _VINASINOVSE3T_H_
#define _VINASINOVSE3T_H_

#include "../IEC/IEC_62056_21_Def.h"
#include "../IEC/IEC_62056_21.h"

class Vinasino_VSE3T: public IEC_62056_21
{

public:
    Vinasino_VSE3T(Device_Info_Structure t ,RS485Driver *r):IEC_62056_21(t, r)
    {
        id = id_count++;
        del = destructor;

        memset(Device_address, 0, sizeof(Device_address));
        memcpy(Device_address, t.info.IEC.ID, sizeof(t.info.IEC.ID));

        memset(Device_Pass, 0, sizeof(Device_Pass));
        memcpy(Device_Pass, String("11111111").c_str(), String("11111111").length());

        // Device_address = String(t.info.IEC.ID);
        Primary_value = t.info.IEC.primary;
        Secondary_value = t.info.IEC.secondary;
        // begin(r);

    }

    String toString()
    {
        // data
        String data = "";
        if(status == true){
            data += printdata("a1", d.Positive_active_energy_total);
            data += printdata("a2", d.Positive_active_energy_in_tariff_T1);
            data += printdata("a3", d.Positive_active_energy_in_tariff_T2);
            data += printdata("a4", d.Positive_active_energy_in_tariff_T3);
            data += printdata("a5", d.Positive_active_energy_in_tariff_T4);
            data += printdata("a6", d.Negative_active_energy_total);
            data += printdata("a7", d.Negative_active_energy_in_tariff_T1);
            data += printdata("a8", d.Negative_active_energy_in_tariff_T2);
            data += printdata("a9", d.Negative_active_energy_in_tariff_T3);
            data += printdata("a10", d.Negative_active_energy_in_tariff_T4);
            data += printdata("a21", d.Positive_reactive_energy_total);
            data += printdata("a22", d.Positive_reactive_energy_in_tariff_T1);
            data += printdata("a23", d.Positive_reactive_energy_in_tariff_T2);
            data += printdata("a24", d.Positive_reactive_energy_in_tariff_T3);
            data += printdata("a25", d.Positive_reactive_energy_in_tariff_T4);
            data += printdata("a26", d.Negative_reactive_energy_total);
            data += printdata("a27", d.Negative_reactive_energy_in_tariff_T1);
            data += printdata("a28", d.Negative_reactive_energy_in_tariff_T2);
            data += printdata("a29", d.Negative_reactive_energy_in_tariff_T3);
            data += printdata("a30", d.Negative_reactive_energy_in_tariff_T4);
            data += printdata("a119", d.Positive_active_instantaneous_power);
            data += printdata("a120", d.Positive_active_instantaneous_power_in_phase_L1);
            data += printdata("a121", d.Positive_active_instantaneous_power_in_phase_L2);
            data += printdata("a122", d.Positive_active_instantaneous_power_in_phase_L3);
            data += printdata("a123", d.Negative_active_instantaneous_power);
            data += printdata("a124", d.Negative_active_instantaneous_power_in_phase_L1);
            data += printdata("a125", d.Negative_active_instantaneous_power_in_phase_L2);
            data += printdata("a126", d.Negative_active_instantaneous_power_in_phase_L3);
            data += printdata("a135", d.Positive_reactive_instantaneous_power);
            data += printdata("a136", d.Positive_reactive_instantaneous_power_in_phase_L1);
            data += printdata("a137", d.Positive_reactive_instantaneous_power_in_phase_L2);
            data += printdata("a138", d.Positive_reactive_instantaneous_power_in_phase_L3);

            data += printdata("a148", d.Instantaneous_current_in_phase_L1);
            data += printdata("a149", d.Instantaneous_current_in_phase_L2);
            data += printdata("a150", d.Instantaneous_current_in_phase_L3);
            data += printdata("a158", d.Instantaneous_voltage_in_phase_L1);
            data += printdata("a159", d.Instantaneous_voltage_in_phase_L2);
            data += printdata("a160", d.Instantaneous_voltage_in_phase_L3);
            data += printdata("a161", d.Instantaneous_power_factor);
            data += printdata("a162", d.Instantaneous_power_factor_in_phase_L1);
            data += printdata("a163", d.Instantaneous_power_factor_in_phase_L2);
            data += printdata("a164", d.Instantaneous_power_factor_in_phase_L3);

            data += printdata("a165", d.Frequency);
            data += printdata("a213", d.Meter_serial_number);

            data += printdata("a227",    d.import_export_sign);
        }

        //api
        String s = "";
        #if API_VERSION == 0

        #elif API_VERSION ==1
            s+= APIHeader();
            s+= ",VinasinoList:[{";
            s+= "MetterId: "+ String(id);
            if(status == false)
            {
                s += ",Status:0";
            }
            else
            {
                s += ",Status:1";
                s += data;
            }
            s+="}]}";

        #endif
            // log_d("-->%s", s.c_str());
        return s;
    }

    bool load()
    {
        if(Login()){
            getData();
            status = true;
        }else{
            status = false;
        }
        break_message();
        return status;

        // send_HHU_R2("2020");
        // log_d("%s",receive_HHU_R2().c_str());
        // receive(2000, "123456");
    }

protected:
    IEC_62056_Structure d;
    uint8_t status;
private:
    static int id_count;
    static void destructor() {
        log_d("Vinasino delete!!!");
        id_count--;

    }

    char Device_Pass[20];
    uint16_t Primary_value = 0;
    uint16_t Secondary_value = 0;

    bool Login()
    {
        IEC_62056_21_Structure res;
        res = Sign_On();
        // log_d("---- Mode login: %d", (int)res.protocol_mode);
        if(res.status == false)
        {
            status = 0;
            break_message();
            return false;
        }
        send_ACK('0', '3', '1'); // '3': 2400
        setBaudrate(2400);
        Login_ACK();
        //

        send_Program_Command_Message('P', '1', (char *)Device_Pass);  //.c_str()
        receive(2000, "\x06");
        // log_d("Login OK");
        status = true;
        // Login_ACK();
    }

    bool Login_ACK()
    {
        bool res;
        String str = receive(2000, "\x03");
        uint8_t bbc = _serial->read();
        // Serial.write(bbc);
        if(CheckBBC(bbc, str))
        {
            // log_d("ACK: %s", str.c_str());
            String pwd = str.substring(str.indexOf('('), str.lastIndexOf(')') + 1);
            memset(Device_Pass, 0, sizeof(Device_Pass));
            memcpy(Device_Pass, pwd.c_str(), pwd.length());
            res = 1;
            // res.C = str.charAt(1);
            // res.D = str.substring(2, 3).toInt();
            // str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).toCharArray(res.Data_Set, str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03')).length() + 1);
            // res.Data_Set = str.substring(str.indexOf('\x02') + 1, str.indexOf('\x03'));
        }
        else
        {
            res = 0;
        }
        return res;
    }

    int hget(String ID)
    {
        float res;
        send_HHU_R2(ID);
        String str = receive_HHU_R2();
        if(str == "")
        {
            return 0;
        }
        res = (int)strtol(str.c_str(), NULL, 16);
        return res;
    }

    float fget(String ID, int factor)
    {
        float res;
        // Serial.println(ID);
        send_HHU_R2(ID);
        String str = receive_HHU_R2();
        if(str == "")
        {
            return 0;
        }
        res = str.toFloat();
        res *= pow(10, factor);
        return res;
    }

    String sget(String ID, char *o, size_t size = 20)
    {
        String res;
        send_HHU_R2(ID);
        String str = receive_HHU_R2();
        if(str == "")
        {
            return "";
        }
        memset(o, 0, size);
        memcpy(o, str.c_str(), str.length());
        // str.toCharArray(o, res.length() + 1);

        return String(o);
    }

    void getData()
    {
        d.import_export_sign = hget("C000") ;
        d.Instantaneous_current_in_phase_L1 = fget("2020", -2)*Primary_value/Secondary_value;
        d.Instantaneous_current_in_phase_L2 = fget("2021", -2)*Primary_value/Secondary_value;
        d.Instantaneous_current_in_phase_L3 = fget("2022", -2)*Primary_value/Secondary_value;
        d.Instantaneous_voltage_in_phase_L1 = fget("2023", -1);
        d.Instantaneous_voltage_in_phase_L2 = fget("2024", -1);
        d.Instantaneous_voltage_in_phase_L3 = fget("2025", -1);
        if(bitRead(d.import_export_sign, 3) )
            // if(bitRead(d.import_export_sign, 3) && bitRead(d.import_export_sign, 7))
        {


            d.Negative_active_instantaneous_power_in_phase_L1 = fget("2026", -3)*Primary_value/Secondary_value;
            d.Negative_active_instantaneous_power_in_phase_L2 = fget("2027", -3)*Primary_value/Secondary_value;
            d.Negative_active_instantaneous_power_in_phase_L3 = fget("2028", -3)*Primary_value/Secondary_value;
            d.Negative_active_instantaneous_power = fget("2029", -3)*Primary_value/Secondary_value;
            d.Positive_active_instantaneous_power_in_phase_L1 = 0;
            d.Positive_active_instantaneous_power_in_phase_L2 = 0;
            d.Positive_active_instantaneous_power_in_phase_L3 = 0;
            d.Positive_active_instantaneous_power = 0;
        }
        else
        {
            d.Positive_active_instantaneous_power_in_phase_L1 = fget("2026", -3)*Primary_value/Secondary_value;
            d.Positive_active_instantaneous_power_in_phase_L2 = fget("2027", -3)*Primary_value/Secondary_value;
            d.Positive_active_instantaneous_power_in_phase_L3 = fget("2028", -3)*Primary_value/Secondary_value;
            d.Positive_active_instantaneous_power = fget("2029", -3)*Primary_value/Secondary_value;
            d.Negative_active_instantaneous_power_in_phase_L1 = 0;
            d.Negative_active_instantaneous_power_in_phase_L2 = 0;
            d.Negative_active_instantaneous_power_in_phase_L3 = 0;
            d.Negative_active_instantaneous_power = 0;
        }


        d.Positive_reactive_instantaneous_power_in_phase_L1 = fget("202A", -3)*Primary_value/Secondary_value;
        d.Positive_reactive_instantaneous_power_in_phase_L2 = fget("202B", -3)*Primary_value/Secondary_value;
        d.Positive_reactive_instantaneous_power_in_phase_L3 = fget("202C", -3)*Primary_value/Secondary_value;
        d.Positive_reactive_instantaneous_power = fget("202D", -3)*Primary_value/Secondary_value;
        d.Instantaneous_power_factor_in_phase_L1 = fget("2030", -3);
        d.Instantaneous_power_factor_in_phase_L2 = fget("2031", -3);
        d.Instantaneous_power_factor_in_phase_L3 = fget("2032", -3);
        d.Instantaneous_power_factor = fget("2033", -3);
        d.Frequency = fget("203E", -1);
        sget("C100", d.Meter_serial_number );
        d.Positive_active_energy_total  = fget("9010", -2)*Primary_value/Secondary_value;
        d.Positive_active_energy_in_tariff_T1   = fget("9011", -2)*Primary_value/Secondary_value;
        d.Positive_active_energy_in_tariff_T2   = fget("9012", -2)*Primary_value/Secondary_value;
        d.Positive_active_energy_in_tariff_T3   = fget("9013", -2)*Primary_value/Secondary_value;
        d.Positive_active_energy_in_tariff_T4   = fget("9014", -2)*Primary_value/Secondary_value;
        d.Negative_active_energy_total  = fget("9020", -2)*Primary_value/Secondary_value;
        d.Negative_active_energy_in_tariff_T1   = fget("9021", -2)*Primary_value/Secondary_value;
        d.Negative_active_energy_in_tariff_T2   = fget("9022", -2)*Primary_value/Secondary_value;
        d.Negative_active_energy_in_tariff_T3   = fget("9023", -2)*Primary_value/Secondary_value;
        d.Negative_active_energy_in_tariff_T4   = fget("9024", -2)*Primary_value/Secondary_value;
        d.Positive_reactive_energy_total    = fget("9030", -2)*Primary_value/Secondary_value;
        d.Positive_reactive_energy_in_tariff_T1 = fget("9031", -2)*Primary_value/Secondary_value;
        d.Positive_reactive_energy_in_tariff_T2 = fget("9032", -2)*Primary_value/Secondary_value;
        d.Positive_reactive_energy_in_tariff_T3 = fget("9033", -2)*Primary_value/Secondary_value;
        d.Positive_reactive_energy_in_tariff_T4 = fget("9034", -2)*Primary_value/Secondary_value;
        d.Negative_reactive_energy_total    = fget("9040", -2)*Primary_value/Secondary_value;
        d.Negative_reactive_energy_in_tariff_T1 = fget("9041", -2)*Primary_value/Secondary_value;
        d.Negative_reactive_energy_in_tariff_T2 = fget("9042", -2)*Primary_value/Secondary_value;
        d.Negative_reactive_energy_in_tariff_T3 = fget("9043", -2)*Primary_value/Secondary_value;
        d.Negative_reactive_energy_in_tariff_T4 = fget("9044", -2)*Primary_value/Secondary_value;

    }

};

#endif
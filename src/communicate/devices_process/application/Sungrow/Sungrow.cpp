#include "Sungrow.h"

int Sungrow::id_count = 1;

float Sungrow::__swap_float(float i)
{
	uint32_t c = *(uint32_t *)&i;
	uint16_t ch = c >> 16;
	ch = __bswap_16(ch);
	uint16_t cl = c & 0xFFFFFFFF;
	cl = __bswap_16(cl);
	c = (ch << 16) | cl;
	return *(float *)&c;
}

bool Sungrow::load()
{
	status = true;
	if (readIreg(4999, (uint16_t *)&d, 43) != 0) // 5000  -- 45
	{
		status = false;
	}
	if (readIreg(5080, (uint16_t *)&d2.Work_state, 2) != 0 ||
		readIreg(5012, (uint16_t *)&d2.Daily_running_time, 25) != 0)
	{
		status = false;
	}
	if (readHreg(7012, (uint16_t *)&d3, 23) != 0)
	{
		status = false;
	}
	memset(SN, 0, sizeof(SN));
	if (readHreg(4989, (uint16_t *)&SN, 10) != 0)
	{
		// status = false;
	}
	return status;
}

String Sungrow::getString()
{
	String data = "";
	data += ",A0:" + String(to_u16(d.State));
	data += ",A1:" + String(to_u32((uint32_t *)&d2.Work_state));
	data += ",A2:" + String(to_u16(d.Fault));
	data += ",A3:" + String(to_u16(d.Device_type));

	data += ",A4:" + String(to_u16(d.DC_Voltage1) / (float)10);
	data += ",A5:" + String(to_u16(d.DC_current1) / (float)10);
	data += ",A6:" + String(to_u16(d.DC_Voltage2) / (float)10);
	data += ",A7:" + String(to_u16(d.DC_current2) / (float)10);
	data += ",A8:" + String(to_u16(d.DC_Voltage3) / (float)10);
	data += ",A9:" + String(to_u16(d.DC_current3) / (float)10);

	data += ",A10:" + String(to_u16(d2.DC_Voltage4) / (float)10);
	data += ",A11:" + String(to_u16(d2.DC_current4) / (float)10);
	data += ",A12:" + String(to_u16(d2.DC_Voltage5) / (float)10);
	data += ",A13:" + String(to_u16(d2.DC_current5) / (float)10);
	data += ",A14:" + String(to_u16(d2.DC_Voltage6) / (float)10);
	data += ",A15:" + String(to_u16(d2.DC_current6) / (float)10);

	data += ",A16:" + String(to_u16(d2.DC_Voltage7) / (float)10);
	data += ",A17:" + String(to_u16(d2.DC_current7) / (float)10);
	data += ",A18:" + String(to_u16(d2.DC_Voltage8) / (float)10);
	data += ",A19:" + String(to_u16(d2.DC_current8) / (float)10);
	data += ",A20:" + String(to_u16(d2.DC_Voltage9) / (float)10);
	data += ",A21:" + String(to_u16(d2.DC_current9) / (float)10);

	data += ",A22:" + String(to_u16(d2.DC_Voltage10) / (float)10);
	data += ",A23:" + String(to_u16(d2.DC_current10) / (float)10);
	data += ",A24:" + String(to_u16(d2.DC_Voltage11) / (float)10);
	data += ",A25:" + String(to_u16(d2.DC_current11) / (float)10);
	data += ",A26:" + String(to_u16(d2.DC_Voltage12) / (float)10);
	data += ",A27:" + String(to_u16(d2.DC_current12) / (float)10);

	data += ",A28:" + String(to_u16(d.phase_A_voltage) / (float)10);
	data += ",A29:" + String(to_u16(d.Phase_A_current) / (float)10);
	data += ",A30:" + String(to_u16(d.phase_B_voltage) / (float)10);
	data += ",A31:" + String(to_u16(d.Phase_B_current) / (float)10);
	data += ",A32:" + String(to_u16(d.phase_C_voltage) / (float)10);
	data += ",A33:" + String(to_u16(d.Phase_C_current) / (float)10);

	data += ",A34:" + String(to_u32((uint32_t *)&d.Total_active_power) / (float)1000);
	data += ",A35:" + String(((int32_t)__bswap_32(*(uint32_t *)(&d.Reactive_power))) / (float)1000);
	data += ",A36:" + String(to_u32((uint32_t *)&d.Total_Apparent_Power));
	data += ",A37:" + String(to_u16(d.Grid_frequency) / (float)10);
	data += ",A38:" + String(to_i16(d.Power_factor) / (float)1000);

	data += ",A39:" + String(to_u32((uint32_t *)&d.Total_power_yields));
	data += ",A40:" + String(to_u32((uint32_t *)&d.Total_running_time));
	data += ",A41:" + String(to_u16(d.Daily_power_yiels) / (float)10);
	data += ",A42:" + String(to_u16(d2.Daily_running_time)); // d2
	data += ",A43:" + String(to_u32((uint32_t *)&d.Total_DC_power) / (float)1000);
	data += ",A44:" + String(((int16_t)__bswap_16(d.Internal_temperature)) / (float)10);
	data += ",A45:" + String(to_u16(d2.Present_country)); // d2

	// String combiner
	data += ",A46:" + String(to_u16(d3.String_1_current) / (float)100);
	data += ",A47:" + String(to_u16(d3.String_2_current) / (float)100);
	data += ",A48:" + String(to_u16(d3.String_3_current) / (float)100);
	data += ",A49:" + String(to_u16(d3.String_4_current) / (float)100);
	data += ",A50:" + String(to_u16(d3.String_5_current) / (float)100);
	data += ",A51:" + String(to_u16(d3.String_6_current) / (float)100);
	data += ",A52:" + String(to_u16(d3.String_7_current) / (float)100);
	data += ",A53:" + String(to_u16(d3.String_8_current) / (float)100);
	data += ",A54:" + String(to_u16(d3.String_9_current) / (float)100);
	data += ",A55:" + String(to_u16(d3.String_10_current) / (float)100);
	data += ",A56:" + String(to_u16(d3.String_11_current) / (float)100);
	data += ",A57:" + String(to_u16(d3.String_12_current) / (float)100);

	data += ",A58:" + String(to_u16(d3.String_13_current) / (float)100);
	data += ",A59:" + String(to_u16(d3.String_14_current) / (float)100);
	data += ",A60:" + String(to_u16(d3.String_15_current) / (float)100);
	data += ",A61:" + String(to_u16(d3.String_16_current) / (float)100);
	data += ",A62:" + String(to_u16(d3.String_17_current) / (float)100);
	data += ",A63:" + String(to_u16(d3.String_18_current) / (float)100);
	data += ",A64:" + String(to_u16(d3.String_19_current) / (float)100);
	data += ",A65:" + String(to_u16(d3.String_20_current) / (float)100);
	data += ",A66:" + String(to_u16(d3.String_21_current) / (float)100);
	data += ",A67:" + String(to_u16(d3.String_22_current) / (float)100);
	data += ",A68:" + String(to_u16(d3.String_23_current) / (float)100);
	data += ",A69:" + String(to_u16(d3.String_24_current) / (float)100);

	char str[20];
	memset(str, 0, 20);
	memcpy(str, SN, 20);
	data += ",A70:\"" + String(str) + "\n";
	//api
	String res = "";
#if API_VERSION == 1

#elif API_VERSION == 0
	res += APIHeader();
	res += ",SungrowList:[{";
	res += "Id:" + String(local_id);
	if (!status == false)
	{
		res += ",Status:0";
	}
	else
	{
		res += ",Status:1";
		res += data;
	}
	res += "}]}";
#endif
	return res;
}
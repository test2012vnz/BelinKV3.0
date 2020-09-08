#include "Sungrow.h"

int Sungrow::id_count = 1;

float Sungrow::__swap_float(float i)
{
	uint32_t c = *(uint32_t*)&i;
	uint16_t ch = c >> 16;
	ch = __bswap_16(ch);
	uint16_t cl = c & 0xFFFFFFFF;
	cl = __bswap_16(cl);
	c = (ch << 16) | cl;
	return *(float*)&c;
}

bool Sungrow::load()
{
	status = false;
	if (readHreg(4999, (uint16_t*)&d, 43) == 0)		// 5000  -- 45
	{
		status = true;

	}
	if (readHreg(5080, (uint16_t*)&d2, 56) == 0)	
	{
		status = true;
	}
	if (readHreg(7012, (uint16_t*)&d3, 23) == 0)	
	{
		status = true;
	}
	return status;
}
#include "mb.h"

uint16_t MB::writeCoil(uint16_t offset, bool value, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->writeCoil(type.info.RTU.ID, offset, value, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->writeCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, cb);
    }
    break;
    }
    return 0xE5;
}

uint16_t MB::writeCoil(uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->writeCoil(type.info.RTU.ID, offset, value, numregs, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->writeCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
    }
    break;
    }
    return 0xE5;
}

uint16_t MB::writeHreg(uint16_t offset, uint16_t value, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->writeHreg(type.info.RTU.ID, offset, value, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->writeHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, cb);
    }
    break;
    }
    return 0xE5;
}
uint16_t MB::writeHreg(uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->writeHreg(type.info.RTU.ID, offset, value, numregs, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->writeHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
    }
    break;
    }
    return 0xE5;
}
// uint16_t MB::writeHreg_Sofar(uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
// {
//     switch (type.type)
//     {
//     case Device_Modbus_RTU:
//     case Device_Modbus_RTU_SunSpec:
//     {
//         if (rtu->element(type.info.RTU.ID) == NULL)
//         {
//             log_d("Element NULL");
//             return 0xE5;
//         }
//         rtu->element(type.info.RTU.ID)->rtu->rs->config(9600, SERIAL_8N1);
//         return rtu->element(type.info.RTU.ID)->rtu->writeHreg_Sofar(type.info.RTU.ID, offset, value, numregs, cb);
//     }
//     break;

//     case Device_Modbus_TCP:
//     case Device_Modbus_TCP_SunSpec:
//     {
//         // return mbIP->writeHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);;
//     }
//     break;
//     }
//     return 0xE5;
// }
uint16_t MB::readCoil(uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->readCoil(type.info.RTU.ID, offset, value, numregs, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->readCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
    }
    break;
    }
    return 0xE5;
}

uint16_t MB::readIsts(uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->readIsts(type.info.RTU.ID, offset, value, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->readIsts(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::readHreg(uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    switch (type.type)
    {
    case Device_Modbus_RTU:
    case Device_Modbus_RTU_SunSpec:
    {
        rs485->config(9600, SERIAL_8N1);
        return rtu->readHreg(type.info.RTU.ID, offset, value, numregs, cb);
    }
    break;

    case Device_Modbus_TCP:
    case Device_Modbus_TCP_SunSpec:
    {
        return mIP->readHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
    }
    break;
    }
    return 0xE5;
}
uint16_t MB::readIreg(uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->readIreg(type.info.RTU.ID, offset, value, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->readIreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, value, numregs, cb);
	}
	break;
	}
	return 0xE5;
}
uint16_t MB::pushCoil(uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pushCoil(type.info.RTU.ID, to, from, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pushCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, to, from, numregs, cb);
	}
	break;
	}
	return 0xE5;
}
uint16_t MB::pullCoil(uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullCoil(type.info.RTU.ID, from, to, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, from, to, numregs, cb);
	}
	break;
	}
	return 0xE5;
}
uint16_t MB::pullIsts(uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullIsts(type.info.RTU.ID, from, to, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullIsts(IPAddress(type.info.TCP.IP), type.info.TCP.ID, from, to, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pushHreg(uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pushHreg(type.info.RTU.ID, to, from, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pushHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, to, from, numregs, cb);
	}
	break;
	}
	return 0xE5;
}
uint16_t MB::pullHreg(uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullHreg(type.info.RTU.ID, from, to, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, from, to, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pullIreg(uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullIreg(type.info.RTU.ID, from, to, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullIreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, from, to, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pullHregToIreg(uint16_t offset, uint16_t startreg, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullHregToIreg(type.info.RTU.ID, offset, startreg, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullHregToIreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, startreg, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pullCoilToIsts(uint16_t offset, uint16_t startreg, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pullCoilToIsts(type.info.RTU.ID, offset, startreg, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pullCoilToIsts(IPAddress(type.info.TCP.IP), type.info.TCP.ID, offset, startreg, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pushIstsToCoil(uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pushIstsToCoil(type.info.RTU.ID, to, from, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pushIstsToCoil(IPAddress(type.info.TCP.IP), type.info.TCP.ID, to, from, numregs, cb);
	}
	break;
	}
	return 0xE5;
}

uint16_t MB::pushIregToHreg(uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
	switch (type.type)
	{
	case Device_Modbus_RTU:
	case Device_Modbus_RTU_SunSpec:
	{
		rs485->config(9600, SERIAL_8N1);
		return rtu->pushIregToHreg(type.info.RTU.ID, to, from, numregs, cb);
	}
	break;

	case Device_Modbus_TCP:
	case Device_Modbus_TCP_SunSpec:
	{
		return mIP->pushIregToHreg(IPAddress(type.info.TCP.IP), type.info.TCP.ID, to, from, numregs, cb);
	}
	break;
	}
	return 0xE5;
}
#ifndef _MB_H_
#define _MB_H_
#include "../../type.h"
#include "driver/connection/rs485/rs485.h"
#include "device_base.h"

class MB : public DeviceBase
{

public:
    MB(Device_Info_Structure t, RS485Driver *r) : DeviceBase(r)
    {
        type = t;
        rs485 = r;
    }
    ~MB()
    {
    }

    uint16_t writeCoil(uint16_t offset, bool value, cbTransaction cb = nullptr);
    uint16_t writeCoil(uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t writeHreg(uint16_t offset, uint16_t value, cbTransaction cb = nullptr);
    uint16_t writeHreg(uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    // uint16_t writeHreg_Sofar(uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t readCoil(uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIsts(uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readHreg(uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIreg(uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pushCoil(uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoil(uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIsts(uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushHreg(uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullHreg(uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIreg(uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pullHregToIreg(uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoilToIsts(uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIstsToCoil(uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIregToHreg(uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);

protected:
    RS485Driver *rs485 = nullptr;
    void tcp_disconect()
    {
        mIP->disconnect();
    }

private:
};

#endif
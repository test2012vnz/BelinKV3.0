#ifndef _MODBUSRTU_H_
#define _MODBUSRTU_H_

#include <byteswap.h>
#include "../Modbus/Modbus.h"

#define RTU_TIMEOUT 1000

#define RTU_MIN_PAYLOAD 0x0001
#define RTU_MAX_PAYLOAD 0x007D

class ModbusRTU : public Modbus
{
protected:
    enum
    {
        MASTER = 0,
        SLAVE,
    } MODE;

    std::vector<TTransaction> _trans;
    int16_t transactionId;
    int8_t n = -1;
    TTransaction *searchTransaction(uint16_t id);
    int8_t getFreeClient(); // Returns free slot position
    uint16_t send(uint8_t id, TAddress startreg, cbTransaction cb, void *data = nullptr);
    uint8_t receive(uint16_t id, uint16_t length);

    // uint8_t unitID;
    uint8_t slaveID;

public:
    HardwareSerial *rs; //
    ModbusRTU(HardwareSerial *r, uint8_t uID = 0)
    {
        begin(r, uID);
    }

    ModbusRTU()
    {
        ResCnt = 0;
    }

    ~ModbusRTU()
    {
    }

    void begin(HardwareSerial *r, uint8_t uID = 0)
    {
        rs = r;
        ResCnt = 0;
        slave(uID);
    }

    void slave(uint8_t id)
    {
        unitID = id;
    }

    void master(uint8_t id)
    {
        slaveID = id;
    }
    bool check_avilable()
    {
        if (rs == nullptr)
        {
            return false;
        }
        return true;
    }
    void task();
    void process();

    uint16_t writeCoil(uint16_t id, uint16_t offset, bool value, cbTransaction cb = nullptr);
    uint16_t writeCoil(uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t writeHreg(uint16_t id, uint16_t offset, uint16_t value, cbTransaction cb = nullptr);

    uint16_t writeHreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readCoil(uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIsts(uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readHreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pushCoil(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoil(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIsts(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushHreg(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullHreg(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIreg(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pullHregToIreg(uint16_t id, uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoilToIsts(uint16_t id, uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIstsToCoil(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIregToHreg(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);

    // custom write holding register for inverter
    uint16_t writeHreg_Sofar(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);

    bool isTransaction(uint16_t id)
    {
        return searchTransaction(id) != nullptr;
    }
    uint16_t transactions()
    {
        return _trans.capacity();
    }

private:
    byte lstResponse[300];
    int ResCnt = 0;
    unsigned long lastrecv;
    uint16_t crc16_update(uint16_t crc, uint8_t a)
    {
        int i;

        crc ^= (uint16_t)a;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }

        return crc;
    }

    uint16_t crc_modbus(uint8_t *input_str, size_t num_bytes)
    {
        uint16_t crc = 0xFFFF;
        for (int i = 0; i < num_bytes; i++)
        {
            crc = crc16_update(crc, input_str[i]);
        }

        return crc;
    }

    uint16_t check_length(FunctionCode fc, uint16_t numregs)
    {
        switch (fc)
        {
        case FC_READ_COILS:
        case FC_READ_INPUT_STAT:
            return 5 + (numregs / 8);

        case FC_READ_REGS:
        case FC_READ_INPUT_REGS:
            return 5 + (numregs * 2);

        case FC_WRITE_COIL:
        case FC_WRITE_REG:
        case FC_WRITE_COILS:
        case FC_WRITE_REGS:
            return 8;
        }
        return 0;
    }
};

#endif
#ifndef _MODBUSTCP_H_
#define _MODBUSTCP_H_

#include "../Modbus/Modbus.h"
#include <WiFi.h>
#include <byteswap.h>

#define MODBUSIP_PORT 502
#define MODBUSIP_MAXFRAME 1024
#define MODBUSIP_TIMEOUT 1000
#define MODBUSIP_UNIT mb_unitID
#define MODBUSIP_MAX_TRANSACIONS 16
#define MODBUSIP_MAX_CLIENTS 4
#define MODBUSIP_ADD_REG 1

typedef bool (*cbModbusConnect)(IPAddress ip);

typedef bool (*cbTransaction)(Modbus::ResultCode event, uint16_t transactionId, void *data);

class ModbusIP : public Modbus
{
protected:
    typedef union {
        struct
        {
            uint16_t transactionId;
            uint16_t protocolId;
            uint16_t length;
            uint8_t unitId;
        };
        uint8_t raw[7];
    } MBAP_t;
    MBAP_t _MBAP;
    uint8_t _replyMaster = 0;
    cbModbusConnect cbConnect = nullptr;
    cbModbusConnect cbDisconnect = nullptr;
    WiFiServer *server = nullptr;
    WiFiClient *client, serverclient;
    std::vector<TTransaction> _trans;
    int8_t n = -1;

    TTransaction *searchTransaction(uint16_t id);
    int16_t transactionId;
    void cleanup();         // Free clients if not connected and remove timedout transactions
    int8_t getFreeClient(); // Returns free slot position
    int8_t getSlave(IPAddress ip);
    int8_t getMaster(IPAddress ip);
    uint16_t send(IPAddress ip, uint16_t id, TAddress startreg, cbTransaction cb, void *data = nullptr);
    uint16_t receive(uint32_t timeout = 2);

public:
    ModbusIP();
    ~ModbusIP()
    {
        log_d("Modbus IP destructor");
        free(_frame);
    }
    uint16_t mb_unitID;
    bool isTransaction(uint16_t id);
    bool isConnected(IPAddress ip);
    bool connect(IPAddress ip);
    bool disconnect(); // Not implemented yet
    void slave();
    void master();
    void task();
    void process();
    void begin(); // Depricated
    uint16_t transactions();
    void onConnect(cbModbusConnect cb = nullptr);
    void onDisconnect(cbModbusConnect cb = nullptr);
    IPAddress eventSource();

    uint16_t writeCoil(IPAddress ip, uint16_t id, uint16_t offset, bool value, cbTransaction cb = nullptr);
    uint16_t writeHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t value, cbTransaction cb = nullptr);
    uint16_t writeCoil(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t writeHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readCoil(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIsts(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t readIreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pushCoil(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoil(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIsts(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushHreg(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullHreg(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullIreg(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs = 1, cbTransaction cb = nullptr);

    uint16_t pullHregToIreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pullCoilToIsts(IPAddress ip, uint16_t id, uint16_t offset, uint16_t startreg, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIstsToCoil(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);
    uint16_t pushIregToHreg(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs = 1, cbTransaction cb = nullptr);

private:

};
#endif
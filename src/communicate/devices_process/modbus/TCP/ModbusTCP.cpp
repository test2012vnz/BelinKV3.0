/*
* @Author: phaphh
* @Date:   2019-10-15 09:37:24
* @Last Modified by:   phaphh
* @Last Modified time: 2019-11-08 10:06:02
*/

#include "ModbusTCP.h"

ModbusIP mbIP_;

ModbusIP::ModbusIP()
{

    client = nullptr;
}

void ModbusIP::master()
{
}

void ModbusIP::slave()
{
    server = new WiFiServer(MODBUSIP_PORT);
    server->begin();
}

void ModbusIP::begin()
{
    slave();
}

bool ModbusIP::connect(IPAddress ip)
{
    cleanup();
    if (getSlave(ip) != -1)
    {
        return true;
        //  log_d("Get Slave OK");
    }
    else
    {
        // log_d("Get Slave False");
        // cleanup();
    }

    int8_t p = getFreeClient();
    // log_d("Free Client %d",p);
    if (p == -1)
        return false;
    client = new WiFiClient();
    if (client)
        Serial.println("Connect IP:" + String(client->remoteIP().toString()));
    bool res = client->connect(ip, MODBUSIP_PORT, 2000);
    // log_d("Result: %d",res);
    return res;
}

bool ModbusIP::disconnect()
{
    if (client)
    {
        // log_d("disconect");
        client->stop();
        delete client;
        client = nullptr;
    }
    return true;
}

IPAddress ModbusIP::eventSource() // Returns IP of current processing client query
{
    if (client)
        return client->remoteIP();
    return INADDR_NONE;
}

TTransaction *ModbusIP::searchTransaction(uint16_t id)
{
    //TTransaction tmp;
    //tmp.transactionId = id;
    //tmp.timestamp = 0;
    //tmp.cb = nullptr;
    //tmp._frame = nullptr;
    //std::vector<TTransaction>::iterator it = std::find(_trans.begin(), _trans.end(), tmp);
    std::vector<TTransaction>::iterator it = std::find_if(_trans.begin(), _trans.end(), [id](TTransaction &trans) {
        return trans.transactionId == id;
    });
    if (it != _trans.end())
        return &*it;
    return nullptr;
}

void ModbusIP::onConnect(cbModbusConnect cb)
{
    cbConnect = cb;
}

void ModbusIP::onDisconnect(cbModbusConnect cb)
{
    cbDisconnect = cb;
}

bool ifExpired(TTransaction &t)
{
    if (millis() - t.timestamp > MODBUSIP_TIMEOUT)
    {
        if (t.cb)
            t.cb(Modbus::EX_TIMEOUT, t.transactionId, nullptr);
        free(t._frame);
        return true;
    }
    return false;
}

void ModbusIP::cleanup() // Free clients if not connected and remove timedout transactions
{

    if (client && !client->connected())
    {
        IPAddress ip = client->remoteIP();
        delete client;
        client = nullptr;
        if (cbDisconnect && cbEnabled)
            cbDisconnect(ip);
    }
    _trans.erase(remove_if(_trans.begin(), _trans.end(), ifExpired), _trans.end());
}

int8_t ModbusIP::getFreeClient() // Returns free slot position
{
    //clientsCleanup();
    if (!client)
        return 1;
    return -1;
}

int8_t ModbusIP::getSlave(IPAddress ip)
{

    if (client && client->connected() && client->remoteIP() == ip && client->localPort() != MODBUSIP_PORT)
        return 1;
    return -1;
}

int8_t ModbusIP::getMaster(IPAddress ip)
{

    if (client && client->connected() && client->remoteIP() == ip && client->localPort() == MODBUSIP_PORT)
        return 1;
    return -1;
}

uint16_t ModbusIP::writeCoil(IPAddress ip, uint16_t id, uint16_t offset, bool value, cbTransaction cb)
{
    readSlave(offset, COIL_VAL(value), FC_WRITE_COIL);
    if (send(ip, id, COIL(offset), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::writeCoil(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    writeSlaveBits(COIL(offset), offset, numregs, FC_WRITE_COILS, value);
    if (send(ip, id, COIL(offset), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::readCoil(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    readSlave(offset, numregs, FC_READ_COILS);
    if (send(ip, id, COIL(offset), cb, value) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::writeHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t value, cbTransaction cb)
{
    readSlave(offset, value, FC_WRITE_REG);
    if (send(ip, id, HREG(offset), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::writeHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    writeSlaveWords(HREG(offset), offset, numregs, FC_WRITE_REGS, value);
    if (send(ip, id, HREG(offset), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::readHreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    readSlave(offset, numregs, FC_READ_REGS);
    if (send(ip, id, HREG(offset), cb, value) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::readIsts(IPAddress ip, uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    readSlave(offset, numregs, FC_READ_INPUT_STAT);
    if (send(ip, id, ISTS(offset), cb, value) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::readIreg(IPAddress ip, uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    readSlave(offset, numregs, FC_READ_INPUT_REGS);
    if (send(ip, id, IREG(offset), cb, value) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pushCoil(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    if (!searchRegister(COIL(from)))
        return false;
    if (numregs == 1)
    {
        readSlave(to, COIL_VAL(Coil(from)), FC_WRITE_COIL);
    }
    else
    {
        writeSlaveBits(COIL(from), to, numregs, FC_WRITE_COILS);
    }
    if (send(ip, id, COIL(from), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullCoil(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addCoil(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_COILS);
    if (send(ip, id, COIL(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullIsts(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIsts(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_INPUT_STAT);
    if (send(ip, id, ISTS(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pushHreg(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    if (!searchRegister(HREG(from)))
        return false;
    if (numregs == 1)
    {
        readSlave(to, Hreg(from), FC_WRITE_REG);
    }
    else
    {
        writeSlaveWords(HREG(from), to, numregs, FC_WRITE_REGS);
    }
    if (send(ip, id, HREG(from), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullHreg(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addHreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_REGS);
    if (send(ip, id, HREG(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullIreg(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_INPUT_REGS);
    if (send(ip, id, IREG(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pushIregToHreg(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    if (!searchRegister(IREG(from)))
        return false;
    if (numregs == 1)
    {
        readSlave(to, Ireg(from), FC_WRITE_REG);
    }
    else
    {
        writeSlaveWords(IREG(from), to, numregs, FC_WRITE_REGS);
    }
    if (send(ip, id, IREG(from), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pushIstsToCoil(IPAddress ip, uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
    if (!searchRegister(ISTS(from)))
        return false;
    if (numregs == 1)
    {
        readSlave(to, ISTS_VAL(Ists(from)), FC_WRITE_COIL);
    }
    else
    {
        writeSlaveBits(ISTS(from), to, numregs, FC_WRITE_COILS);
    }
    if (send(ip, id, ISTS(from), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullHregToIreg(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_REGS);
    if (send(ip, id, IREG(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

uint16_t ModbusIP::pullCoilToIsts(IPAddress ip, uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < 0x0001 || numregs > 0x007B)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIsts(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_COILS);
    if (send(ip, id, ISTS(to), cb) == 0)
    {
        return EX_CONNECTION_LOST;
    }
    return receive();
}

bool ModbusIP::isTransaction(uint16_t id) // Check if transaction is in progress (by ID)
{
    return searchTransaction(id) != nullptr;
}
bool ModbusIP::isConnected(IPAddress ip)
{
    int8_t p = getSlave(ip);
    return p != -1; // && client->connected();
}

uint16_t ModbusIP::transactions()
{
    return _trans.capacity();
}

void ModbusIP::task()
{
    cleanup();
    if (server)
    {
        while (server->hasClient())
        {
            WiFiClient *currentClient = new WiFiClient(server->available());
            if (currentClient == nullptr && !currentClient->connected())
                continue;
            if (cbConnect == nullptr || cbConnect(currentClient->remoteIP()))
            {
                n = getFreeClient();
                if (n > -1)
                {
                    client = currentClient;
                    continue; // while
                }
            }
            // Close connection if callback returns false or MODBUSIP_MAX_CLIENTS reached
            currentClient->flush();
            currentClient->stop();
            delete currentClient;
        }
    }
    for (n = 0; n < MODBUSIP_MAX_CLIENTS; n++)
    {
        if (client == nullptr)
            continue; // for (n)
        if (client->available() < sizeof(_MBAP) + 1)
            continue;
        client->readBytes(_MBAP.raw, sizeof(_MBAP.raw)); //Get MBAP
        _len = __bswap_16(_MBAP.length);
        _len--; // Do not count with last byte from MBAP

        if (__bswap_16(_MBAP.protocolId) != 0) //Check if MODBUSIP packet. __bswap is usless there.
        {
            client->flush();
            continue; // for (n)
        }
        if (_len > MODBUSIP_MAXFRAME) //Length is over MODBUSIP_MAXFRAME
        {
            exceptionResponse((FunctionCode)client->read(), EX_SLAVE_FAILURE);
            client->flush();
        }
        else
        {
            free(_frame);
            _frame = (uint8_t *)malloc(_len);
            if (!_frame)
            {
                exceptionResponse((FunctionCode)client->read(), EX_SLAVE_FAILURE);
                client->flush();
            }
            else
            {
                if (client->readBytes(_frame, _len) < _len) //Try to read MODBUS frame
                {
                    exceptionResponse((FunctionCode)_frame[0], EX_ILLEGAL_VALUE);
                    client->flush();
                }
                else
                {
                    // Serial.printf("Receive:");
                    // for(int i = 0; i < _len ; i++)
                    // {
                    //     Serial.printf("%#x ", _frame[i]);
                    // }
                    if (client->localPort() == MODBUSIP_PORT)
                    {

                        // Process incoming frame as slave
                        slavePDU(_frame);
                    }
                    else
                    {
                        // Process reply to master request
                        _reply = EX_SUCCESS;
                        TTransaction *trans = searchTransaction(__bswap_16(_MBAP.transactionId));
                        if (trans) // if valid transaction id
                        {
                            if ((_frame[0] & 0x7F) == trans->_frame[0]) // Check if function code the same as requested
                            {
                                // Procass incoming frame as master
                                masterPDU(_frame, trans->_frame, trans->startreg, trans->data);
                            }
                            else
                            {
                                _reply = EX_UNEXPECTED_RESPONSE;
                            }
                            if (cbEnabled && trans->cb)
                            {
                                trans->cb((ResultCode)_reply, trans->transactionId, nullptr);
                            }
                            free(trans->_frame);
                            //std::vector<TTransaction>::iterator it = std::find(_trans.begin(), _trans.end(), *trans);
                            _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
                            //_trans.erase(it);
                        }
                    }
                    client->flush(); // Not sure if we need flush rest of data available
                }
            }
        }
        if (client->localPort() != MODBUSIP_PORT)
            _reply = REPLY_OFF; // No replay if it was request to master
        if (_reply != REPLY_OFF)
        {
            _MBAP.length = __bswap_16(_len + 1); //_len+1 for last byte from MBAP
            size_t send_len = (uint16_t)_len + sizeof(_MBAP.raw);
            uint8_t sbuf[send_len];
            memcpy(sbuf, _MBAP.raw, sizeof(_MBAP.raw));
            memcpy(sbuf + sizeof(_MBAP.raw), _frame, _len);
            client->write(sbuf, send_len);
        }
        free(_frame);
        _frame = nullptr;
        _len = 0;
    }
    n = -1;
}

uint16_t ModbusIP::send(IPAddress ip, uint16_t id, TAddress startreg, cbTransaction cb, void *data) // Prepare and send ModbusIP frame. _frame buffer should be filled with Modbus data
{
    TTransaction *trans = searchTransaction(transactionId);
    if (trans)
    {
        free(trans->_frame);
        _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
    }

    if (!connect(ip))
    {
        return false;
    }
    // Serial.printf("Send:");
    MODBUSIP_UNIT = id;
    int8_t p = getSlave(ip);
    // if (p == -1 || !client->connected())
    //     return false;
    // Serial.println(ip);
    transactionId++;
    if (!transactionId)
        transactionId = 1;
    _MBAP.transactionId = __bswap_16(transactionId);
    _MBAP.protocolId = __bswap_16(0);
    _MBAP.length = __bswap_16(_len + 1); //_len+1 for last byte from MBAP
    _MBAP.unitId = MODBUSIP_UNIT;
    size_t send_len = _len + sizeof(_MBAP.raw);
    uint8_t sbuf[send_len];
    memcpy(sbuf, _MBAP.raw, sizeof(_MBAP.raw));
    memcpy(sbuf + sizeof(_MBAP.raw), _frame, _len);

    // for(int i = 0; i < send_len ; i++)
    // {
    //     Serial.printf("%#x ", sbuf[i]);
    // }
    client->flush();
    int l = client->write(sbuf, send_len);
    // log_d("Length %d - Length send %d!!!");
    if (l < send_len)
        return false;
    TTransaction tmp;
    tmp.transactionId = transactionId;
    tmp.timestamp = millis();
    tmp.cb = cb;
    tmp.data = data;
    tmp._frame = _frame;
    tmp.startreg = startreg;
    _trans.push_back(tmp);
    _frame = nullptr;
    return transactionId;
}

uint16_t ModbusIP::receive(uint32_t timeout)
{

    if (client == nullptr)
    {
        _reply = EX_CONNECTION_LOST;
        return _reply;
    }
    if (client->available() < 0)
    {
        _reply = EX_TIMEOUT;
        return _reply;
    }
    client->setTimeout(timeout);

    if (client->readBytes(_MBAP.raw, sizeof(_MBAP.raw)) < sizeof(_MBAP.raw))
    {
        _reply = EX_TIMEOUT;
        client->flush();
        return _reply;
    }

    _len = __bswap_16(_MBAP.length);
    _len--;                                // Do not count with last byte from MBAP
    if (__bswap_16(_MBAP.protocolId) != 0) //Check if MODBUSIP packet. __bswap is usless there.
    {
        client->flush();
        return receive(timeout);
    }
    if (_len > MODBUSIP_MAXFRAME)
    {
        exceptionResponse((FunctionCode)client->read(), EX_SLAVE_FAILURE);
        client->flush();
    }
    else
    {
        free(_frame);
        _frame = (uint8_t *)malloc(_len);
        if (!_frame)
        {
            exceptionResponse((FunctionCode)client->read(), EX_SLAVE_FAILURE);
            client->flush();
        }
        else
        {
            if (client->readBytes(_frame, _len) < _len) //Try to read MODBUS frame
            {
                exceptionResponse((FunctionCode)_frame[0], EX_ILLEGAL_VALUE);
                client->flush();
            }
            else
            {
                // for(int i=0;i<_len;i++){
                //     // if(_frame[i]!= 0){
                //         Serial.print(_frame[i]);
                //         Serial.print(" ");
                //     // }
                // }
                // Serial.println(" ");
                if (client->localPort() == MODBUSIP_PORT)
                {
                }
                else
                {
                    _reply = EX_SUCCESS;
                    TTransaction *trans = searchTransaction(__bswap_16(_MBAP.transactionId));
                    if (trans) // if valid transaction id
                    {
                        if ((_frame[0] & 0x7F) == trans->_frame[0]) // Check if function code the same as requested
                        {
                            // Procass incoming frame as master
                            masterPDU(_frame, trans->_frame, trans->startreg, trans->data);
                        }
                        else
                        {
                            _reply = EX_UNEXPECTED_RESPONSE;
                        }
                        if (cbEnabled && trans->cb)
                        {
                            trans->cb((ResultCode)_reply, trans->transactionId, nullptr);
                        }
                        free(trans->_frame);
                        //std::vector<TTransaction>::iterator it = std::find(_trans.begin(), _trans.end(), *trans);
                        _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
                        //_trans.erase(it);
                    }
                }
                client->flush(); // Not sure if we need flush rest of data available
            }
        }
        free(_frame);
        _frame = nullptr;
        _len = 0;
    }
    return _reply;
}

void ModbusIP::process()
{
    static unsigned long lastt = millis();
    if ((millis() - lastt) > 2000)
    {
        lastt = millis();
    }
    if (server)
    {
        while (server->hasClient())
        {
            serverclient = server->available();
            while (serverclient.connected())
            {
                serverclient.setTimeout(2);
                memset(_MBAP.raw, 0, sizeof(_MBAP.raw));
                int length = serverclient.readBytes(_MBAP.raw, sizeof(_MBAP.raw));
                // Serial.println("receive:" + String(length));
                if (length < sizeof(_MBAP.raw))
                {
                    // Serial.printf("length err");
                    // for(int i = 0; i < length; i++)
                    // {
                    //     Serial.printf("%#x ", _MBAP.raw[i]);
                    // }
                    _replyMaster = EX_TIMEOUT;
                    serverclient.flush();
                    serverclient.stop();
                    break;
                }
                // Serial.println("receive:");
                // for(int i = 0; i < sizeof(_MBAP.raw); i++)
                // {
                //     Serial.printf("%#x ", _MBAP.raw[i]);
                // }
                _len = __bswap_16(_MBAP.length);
                _len--; // Do not count with last byte from MBAP

                if (__bswap_16(_MBAP.protocolId) != 0) //Check if MODBUSIP packet. __bswap is usless there.
                {
                    serverclient.flush();
                    serverclient.stop();
                    break;
                }
                if (_len > MODBUSIP_MAXFRAME)
                {
                    exceptionResponse((FunctionCode)serverclient.read(), EX_SLAVE_FAILURE);
                    serverclient.flush();
                }
                else
                {
                    free(_frame);
                    _frame = (uint8_t *)malloc(_len);
                    if (serverclient.readBytes(_frame, _len) < _len) //Try to read MODBUS frame
                    {
                        exceptionResponse((FunctionCode)_frame[0], EX_ILLEGAL_VALUE);
                        serverclient.flush();
                    }
                    else
                    {
                        if (serverclient.localPort() == MODBUSIP_PORT)
                        {

                            // Process incoming frame as slave
                            slavePDU(_frame);
                        }
                        serverclient.flush(); // Not sure if we need flush rest of data available
                    }
                }
                if (serverclient.localPort() != MODBUSIP_PORT)
                    _reply = REPLY_OFF; // No replay if it was request to master
                if (_reply != REPLY_OFF)
                {
                    _MBAP.length = __bswap_16(_len + 1); //_len+1 for last byte from MBAP
                    size_t send_len = (uint16_t)_len + sizeof(_MBAP.raw);
                    uint8_t sbuf[send_len];
                    memcpy(sbuf, _MBAP.raw, sizeof(_MBAP.raw));
                    memcpy(sbuf + sizeof(_MBAP.raw), _frame, _len);

                    serverclient.write(sbuf, send_len);
                    serverclient.flush();
                }
                free(_frame);
                _frame = nullptr;
                _len = 0;
            }

            serverclient.stop();
        }
    }
}
/*
* @Author: phaphh
* @Date:   2019-10-11 11:34:22
* @Last Modified by:   phaphh
* @Last Modified time: 2019-11-27 17:11:46
*/

#include "ModbusRTU.h"

TTransaction *ModbusRTU::searchTransaction(uint16_t id)
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

uint16_t ModbusRTU::send(uint8_t id, TAddress startreg, cbTransaction cb, void *data)
{

    master(id);
    size_t send_len = _len + sizeof(unitID);
    uint8_t sbuf[send_len + 2];
    sbuf[unitID] = id;
    memcpy(sbuf + sizeof(unitID), _frame, _len);
    uint16_t crc = crc_modbus(sbuf, send_len);
    // log_d("CRC send %x, sizeof(uwriteHreg_SofarnitID): %d", crc, sizeof(unitID));
    memcpy(sbuf + send_len, &crc, 2);
    transactionId++;
    if (!transactionId)
        transactionId = 1;

    // Serial.println("\n-------------");
    // for(int i = 0; i < _len; i++)
    // {
    //     Serial.printf("%X ", _frame[i]);
    // }
    // Serial.println("\n");
    // for(int i = 0; i < send_len + 2; i++)
    // {
    //     Serial.printf("%X ", sbuf[i]);
    // }
    // Serial.println("\n-------------");

    // log_d("Send len: %d", send_len + 2);

    if (rs->write(sbuf, send_len + 2) != (send_len + 2))
    {
        return false;
    }

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

uint8_t ModbusRTU::receive(uint16_t id, uint16_t length)
{
    free(_frame);
    uint8_t *rec = nullptr;
    if (length != 0)
    {
        _frame = (uint8_t *)malloc(length - 3);
        rec = (uint8_t *)malloc(length);
        rs->setTimeout(2000);
        delay(10);
        uint16_t lengthrec = rs->readBytes(rec, length);

        if (lengthrec < length)
        {
            TTransaction *trans = searchTransaction(transactionId);
            if (trans)
            {
                free(trans->_frame);
                _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
            }
            exceptionResponse((FunctionCode)_frame[0], EX_ILLEGAL_VALUE);
            free(rec);
            return _reply;
        }
        else
        {
            uint16_t crc;
            uint16_t crc_check = crc_modbus(rec, length - 2);
            memcpy(&crc, rec + (length - 2), 2);
            memcpy(_frame, rec + 1, length - 3);
            if (crc == crc_check)
            {
                // for(int i = 0; i < length - 3; i++)
                // {
                //     Serial.printf("%#x ", _frame[i]);
                // }
                _reply = EX_SUCCESS;
                TTransaction *trans = searchTransaction(transactionId);
                if (trans)
                {
                    if ((_frame[0] & 0x7F) == trans->_frame[0])
                    {
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
                    _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
                }
            }
        }
    }
    TTransaction *trans = searchTransaction(transactionId);
    if (trans)
    {
        free(trans->_frame);
        _trans.erase(std::remove(_trans.begin(), _trans.end(), *trans), _trans.end());
    }
    free(rec);
    return _reply;
}

void ModbusRTU::task()
{
    static uint16_t s = 0;
    // Serial.printf("Send: %d \r\n",(uint16_t)s[0]);
    if (writeHreg(1, 8, s++) == 0)
    {
        log_d("Write Hreg OK");
    }
    else
    {
        log_d("Write Hreg ERROR :%d", _reply);
    }
    uint16_t r[20];
    // delay(1000);
    if (readHreg(1, 8, r, 1) == 0)
    {
        log_d("Hreg 18: %d", __bswap_16(r[0]));
    }
    else
    {
        log_d("Hreg err 18: %d", r[0]);
    }
}

void ModbusRTU::process()
{
    bool bvalid = true;
    uint8_t Slave;
    _reply = REPLY_OFF;
    while (rs->available() > 0)
    {
        uint8_t d = rs->read();
        // log_d("char receive: %#x", d);
        lstResponse[ResCnt++] = d;
        if (ResCnt >= 4)
        {
            Slave = lstResponse[0];
            if (Slave == unitID)
            {
                // log_d("Modbus ID: %d", Slave);
                uint8_t Function = lstResponse[1];
                uint16_t Address = (lstResponse[2] << 8) | lstResponse[3];
                switch (Function)
                {
                case FC_READ_COILS:
                case FC_READ_INPUT_STAT:
                case FC_READ_REGS:
                case FC_READ_INPUT_REGS:
                case FC_WRITE_COIL:
                case FC_WRITE_REG:
                {
                    if (ResCnt >= 8)
                    {
                        uint16_t Length = (lstResponse[4] << 8) | lstResponse[5];
                        uint16_t crc_check = crc_modbus(lstResponse, 6);
                        // log_d("CRC  check: %x , %d ", crc_check, Length);
                        uint16_t crc = (lstResponse[7] << 8) | lstResponse[6];
                        // log_d("CRC  receive: %x", crc);
                        if (Length > 0 && crc_check == crc)
                        {
                            free(_frame);
                            _frame = (uint8_t *)malloc(5);
                            memcpy(_frame, &lstResponse[1], 5);
                            _len = 5;
                            slavePDU(_frame);
                            // break;
                            // Serial.println("Frame Send ");
                            // for (int i = 0; i < _len; i++)
                            // {
                            //     Serial.printf("%#x ", _frame[i]);
                            // }
                            // Serial.println("\n");
                        }
                        else
                        {
                            bvalid = false;
                            break;
                        }
                    }
                }
                break;
                case FC_WRITE_COILS:
                case FC_WRITE_REGS:
                {
                    if (ResCnt >= 7)
                    {
                        uint16_t Length = (lstResponse[4] << 8) | lstResponse[5];
                        uint8_t ByteCount = lstResponse[6];
                        if (ResCnt >= 9 + ByteCount)
                        {
                            uint16_t crc_check = crc_modbus(lstResponse, 7 + ByteCount);
                            uint16_t crc = (lstResponse[8 + ByteCount] << 8) | lstResponse[7 + ByteCount];
                            if (crc_check == crc)
                            {
                                free(_frame);
                                _frame = (uint8_t *)malloc(6 + ByteCount);
                                _len = 6 + ByteCount;
                                memcpy(_frame, &lstResponse[1], 6 + ByteCount);
                                slavePDU(_frame);
                                break;
                                // for(int i = 0; i < _len; i++)
                                // {
                                //     Serial.printf("%#x ", _frame[i]);
                                // }
                            }
                            else
                            {
                                bvalid = false;
                                break;
                            }
                        }
                    }
                }
                break;
                }
            }
        }
        lastrecv = millis();
    }
    if (_reply != REPLY_OFF)
    {

        size_t send_len = (uint16_t)_len + 1;
        uint8_t sbuf[send_len + 2];
        sbuf[0] = Slave;
        memcpy(sbuf + 1, _frame, _len);
        uint16_t crc = crc_modbus(sbuf, send_len);
        memcpy(sbuf + send_len, &crc, 2);
        log_d("RTU: Frame send: %d", send_len);
        if (rs->write(sbuf, send_len + 2) != (send_len + 2))
        {
            log_d("Reply Fails....");
        }
        ResCnt = 0;
    }
    if (!bvalid && ResCnt > 0)
    {
        ResCnt = 0;
    }
    if (ResCnt > 0 && (millis() - lastrecv > RTU_TIMEOUT || millis() < lastrecv))
        ResCnt = 0;
}

uint16_t ModbusRTU::writeCoil(uint16_t id, uint16_t offset, bool value, cbTransaction cb)
{
    readSlave(offset, COIL_VAL(value), FC_WRITE_COIL);
    send(id, COIL(offset), cb);
    return receive(id, check_length(FC_WRITE_COIL, 1));
}

uint16_t ModbusRTU::writeCoil(uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    writeSlaveBits(COIL(offset), offset, numregs, FC_WRITE_COILS, value);
    send(id, COIL(offset), cb);
    return receive(id, check_length(FC_WRITE_COILS, numregs));
}

uint16_t ModbusRTU::readCoil(uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    readSlave(offset, numregs, FC_READ_COILS);
    send(id, COIL(offset), cb, value);
    return receive(id, check_length(FC_READ_COILS, numregs));
}

uint16_t ModbusRTU::writeHreg(uint16_t id, uint16_t offset, uint16_t value, cbTransaction cb)
{
    readSlave(offset, value, FC_WRITE_REG);
    send(id, HREG(offset), cb);
    return receive(id, check_length(FC_WRITE_REG, 1));
}

uint16_t ModbusRTU::writeHreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    writeSlaveWords(HREG(offset), offset, numregs, FC_WRITE_REGS, value);
    send(id, HREG(offset), cb);
    return receive(id, check_length(FC_WRITE_REGS, numregs));
}

uint16_t ModbusRTU::writeHreg_Sofar(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{

    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;

    free(_frame);
    _len = 5 + 2 * numregs;
    _frame = (uint8_t *)malloc(_len);
    _frame[0] = 0x13;
    _frame[1] = offset >> 8;
    _frame[2] = offset & 0x00FF;
    _frame[3] = numregs >> 8;
    _frame[4] = numregs & 0x00FF;
    memcpy(_frame + 5, value, numregs * 2);
    send(id, HREG(offset), cb);
    return receive(id, check_length((FunctionCode)0x13, numregs));
}

uint16_t ModbusRTU::readHreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    readSlave(offset, numregs, FC_READ_REGS);
    send(id, HREG(offset), cb, value);
    return receive(id, check_length(FC_READ_REGS, numregs));
}

uint16_t ModbusRTU::readIsts(uint16_t id, uint16_t offset, bool *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    readSlave(offset, numregs, FC_READ_INPUT_REGS);
    send(id, ISTS(offset), cb, value);
    return receive(id, check_length(FC_READ_INPUT_REGS, numregs));
}

uint16_t ModbusRTU::readIreg(uint16_t id, uint16_t offset, uint16_t *value, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
    readSlave(offset, numregs, FC_READ_INPUT_REGS);
    send(id, IREG(offset), cb, value);
    return receive(id, check_length(FC_READ_INPUT_REGS, numregs));
}

uint16_t ModbusRTU::pushCoil(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
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
    send(id, COIL(from), cb);
    return receive(id, check_length(FC_WRITE_COIL, 1));
}

uint16_t ModbusRTU::pullCoil(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addCoil(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_COILS);
    send(id, COIL(to), cb);
    return receive(id, check_length(FC_READ_COILS, numregs));
}

uint16_t ModbusRTU::pullIsts(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIsts(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_INPUT_STAT);
    send(id, ISTS(to), cb);
    return receive(id, check_length(FC_READ_INPUT_STAT, numregs));
}

uint16_t ModbusRTU::pushHreg(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
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
    send(id, HREG(from), cb);
    return receive(id, check_length(FC_WRITE_REGS, numregs));
}

uint16_t ModbusRTU::pullHreg(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addHreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_REGS);
    send(id, HREG(to), cb);
    return receive(id, check_length(FC_READ_REGS, numregs));
}

uint16_t ModbusRTU::pullIreg(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_INPUT_REGS);
    send(id, IREG(to), cb);
    return receive(id, check_length(FC_READ_INPUT_REGS, numregs));
}

uint16_t ModbusRTU::pushIregToHreg(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
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
    send(id, IREG(from), cb);
    return receive(id, check_length(FC_WRITE_REGS, numregs));
}

uint16_t ModbusRTU::pushIstsToCoil(uint16_t id, uint16_t to, uint16_t from, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
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
    send(id, ISTS(from), cb);
    return receive(id, check_length(FC_WRITE_COILS, numregs));
}

uint16_t ModbusRTU::pullHregToIreg(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIreg(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_REGS);
    send(id, IREG(to), cb);
    return receive(id, check_length(FC_READ_REGS, numregs));
}

uint16_t ModbusRTU::pullCoilToIsts(uint16_t id, uint16_t from, uint16_t to, uint16_t numregs, cbTransaction cb)
{
    if (numregs < RTU_MIN_PAYLOAD || numregs > RTU_MAX_PAYLOAD)
        return false;
#ifdef MODBUSIP_ADD_REG
    addIsts(to, numregs);
#endif
    readSlave(from, numregs, FC_READ_COILS);
    send(id, ISTS(to), cb);
    return receive(id, check_length(FC_READ_COILS, numregs));
}

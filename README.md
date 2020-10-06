# BelinKV3.0

-- Current Dev Version: 3.0.1 Beta

# Note 
GSM Max length: 1460

# Change updateBaudRate in HardwareSerial.h and uartSetBaudRate in esp32-hal-uart.c
```
void uartSetBaudRate(uart_t* uart, uint32_t baud_rate, uint32_t config)
{
    if(uart == NULL) {
        return;
    }
    UART_MUTEX_LOCK();
    uint32_t clk_div = ((getApbFrequency()<<4)/baud_rate);
    uart->dev->conf0.val = config;                          // add for config
    uart->dev->clk_div.div_int = clk_div>>4 ;
    uart->dev->clk_div.div_frag = clk_div & 0xf;
    UART_MUTEX_UNLOCK();
}
```
# 28-7-2020 Change GSM Client Timeout in cHttpClient.h
```
static const int kHttpResponseTimeout = 30*1000 to 5*1000
```

# 30-7-2020 Change Frame RTU Max length
from 0x007B to 0x007D

# 5-8-2020 Remove lenght limit at 1460 bytes for GSM

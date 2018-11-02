#include "SPI.h"
#include <Arduino.h>

SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_ss(-1)
    ,_simpleio((uint32_t *)0xFFFFFF10)
    ,_flash_spi((uint16_t *)0xFFFFFB40)
    ,_sd_spi((uint16_t *)0xFFFFFB50)
    ,_oled_spi((uint16_t *)0xFFFFFB60)
    ,_ext_spi((uint16_t *)0xFFFFFB70)
{}

void SPIClass::begin(int8_t ss)
{
    Serial.println("Initializing...");

    if(_spi) {
        return;
    }

    switch(_spi_num) {
        case FSPI:
            _spi = _flash_spi;
            break;
        case SDSPI:
            _spi = _sd_spi;
            break;
        case LEDSPI:
            _spi = _oled_spi;
            break;
        default:
            _spi = _ext_spi;
            break;
    }
    if(!_spi) {
        return;
    }

    ((uint8_t *)_spi)[1] = 0x19;

    _ss = ss;

    set_pin(11);
    set_pin(10);
    delay(20);
    unset_pin(10);
    delay(20);
    set_pin(10);
    delay(100);
    
    Serial.print("Initialized: ");
    Serial.println((unsigned int)_flash_spi, HEX);
}

uint8_t SPIClass::beginTransaction(void)
{
    Serial.println("Starting transaction");

    uint32_t in;
    ((uint8_t *)_spi)[1] = 0x80;
    do {
        in = *_spi;
    } while ((in & SPI_READY_MASK) == 0);

    Serial.println("Started transaction");

    #if (_BYTE_ORDER == _LITTLE_ENDIAN)
        return (in & 0xff);
    #else
        return (in >> 24);
    #endif
}

uint8_t SPIClass::transfer(uint8_t data)
{
    Serial.println("Starting transfer");

    uint32_t in;

    *(uint8_t *)_spi = data;
    do {
        in = *_spi;
    } while ((in & SPI_READY_MASK) == 0);

    Serial.println("Transfer done");

    #if (_BYTE_ORDER == _LITTLE_ENDIAN)
        return (in & 0xff);
    #else
        return (in >> 24);
    #endif
}

void SPIClass::set_pin(uint8_t pin)
{
    *_simpleio |= (1<<pin);
}


void SPIClass::unset_pin(uint8_t pin)
{
    *_simpleio &= ~(1<<pin);
}

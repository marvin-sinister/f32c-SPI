#include "SPI.h"
#include <Arduino.h>

SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_inTransaction(false)
    ,_ss(-1)
    ,_simpleio((uint32_t *)0xFFFFFF10)
    ,_flash_spi((uint16_t *)0xFFFFFB40)
    ,_sd_spi((uint16_t *)0xFFFFFB50)
    ,_oled_spi((uint16_t *)0xFFFFFB60)
    ,_ext_spi((uint16_t *)0xFFFFFB70)
{}

void SPIClass::begin(int8_t ss)
{
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
}

uint8_t SPIClass::setSpeed(uint8_t _speed)
{
    uint32_t in;
    ((uint8_t *)_spi)[1] = _speed;
    do {
        in = *_spi;
    } while ((in & SPI_READY_MASK) == 0);

    #if (_BYTE_ORDER == _LITTLE_ENDIAN)
        return (in & 0xff);
    #else
        return (in >> 24);
    #endif
}

uint8_t SPIClass::transfer(uint8_t _data)
{
    uint32_t in;

    ((uint8_t *)_spi)[0] = _data;
    do {
        in = *_spi;
        NOP;
    } while ((in & SPI_READY_MASK) == 0);

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

void SPIClass::beginTransaction(SPISettings settings) {
    beginTransaction();
}

void SPIClass::beginTransaction() {
    if (_inTransaction) {
        return;
    }
    _unset_pin(_ss);
    _inTransaction = true;
}

void SPIClass::endTransaction() {
    if (!_inTransaction) {
        return;
    }
    _set_pin(_ss);
    _inTransaction = false;
}


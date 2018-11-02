#include "SPI.h"
#include <Arduino.h>

SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_inTransaction(false)
    ,_ss(-1)
{}

void SPIClass::begin(int8_t ss)
{
    if(_spi) {
        return;
    }

    _spi = (uint16_t *)(0xFFFFFB40 + (_spi_num * 0x10));

    if(!_spi) {
        return;
    }

    setClock(5000000);
    _ss = ss;
}

uint8_t SPIClass::setClock(uint32_t _clock)
{
    uint8_t clock = (uint8_t)((_clock/100000000.0f)*256);
    uint32_t in;
    ((uint8_t *)_spi)[1] = clock;
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
    *((uint32_t *)0xFFFFFF10) |= (1<<pin);
}


void SPIClass::unset_pin(uint8_t pin)
{
    *((uint32_t *)0xFFFFFF10) &= ~(1<<pin);
}

void SPIClass::beginTransaction(SPISettings settings) {
    setClock(settings._clock);
    beginTransaction();
}

void SPIClass::beginTransaction() {
    if (_inTransaction) {
        return;
    }
    unset_pin(_ss);
    _inTransaction = true;
    NOP;
}

void SPIClass::endTransaction() {
    if (!_inTransaction) {
        return;
    }
    set_pin(_ss);
    _inTransaction = false;
}


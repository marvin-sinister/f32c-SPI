#include "SPI.h"
#include <Arduino.h>

SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_ss(-1)
    ,_freq(5000000)
    ,_inTransaction(false)
{}

void SPIClass::begin(int8_t ss)
{
    if(_spi) {
        return;
    }

    _ss = ss;

    _spi = (uint16_t *)(0xFFFFFB40 + (_spi_num * 0x10));

    if(!_spi) {
        return;
    }

    ((uint8_t *)_spi)[1] = 0x19;
}

void SPIClass::end() {
}

void SPIClass::setClock(uint32_t clock)
{
    if (clock == _freq) {
        return;
    }
    _freq = clock;
    uint8_t flag = (uint8_t)((clock/100000000.0f)*256);
    ((uint8_t *)_spi)[1] = flag;
    NOP;
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
}

void SPIClass::setDataMode(uint8_t dataMode) {
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
    if (_spi_num != ESPI) {
        *((uint32_t *)0xFFFFFF10) |= (1<<pin);
    }
    else {
        digitalWrite(pin, LOW);
    }
}


void SPIClass::unset_pin(uint8_t pin)
{
    if (_spi_num != ESPI) {
        *((uint32_t *)0xFFFFFF10) &= ~(1<<pin);
    }
    else {
        digitalWrite(pin, HIGH);
    }
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


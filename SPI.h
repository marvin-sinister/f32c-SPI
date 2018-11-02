#ifndef _SPI_H
#define _SPI_H

#include <stdlib.h>

#define NOP __asm__ __volatile__ ("nop\n\t")

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define  SPI_READY_MASK (1 << 8)
#else
#define SPI_READY_MASK (1 << 16)
#endif

#define SPI_LSBFIRST 0
#define SPI_MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define ESPI 3
#define FSPI 0
#define SDSPI 1
#define LEDSPI 2

class SPISettings
{
public:
    SPISettings() :_clock(1000000), _bitOrder(SPI_MSBFIRST), _dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) :_clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
    uint32_t _clock;
    uint8_t  _bitOrder;
    uint8_t  _dataMode;
};

class SPIClass
{
private:
    int8_t _spi_num;
    uint16_t * _spi;
    bool _inTransaction;
    int8_t _ss;
    void set_pin(uint8_t pin);
    void unset_pin(uint8_t pin);

public:
    SPIClass(uint8_t spi_bus=ESPI);
    void begin(int8_t ss=-1);

    uint8_t setClock(uint32_t _clock);

    uint8_t transfer(uint8_t _data);
    void beginTransaction();
    void beginTransaction(SPISettings settings);
    void endTransaction();
};

extern SPIClass SPI;

#endif // _SPI_H

#ifndef _SPI_H
#define _SPI_H

#include <stdlib.h>

#define NOP __asm__ __volatile__ ("nop\n\t")

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define  SPI_READY_MASK (1 << 8)
#else
#define SPI_READY_MASK (1 << 16)
#endif

#define ESPI 0
#define FSPI 1
#define SDSPI 2
#define LEDSPI 3

class SPIClass
{
private:
    int8_t _spi_num;
    uint16_t * _spi;
    int8_t _ss;
    uint32_t * _simpleio;
    uint16_t * _flash_spi;
    uint16_t * _sd_spi;
    uint16_t * _oled_spi;
    uint16_t * _ext_spi;
    void set_pin(uint8_t pin);
    void unset_pin(uint8_t pin);

public:
    SPIClass(uint8_t spi_bus=ESPI);
    void begin(int8_t ss=-1);

    uint8_t setSpeed(uint8_t _speed);

    uint8_t transfer(uint8_t _data);
};

extern SPIClass SPI;

#endif // _SPI_H

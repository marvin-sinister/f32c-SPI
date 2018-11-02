#define LED_RED 8
#define LED_GREEN 10


#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define  SPI_READY_MASK (1 << 8)
#else
#define SPI_READY_MASK (1 << 16)
#endif

uint32_t *simpleio  = (uint32_t *)0xFFFFFF10;
uint16_t *flash_spi = (uint16_t *)0xFFFFFB40;
uint16_t *sd_spi    = (uint16_t *)0xFFFFFB50;
uint16_t *oled_spi  = (uint16_t *)0xFFFFFB60;
uint16_t *ext_spi   = (uint16_t *)0xFFFFFB70;

uint16_t *xspi;

uint8_t spi_start_tx(volatile uint16_t *port)
{
  uint32_t in;
  ((uint8_t *)port)[1] = 0x80;
  do {
    in = *port;
  } while ((in & SPI_READY_MASK) == 0);

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
  return (in & 0xff);
#else
  return (in >> 24);
#endif
}


uint8_t spi_rxtx(volatile uint16_t *port, uint8_t _data)
{
  uint32_t in;

  *(uint8_t *)port = _data;
  do {
    in = *port;
  } while ((in & SPI_READY_MASK) == 0);

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
  return (in & 0xff);
#else
  return (in >> 24);
#endif
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup");
  ((uint8_t *)ext_spi)[1] = 0x19;
}

uint8_t flash_id(void)
{
  xspi = flash_spi;
  spi_start_tx(xspi);
  spi_rxtx(xspi, 0xAB);
  spi_rxtx(xspi, 0);
  spi_rxtx(xspi, 0);
  spi_rxtx(xspi, 0);
  return spi_rxtx(xspi, 0);
}

void loop(void)
{
  Serial.println("##############################");
  delay(500);
  Serial.print("FLASH ID: 0x");
  Serial.println(flash_id(), HEX);
  Serial.println("##############################");
  delay(5000);
}

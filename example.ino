static const uint8_t SX1276_REG_VERSION = 0x42;
static const uint8_t SX1276_REG_TEMP = 0x3C;
static const uint8_t SX1276_REG_LOW_BAT = 0x3D;
static const uint8_t SX1276_REG_OP_MODE = 0x01;
static const uint8_t SX1276_REG_OSC = 0x24;
static const uint8_t RF_OPMODE_MASK = 0xF8;
static const uint8_t RF_OPMODE_SLEEP = 0x00;
static const uint8_t RF_OPMODE_STANDBY = 0x00;

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define	SPI_READY_MASK (1 << 8)
#else
#define	SPI_READY_MASK (1 << 16)
#endif

volatile uint32_t *simpleio  = (uint32_t *)0xFFFFFF10;
volatile uint16_t *flash_spi = (uint16_t *)0xFFFFFB40;
volatile uint16_t *sd_spi    = (uint16_t *)0xFFFFFB50;
volatile uint16_t *oled_spi  = (uint16_t *)0xFFFFFB60;
volatile uint16_t *ext_spi   = (uint16_t *)0xFFFFFB70;

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

void dc(uint8_t state)
{
  if(state)
    *simpleio |= (1<<11);
  else
    *simpleio &= ~(1<<11);
}

void resn(uint8_t state)
{
  if(state)
    *simpleio |= (1<<10);
  else
    *simpleio &= ~(1<<10);
}

void reset_sequence()
{
  dc(1); // data
  resn(1);
  delay(20);
  resn(0);
  delay(20);
  resn(1);
  delay(100);
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup");
  ((uint8_t *)ext_spi)[1] = 0x19;
  reset_sequence();
}

byte writeRegister(byte address, byte value)
{
    dc(0); // commands
    delay(1);
    bitClear(address, 7);    // Bit 7 cleared to write in registers
    // send in the address and value via SPI:
    spi_rxtx(ext_spi, address | 0x80);
    // write the value
    spi_rxtx(ext_spi, value);
    dc(1); // commands
    Serial.print(F("ULX3S FPGA writing SX1276 register 0x"));
    Serial.print(address, HEX);
    Serial.print(F(": 0x"));
    Serial.print(value, HEX);
    Serial.println();
}

byte readRegister(byte address)
{
    byte value = 0x00;
    dc(0); // commands
    delay(1);
    bitClear(address, 7);    // Bit 7 cleared to write in registers
    spi_rxtx(ext_spi, address);
    value = spi_rxtx(ext_spi, 0x00);
    dc(1); // commands
     //   Serial.print(F("## Reading:  ##\t"));
    Serial.print(F("ULX3S FPGA Reading SX1276 register 0x"));
    Serial.print(address, HEX);
    Serial.print(F(": 0x"));
    Serial.print(value, HEX);
    Serial.println();
    return value;
}

void loop(void)
{
  Serial.println("##############################");
  readRegister(SX1276_REG_VERSION);
  readRegister(SX1276_REG_LOW_BAT);
  readRegister(SX1276_REG_TEMP);
  writeRegister(SX1276_REG_OP_MODE, (readRegister(SX1276_REG_OP_MODE) & RF_OPMODE_MASK) | RF_OPMODE_STANDBY);
  Serial.println("##############################");
  delay(5000);
}

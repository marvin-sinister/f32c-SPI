 #include <SPI.h>

static const uint8_t SX1276_REG_VERSION = 0x42;
static const uint8_t SX1276_REG_TEMP = 0x3C;
static const uint8_t SX1276_REG_LOW_BAT = 0x3D;
static const uint8_t SX1276_RG_OP_MODE = 0x01;
static const uint8_t SX1276_REG_OSC = 0x24;
static const uint8_t RF_OPMODE_MASK = 0xF8;
static const uint8_t RF_OPMODE_SLEEP = 0x00;
static const uint8_t RF_OPMODE_STANDBY = 0x00;

volatile uint32_t *simpleio  = (uint32_t *)0xFFFFFF10;

SPIClass SPI(0);

void set_pin(uint8_t pin)
{
    *simpleio |= (1<<pin);
}

void unset_pin(uint8_t pin)
{
    *simpleio &= ~(1<<pin);
}


byte readRegister(byte address)
{
    byte value = 0x00;
    unset_pin(11);
    SPI.transfer(address);
    value = SPI.transfer(0x00);
    set_pin(11);
    Serial.print(F("ULX3S FPGA Reading SX1276 register 0x"));
    Serial.print(address, HEX);
    Serial.print(F(": 0x"));
    Serial.print(value, HEX);
    Serial.println();
    return value;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin(11);
  Serial.println("Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  readRegister(SX1276_REG_VERSION);
  delay(1000);
}

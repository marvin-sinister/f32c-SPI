 #include <SPI.h>

static const uint8_t SX1276_REG_VERSION = 0x42;
static const uint8_t SX1276_REG_TEMP = 0x3C;
static const uint8_t SX1276_REG_LOW_BAT = 0x3D;
static const uint8_t SX1276_REG_OP_MODE = 0x01;
static const uint8_t SX1276_REG_OSC = 0x24;
static const uint8_t RF_OPMODE_MASK = 0xF8;
static const uint8_t RF_OPMODE_SLEEP = 0x00;
static const uint8_t RF_OPMODE_STANDBY = 0x00;

SPIClass SPI(0);

byte readRegister(byte address)
{
    byte value = 0x00;
    delay(1);
    bitClear(address, 7);    // Bit 7 cleared to write in registers
    SPI.transfer(address);
    value = SPI.transfer(0x00);
    dc(1); // commands
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
  SPI.begin(25);
  Serial.println("Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  readRegister(SX1276_REG_VERSION);
  delay(1000);
}

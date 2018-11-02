#include "SPI.h"

#define LED_RED 8
#define LED_GREEN 10

SPIClass SPI1(FSPI);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup");
  delay(1000);
  SPI1.begin(11);
  delay(1000);
}

void flash_id(void)
{
  SPI1.beginTransaction();
  SPI1.transfer(0x9F);
  leading_zero_hex(SPI1.transfer(0x00));
  leading_zero_hex(SPI1.transfer(0x00));
  leading_zero_hex(SPI1.transfer(0x00));
  SPI1.endTransaction();
}

void loop(void)
{
  Serial.println("##############################");
  delay(500);
  Serial.print("FLASH ID: 0x");
  flash_id();
  Serial.println('\0');
  Serial.println("##############################");
  delay(5000);
}

void leading_zero_hex(uint8_t _value) {
  if (_value < 0x10) {
    Serial.print("0");
  }
  Serial.print(_value, HEX);
}

#include "SPI.h"

#define LED_RED 8
#define LED_GREEN 10

SPIClass SPI1(1);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup");
  delay(1000);
  SPI1.begin(11);
  delay(1000);
}

uint8_t flash_id(void)
{
  SPI1.beginTransaction();
  SPI1.transfer(0xAB);
  SPI1.transfer(0x00);
  SPI1.transfer(0x00);
  SPI1.transfer(0x00);
  return SPI1.transfer(0x00);
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

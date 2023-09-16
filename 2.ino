#include <TM1637Display.h>
#include "DHT.h"

#define CLK 2
#define DIO 3

#define DELAY 2000

#define DHTPIN 4
#define DHTTYPE DHT11 

#define SYM_C SEG_D | SEG_E | SEG_F | SEG_A
#define SYM_O SEG_A | SEG_B | SEG_F | SEG_G
#define SYM_o SEG_C | SEG_D | SEG_E | SEG_G
#define SYM_SPACE 0
#define SYM_MINUS SEG_G

TM1637Display display(CLK, DIO);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  display.setBrightness(0x0f);
  dht.begin();
  display.clear();
}

void loop()
{
  uint8_t segments_t[] = { SYM_O, SYM_C };
  float t = dht.readTemperature();
  display.showNumberDec(t, false, 2, 0);
  display.setSegments(segments_t, 2, 2);
  delay(DELAY);
  uint8_t segments_h[] = { SYM_O, SYM_o };
  float h = dht.readHumidity();
  display.showNumberDec(h, false, 2, 0);
  display.setSegments(segments_h, 2, 2);
  delay(DELAY);
}

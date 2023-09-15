#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define DELAY1 15
#define DELAY2 1000
#define DELAY3 800

TM1637Display display(CLK, DIO);

void setup()
{
}

void loop()
{
  display.setBrightness(0x0f);
  //task1();
  //task2();
  task3();
  while(1);
}

void task1()
{
  for (int i = -100; i < 101; i++)
  {
    display.showNumberDec(i, false, 3, 0);
    delay(DELAY1);
  }
  while(1);
}

void task2()
{
    const uint8_t minus[] = { SEG_G };
    for (int i = -200; i <= 200; i += 20)
    {
        display.clear();

        display.showNumberDecEx(abs(i), 0xff, true, 3, 1);

        if (i < 0)
            display.setSegments(minus, 1);

        delay(DELAY2);
    }
    delay(DELAY2);
}

void task3()
{
  uint8_t text[] = {
    SEG_F | SEG_A | SEG_B | SEG_G | SEG_C,
    SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
    SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
    0,
    SEG_A | SEG_F | SEG_G | SEG_E,
    SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
    SEG_F | SEG_E | SEG_D,
    SEG_F | SEG_E | SEG_D,
    0
  };
  int i = 0;
  while(1)
  {
    display.setSegments(text+i);
    i = (i+1)%10;
    delay(DELAY3);
  }
}

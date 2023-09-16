#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define DELAY1 15
#define DELAY2 1000
#define DELAY3 800

#define SYM_A SEG_E | SEG_F | SEG_A | SEG_B | SEG_C | SEG_G
#define SYM_B SEG_F | SEG_E | SEG_D | SEG_C | SEG_G
#define SYM_C SEG_D | SEG_E | SEG_F | SEG_A
#define SYM_D SEG_G | SEG_E | SEG_D | SEG_C | SEG_B
#define SYM_E SEG_A | SEG_F | SEG_G | SEG_E | SEG_D
#define SYM_F SEG_A | SEG_F | SEG_G | SEG_E
#define SYM_G SEG_A | SEG_F | SEG_E | SEG_D | SEG_C
#define SYM_H (SEG_F | SEG_G | SEG_B | SEG_E | SEG_C)
#define SYM_I SEG_F | SEG_E
#define SYM_J SEG_D | SEG_C | SEG_B
#define SYM_L SEG_F | SEG_E | SEG_D
#define SYM_N SEG_E | SEG_G | SEG_C
#define SYM_O SEG_E | SEG_G | SEG_C | SEG_D
#define SYM_P SEG_E | SEG_F | SEG_A | SEG_B | SEG_G
#define SYM_Q SEG_F | SEG_A | SEG_B | SEG_G | SEG_C
#define SYM_R SEG_E | SEG_G
#define SYM_T SEG_F | SEG_G | SEG_E | SEG_D
#define SYM_U SEG_F | SEG_E | SEG_D | SEG_C | SEG_B
#define SYM_Y SEG_F | SEG_G | SEG_B | SEG_C | SEG_D
#define SYM_SPACE 0
#define SYM_MINUS SEG_G

TM1637Display display(CLK, DIO);

void setup()
{
}

void loop()
{
  display.setBrightness(0x0f);
  display.clear();
  task1();
  display.clear();
  task2();
  display.clear();
  task3();
}

void task1()
{
  for (int i = -100; i < 101; i++)
  {
    display.showNumberDec(i, false, 3, 0);
    delay(DELAY1);
  }
}

void task2()
{
    const uint8_t minus[] = { SYM_MINUS };
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
  uint8_t text[] = { SYM_Q, SYM_U, SYM_E, SYM_U, SYM_E, SYM_SPACE, SYM_F, SYM_U, SYM_L, SYM_L, SYM_SPACE, SYM_SPACE, SYM_SPACE, SYM_SPACE,};
  int size_text = sizeof(text)/sizeof(text[0])-4;
  int i = 0;
  while(1)
  {
    display.setSegments(text+i);
    i = (i+1)%size_text;
    delay(DELAY3);
  }
}

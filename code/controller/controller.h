#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define p_lSchulter A3
#define p_rSchulter A2
#define p_joyX A1
#define p_joyY A0

#define p_red 4
#define p_green 5
#define p_blue 6

#define p_joyButton 0
#define p_buzzer 8


#ifdef controller1
  const byte robotAddress[6] = "c-r01";
#endif

#ifdef controller2
  const byte robotAddress[6] = "c-r02";
#endif

#ifdef controller3
  const byte robotAddress[6] = "c-r03";
#endif


int crntMillis, millisLastPacket, robotConnected, prevMillisLoading, millisBoost;
int loading;
#define BOOST_DURATION 5000

#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void lcdSwish();
void rgbWrite(int r, int g, int b);

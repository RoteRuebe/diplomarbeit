#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define p_lSchulter A2
#define p_rSchulter A3
#define p_joyX A1
#define p_joyY A0

#define p_red 4
#define p_green 5
#define p_blue 6


#ifdef controller1
  const byte robotAddress[6] = "c-r01";
  #define ROBOTCHANNEL 25
#endif

#ifdef controller2
  const byte robotAddress[6] = "c-r02";
  #define ROBOTCHANNEL 75
#endif

#ifdef controller3
  const byte robotAddress[6] = "c-r03";
  #define ROBOTCHANNEL 125
#endif


int crntMillis, millisLastPacket, robotConnected;

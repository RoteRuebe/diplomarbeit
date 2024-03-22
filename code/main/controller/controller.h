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
  const byte robotAddress[6] = "c00001";
  #define CHANNEL 75
#endif

#ifdef controller2
  const byte robotAddress[6] = "c00002";
  #define CHANNEL 100
#endif

#ifdef controller3
  const byte robotAddress[6] = "c00003";
  #define CHANNEL 125
#endif

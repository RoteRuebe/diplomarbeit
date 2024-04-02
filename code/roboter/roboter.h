#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/*
Roboter 1	r00001	100
Controller 1	c00001	25
Roboter 2	r00002	50
Controller 2	c00002	75
Roboter 3	r00003	0
Controller 3	c00003	87
*/

// Different channels and pipes for every robot
#ifdef robot1
  const byte controllerAddress[6] = "c-r01";
  const byte serverAddress[6] = "r-s01";
  #define SERVERCHANNEL 0
  #define CONTROLLERCHANNEL 25
#endif

#ifdef robot2
  const byte controllerAddress[6] = "c-r02";
  const byte serverAddress[6] = "r-s02";
  #define SERVERCHANNEL 50
  #define CONTROLLERCHANNEL 75
#endif

#ifdef robot3
  const byte controllerAddress[6] = "c-r03";
  const byte serverAddress[6] = "r-s003";
  #define SERVERCHANNEL 97
  #define CONTROLLERCHANNEL 125
#endif

#define p_red 8
#define p_green 7
#define p_blue 6

#define p_lf 5  //Left Forward Pin
#define p_lb 2
#define p_rf 3
#define p_rb 4  //Right Backward Pin

#define p_vibration 46

const int pushDataTimestamp = 100/5;

//CE, CSN
RF24 radio(13, 12);
void configureRadio();
void drive(int left, int right);
void rgbWrite(int r, int g, int b);
float fmap(float x, float in_min, float in_max, float out_min, float out_max);
int logMsg(char *x);
int sendSensorData();

int speed;
float turn;
float l_motor_turn, r_motor_turn;
int crntMillis, prevMillisData, millisLastPacket;
int lm_ist, rm_ist, lm_soll, rm_soll;
int controllerConnected, serverConnected;

int inputs[4];
#define joyX inputs[0]
#define joyY inputs[1]
#define shoL inputs[2]
#define shoR inputs[3]

Adafruit_MPU6050 mpu;

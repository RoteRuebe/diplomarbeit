#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Different channels and pipes for every robot
#ifdef robot1
  const byte controllerAddress[6] = "c00001";
  const byte serverAddress[6] = "r00001";
  #define serverChannel 100
  #define controllerChannel 25
#endif

#ifdef robot2
  const byte controllerAddress[6] = "c00002";
  const byte serverAddress[6] = "r00002";
  #define serverChannel 50
  #define controllerChannel 75
#endif

#ifdef robot3
  const byte controllerAddress[6] = "c00001";
  const byte serverAddress[6] = "r00001";
  #define serverChannel 0
  #define controllerChannel 87
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
void drive(int left, int right);
void rgbWrite(int r, int g, int b);
float fmap(float x, float in_min, float in_max, float out_min, float out_max);
int logMsg(char *x);
void sendSensorData();

int speed;
float turn;
float l_motor_turn, r_motor_turn;
int crntMillis, prevMillisData, millisLastPacket;
int lm_ist, rm_ist, lm_soll, rm_soll;
int controllerConnected;

int inputs[4];
#define joyX inputs[0]
#define joyY inputs[1]
#define shoL inputs[2]
#define shoR inputs[3]

Adafruit_MPU6050 mpu;

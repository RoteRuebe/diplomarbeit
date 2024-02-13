#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

#define p_red 8
#define p_green 7
#define p_blue 6

#define p_lf 5  //Left Forward Pin
#define p_lb 2
#define p_rf 3
#define p_rb 4  //Right Backward Pin

#define p_vibration 46

const int doDebug = 0;
const byte controllerAddress[6] = "00001";
const byte serverAddress[6] = "00002";
const int pushDataTimestamp = 500;

//CE, CSN
RF24 radio(13, 12);
void drive(int left, int right);
void rgbWrite(int r, int g, int b);
float fmap(float x, float in_min, float in_max, float out_min, float out_max);
int logMsg(char *x, int listenAfter = 1);
int sendSensorData(char* x, int listenAfter = 1);

int speed = 0;
float turn;
float l_motor_turn, r_motor_turn;
int crntMillis, prevMillis = 0;
int lm_ist, rm_ist, lm_soll, rm_soll;

int inputs[4];
int joyX, joyY, shoL, shoR;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Different channels and pipes for every robot
#ifdef robot1
  const byte controllerAddress[6] = "c-r01";
  const byte serverAddress[6] = "1-r-s";

  #include "bitmaps/bitmap1.h"
#endif

#ifdef robot2
  const byte controllerAddress[6] = "c-r02";
  const byte serverAddress[6] = "2-r-s";
  
  #include "bitmaps/bitmap2.h"
#endif

#ifdef robot3
  const byte controllerAddress[6] = "c-r03";
  const byte serverAddress[6] = "3-r-s";

  #include "bitmaps/bitmap3.h"
#endif

#define p_red 8
#define p_green 7
#define p_blue 6

#define p_lf 5  //Left Forward Pin
#define p_lb 2
#define p_rf 3
#define p_rb 4  //Right Backward Pin

#define p_vibration 46

#define p_color_s0 45
#define p_color_s1 44
#define p_color_s2 48
#define p_color_s3 47
#define p_color_out 49

#define p_breakLight 9
#define p_lamp 41

const int pushDataTimestamp = 100;

//CE, CSN
RF24 radio(13, 12);
void configureRadio();
void drive(int left, int right);
void rgbWrite(int r, int g, int b);
float fmap(float x, float in_min, float in_max, float out_min, float out_max);
int logMsg(char *x, int len);
int sendSensorData();

int speed;
float turn;
float lm_turn, rm_turn;
int crntMillis, prevMillisData, millisLastPacket, prevMillisDraw;
int lm_ist, rm_ist, lm_soll, rm_soll;
uint8_t controllerConnected, serverConnected;

int inputs[4];
#define JOY_X inputs[0]
#define JOY_Y inputs[1]
#define SHO_L inputs[2]
#define SHO_R inputs[3]

Adafruit_MPU6050 mpu;

struct DataPayload {
  uint8_t zero; // should be 0x00 to signify to server it isn't a log message

  float gyroX;
  float gyroY; 
  float gyroZ;

  float accX;
  float accY;
  float accZ;

  float temp;
  
  uint8_t controllerConnected;
  uint8_t vibration;
};

sensors_event_t sensor_a, sensor_g, sensor_temp;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

typedef enum { Default, Dead, Surprised} face_t;

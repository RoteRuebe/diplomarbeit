/**
*   Auth: Yannick Zickler
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

#define robot1
#include "roboter.h"

/** Initialization **/

void setup() {
  // Setup Pins
  pinMode(p_lf, OUTPUT);
  pinMode(p_lb, OUTPUT);
  pinMode(p_rf, OUTPUT);
  pinMode(p_rb, OUTPUT);
  
  pinMode(p_red, OUTPUT);
  pinMode(p_green, OUTPUT);
  pinMode(p_blue, OUTPUT);

  pinMode(p_vibration, INPUT);
  
  rgbWrite(1, 0, 0);
  
  // Do nothing while radio module not connected
  while ( !radio.begin() ) {}
  
  // Open Radio pipes
  radio.openReadingPipe(1, controllerAddress);
  radio.openWritingPipe(serverAddress); 
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(CHANNEL);
  radio.enableDynamicAck();
  
  if(logMsg("Radio initalized."))
    rgbWrite(0, 1, 0);
  else
    rgbWrite(0, 0, 1);

  if(mpu.begin()) {
    logMsg("Gyro Initialized.");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  else {
    logMsg("Gyro not connected.");
  }

  logMsg("Initialization complete.");  
}

void loop() {
  crntMillis = millis();

  if (radio.available()) {
    radio.read(inputs, sizeof(inputs)); 
    controllerConnected = 1;
    millisLastPacket = crntMillis;
  }

  else if (crntMillis - millisLastPacket > 100)
    controllerConnected = 0;

  if (crntMillis - prevMillisData > pushDataTimestamp) {
    sendSensorData();
    prevMillisData = crntMillis;
  }

  // Calculate Motor speeds
  if (shoR > shoL)
    speed = shoR;

  else
    speed = -shoL;

  turn = (float)joyX / 256.0;  
  if (joyX < 0) {
    l_motor_turn = fmap(turn, -1.0, 0.0, -1.0, 1.0);
    r_motor_turn = 1.0;
  }

  else if (joyX >= 0) {
    l_motor_turn = 1.0;
    r_motor_turn = fmap(turn, 0.0, 1.0, 1.0, -1.0);
  }
  
  lm_soll = (int)(l_motor_turn * speed);
  rm_soll = (int)(r_motor_turn * speed);

  // Slow down acceleration of motors
  if (controllerConnected) {
    if (lm_ist > lm_soll)
      lm_ist -= 1;

    else if (lm_ist < lm_soll)
      lm_ist += 1;

    if (rm_ist > rm_soll)
      rm_ist -= 1;

    else if (rm_ist < rm_soll)
      rm_ist += 1;

    drive(lm_ist, rm_ist);
  }

  else {
    drive(0, 0);
  }
}

/** Functions **/

void rgbWrite(int r, int g, int b) {
  digitalWrite(p_red, r);
  digitalWrite(p_green, g);
  digitalWrite(p_blue, b);
}

void drive(int left, int right) {
  left = -left;
  if (left > 0) {
    analogWrite(p_lf, left);
    analogWrite(p_lb, 0);
  } 
  else {
    analogWrite(p_lb, -left);
    analogWrite(p_lf, 0);
  }

  if (right > 0) {
    analogWrite(p_rf, right);
    analogWrite(p_rb, 0);
  } 
  else {
    analogWrite(p_rb, -right);
    analogWrite(p_rf, 0);
  }
}

int logMsg(char *x) {
  int resp;
  radio.stopListening();

  #ifdef robot1
    char msg[32] = "1,log,";
  #endif
  #ifdef robot2
    char msg[32] = "2,log,";
  #endif
  #ifdef robot3
    char msg[32] = "3,log,";
  #endif
  strcat(msg, x);
  resp = radio.write( &msg, sizeof(msg) );

  radio.startListening();
  return resp;
}

#ifdef robot1
void sendSensorData() {
  radio.stopListening();

  static int index = 0;
  int response;
  char data[32] = {0};
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  switch(index) {
    case(0):
      sprintf(data, "1,temp,%.2f", temp.temperature);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(1):
      sprintf(data, "1,acc,%.2f,%.2f,%.2f", a.acceleration.x, a.acceleration.y, a.acceleration.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(2):
      sprintf(data, "1,gyro,%.2f,%.2f,%.2f", g.gyro.x, g.gyro.y, g.gyro.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(3):
      sprintf(data, "1,vibration,%d", digitalRead(p_vibration));
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(4):
      sprintf(data, "1,controllerConnected,%d", controllerConnected);
      radio.startWrite(&data, sizeof(data), true);
      break;
  }

  // Dont switch too fast to read mode
  delay(1);

  index ++;
  if (index == 5) index = 0;

  radio.startListening();
}
#endif

#ifdef robot2
void sendSensorData() {
  radio.stopListening();

  static int index = 0;
  int response;
  char data[32] = {0};
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  switch(index) {
    case(0):
      sprintf(data, "2,temp,%.2f", temp.temperature);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(1):
      sprintf(data, "2,acc,%.2f,%.2f,%.2f", a.acceleration.x, a.acceleration.y, a.acceleration.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(2):
      sprintf(data, "2,gyro,%.2f,%.2f,%.2f", g.gyro.x, g.gyro.y, g.gyro.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(3):
      sprintf(data, "2,vibration,%d", digitalRead(p_vibration));
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(4):
      sprintf(data, "2,controllerConnected,%d", controllerConnected);
      radio.startWrite(&data, sizeof(data), true);
      break;
  }

  // Dont switch too fast to read mode
  delay(1);

  index ++;
  if (index == 5) index = 0;

  radio.startListening();
}
#endif

#ifdef robot3
void sendSensorData() {
  radio.stopListening();

  static int index = 0;
  int response;
  char data[32] = {0};
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  switch(index) {
    case(0):
      sprintf(data, "3,temp,%.2f", temp.temperature);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(1):
      sprintf(data, "3,acc,%.2f,%.2f,%.2f", a.acceleration.x, a.acceleration.y, a.acceleration.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(2):
      sprintf(data, "3,gyro,%.2f,%.2f,%.2f", g.gyro.x, g.gyro.y, g.gyro.z);
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(3):
      sprintf(data, "3,vibration,%d", digitalRead(p_vibration));
      radio.startWrite(&data, sizeof(data), true);
      break;
    
    case(4):
      sprintf(data, "3,controllerConnected,%d", controllerConnected);
      radio.startWrite(&data, sizeof(data), true);
      break;
  }

  // Dont switch too fast to read mode
  delay(1);

  index ++;
  if (index == 5) index = 0;

  radio.startListening();
}
#endif


float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

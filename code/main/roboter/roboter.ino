/**
*   Auth: Yannick Zickler
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

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
  
  radio.startListening();
  radio.setPALevel(RF24_PA_MIN);
  
  if(logMsg("Radio initalized."))
    rgbWrite(0, 1, 0);
  else
    rgbWrite(0, 0, 1);

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);

  logMsg("Gyro Initialized.");
  
}

void loop() {
  crntMillis = millis();
  
  if (crntMillis - prevMillis > pushDataTimestamp) {
    // Push sensor data
    logMsg("Sending Sensordata.");
    sendSensorData();
    prevMillis = crntMillis;
  }

  if (radio.available()) {
    radio.read(inputs, sizeof(inputs)); 
    //logMsg("Inputs received");     
  }
  joyX = inputs[0];
  joyY = inputs[1];
  shoL = inputs[2];
  shoR = inputs[3];

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

  if (doDebug) {
    /**
    logMsg("X: ", 0);
    logMsg(inputs[0], 0);
    logMsg("Y: ", 0);
    logMsg(inputs[1], 0);
    logMsg("L: ", 0);
    logMsg(inputs[2], 0);
    logMsg("R: ", 0);
    logMsg(inputs[3]);
    */
  }

  // Slow down acceleration of motors
  else {
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

int logMsg(char *x, int listenAfter = 1) {
  int resp;
  radio.stopListening();

  char msg[32] = "log,";
  strcat(msg, x);
  resp = radio.write( &msg, sizeof(msg) );

  if (listenAfter) {
    radio.startListening();
  }

  return resp;
}

int sendSensorData() {
  radio.stopListening();

  int response;
  char data[32] = {0};
  sprintf(data, "vibration,%d", digitalRead(p_vibration));
  response = radio.write( &data, sizeof(data) );

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  a.acceleration.x, a.acceleration.y, a.acceleration.z ,%d,%d,%d);
  response &= radio.write(&data, sizeof(data));

  sprintf(data, "gyro,%d,%d,%d", g.gyro.x, g.gyro.y, g.gyro.z);
  response &= radio.write(&data, sizeof(data));

  sprintf(data, "temp,%d", temp.temperature);
  response &= radio.write(&data, sizeof(data));

  radio.startListening();
  return response;
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

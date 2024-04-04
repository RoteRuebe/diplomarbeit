/**
*   Auth: Yannick Zickler
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

#define robot3
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
  
  rgbWrite(1, 0, 0); // red
  configureRadio();
  

  if(mpu.begin()) {
    logMsg("Gyro Initialized.", 17);

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  else {
    logMsg("Gyro not connected.", 19);
  }

  logMsg("Initialization complete.", 24);  
}

void loop() {
  crntMillis = millis();

  if (radio.failureDetected) {
    drive(0, 0);

    //blocks until radio configured
    configureRadio();
    logMsg("Radio reconfigured after failure", 32);
  }

  if (radio.available()) {
    radio.read(inputs, sizeof(inputs)); 
    controllerConnected = 1;
    millisLastPacket = crntMillis;
  }

  else if (crntMillis - millisLastPacket >= 500) {
    controllerConnected = 0;
  }

  if (crntMillis - prevMillisData > pushDataTimestamp) {
    serverConnected = sendSensorData();
    prevMillisData = crntMillis;
  }

  if (serverConnected && controllerConnected)
    rgbWrite(0, 1, 0); // green
  else if (!serverConnected && controllerConnected)
    rgbWrite(0, 0, 1); // blue
  else if (serverConnected && !controllerConnected)
    rgbWrite(1, 1, 0); // yellow
  else if (!serverConnected && !controllerConnected )
    rgbWrite(0, 1, 1); // cyan

  // Calculate Motor speeds
  if (SHO_R > SHO_L)
    speed = SHO_R;

  else
    speed = -SHO_L;

  turn = (float)JOY_X / 256.0;  
  if (JOY_X < 0) {
    lm_turn = fmap(turn, -1.0, 0.0, -1.0, 1.0);
    rm_turn = 1.0;
  }

  else if (JOY_X >= 0) {
    lm_turn = 1.0;
    rm_turn = fmap(turn, 0.0, 1.0, 1.0, -1.0);
  }
  
  lm_soll = (int)(lm_turn * speed);
  rm_soll = (int)(rm_turn * speed);

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

void configureRadio() {
  // Do nothing while radio module not connected
  while ( !radio.begin() ) {}
  
  radio.openReadingPipe(1, controllerAddress);
  radio.openWritingPipe(serverAddress); 
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(4, 10);
}


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

int logMsg(char *x, int len) {
  radio.stopListening();

  int response = radio.write( x, len );

  radio.startListening();
  return response;
}

int sendSensorData() {
  radio.stopListening();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  struct DataPayload payload = {
    0x00,
    g.gyro.x, g.gyro.y, g.gyro.z, 
    a.acceleration.x, a.acceleration.y, a.acceleration.z, 
    temp.temperature,
    controllerConnected,
    digitalRead(p_vibration)
  };
  int response = radio.write(&payload, sizeof(payload));

  radio.startListening();
  return response;
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

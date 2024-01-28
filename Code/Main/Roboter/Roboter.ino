/**
*   Auth: Yannick Zickler
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

#include "Roboter.h"

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
  rgbWrite(0, 0, 1);
  
  // Open Radio pipes
  radio.openReadingPipe(1, controllerAddress);
  radio.openWritingPipe(serverAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  rgbWrite(0, 1, 0);
  
  logMsg("Initalized.");
  crntMillis = 0;
}

int data = 0;
char data_str[16] = "0";
void loop() {
  crntMillis = millis();
  
  if (crntMillis - prevMillis > pushDataTimestamp) {
    // Push sensor data
    logMsg("Sending Sensordata.");
    data = digitalRead(p_vibration);
    sprintf(data_str, "%d", data);
    sendSensorData( data_str );
    prevMillis = crntMillis;
  }

  if (radio.available()) {
    radio.read(inputs, sizeof(inputs)); 
    logMsg("Inputs received");     
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
    l_motor_turn = map(turn, -1, 0, -1, 1);
    r_motor_turn = 1;
  }

  else if (joyX >= 0) {
    l_motor_turn = 1;
    r_motor_turn = map(turn, 0, 1, 1, -1);
  }
  
  lm_soll = l_motor_turn * speed;
  rm_soll = r_motor_turn * speed;

  if (doDebug && 0) {
    Serial.print("X: ");
    Serial.println(inputs[0]);
    Serial.print("Y: ");
    Serial.println(inputs[1]);
    Serial.print("L: ");
    Serial.println(inputs[2]);
    Serial.print("R: ");
    Serial.println(inputs[3]);
    Serial.println();

    Serial.print("Geschwindigkeit [0..255]: ");
    Serial.println(speed);
    Serial.println("Kurve          [-1..1]: ");
    Serial.println(turn);
    Serial.println();
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

int logMsg( char *x) {
  int resp;
  radio.stopListening();
  radio.setChannel(200);

  char msg[32] = "log,";
  strcat(msg, x);
  resp = radio.write( &msg, sizeof(msg) );

  radio.startListening();
  radio.setChannel(76);

  return resp;
}

int sendSensorData( char *x) {
  int resp;
  radio.stopListening();
  radio.setChannel(200);

  char msg[8] = "data,";
  strcat(msg, x);
  resp = radio.write( &msg, sizeof(msg) );

  radio.startListening();
  radio.setChannel(76);

  return resp;
}

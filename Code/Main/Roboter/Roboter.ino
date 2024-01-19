/**
*   Auth: Yannick Zickler
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

#include "Roboter.h"

/** Initialization **/

void setup() {
  pinMode(p_lf, OUTPUT);
  pinMode(p_lb, OUTPUT);
  pinMode(p_rf, OUTPUT);
  pinMode(p_rb, OUTPUT);
  
  pinMode(p_red, OUTPUT);
  pinMode(p_green, OUTPUT);
  pinMode(p_blue, OUTPUT);
  
  rgbWrite(1, 0, 0);
  
  //Do nothing while radio module not connected
  while ( !radio.begin() ) {}
  rgbWrite(0, 0, 1);
  
  radio.openReadingPipe(1, controllerAddress);
  radio.openWritingPipe(serverAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  logMsg("Initalized.");
  
  rgbWrite(0, 1, 0);
  crntMillis = 0;
}

void loop() {
  crntMillis = millis();
  
  if (crntMillis - prevMillis > pushDataTimestamp) {
    logMsg("Sending Sensordata.");
    prevMillis = crntMillis;
  }

  if (radio.available())
    radio.read(inputs, sizeof(inputs));
    
  joyX = inputs[0];
  joyY = inputs[1];
  shoL = inputs[2];
  shoR = inputs[3];

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
  radio.stopListening();
  radio.setChannel(200);
  radio.write( x, strlen(x) );
  radio.startListening();
  radio.setChannel(76);
}

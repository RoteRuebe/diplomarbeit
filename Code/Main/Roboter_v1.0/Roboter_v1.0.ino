/**
*   Vers: 1.0
*   Auth: Yannick Zickler
*   
*   Created: 11.09.2023
*   Last modified: 11.09.2023
*
*   Desc: Code for the robot. Handles communication, drives motors serial Interface and LED.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BTLE.h>

/** Parameters **/

const int doDebug = 0;
/*
    0: Normal-mode
    1: Debug-mode
*/

#define p_red 8
#define p_green 7
#define p_blue 6

#define p_lf 5  //Left Forward Pin
#define p_lb 2
#define p_rf 3
#define p_rb 4  //Right Backward Pin

//CSN, CE
RF24 radio(1, 11); 


/** Initialization **/

void drive(int left, int right);
void rgbWrite(int r, int g, int b);

const byte address[6] = "00001";

int speed;
float turn = 0;
float l_motor_turn, r_motor_turn;

void setup() {
  pinMode(p_lf, OUTPUT);
  pinMode(p_lb, OUTPUT);
  pinMode(p_rf, OUTPUT);
  pinMode(p_rb, OUTPUT);
  
  pinMode(p_red, OUTPUT);
  pinMode(p_green, OUTPUT);
  pinMode(p_blue, OUTPUT);
  
  rgbWrite(1, 0, 0);
  
  if (doDebug)
    Serial.begin(9600);

  // Do nothing while radio module not connected
  while ( !radio.begin() ) {}
  rgbWrite(0, 0, 1);
  
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  // Do nothing while no connection
  while ( !radio.available() ) {}
  rgbWrite(0, 1, 0); delay(250); 
  rgbWrite(0, 0, 0); delay(250);
  
  rgbWrite(0, 1, 0);
}

/** Main Loop **/

int crntMillis, prevMillis = 0;
int lm_ist, rm_ist, lm_soll, rm_soll = 0;
//Linker/Rechter Motor

//JoyX, JoyY, l_Button, r_Button
int inputs[] = { 0, 0, 0, 0 };

int joyX, joyY, shoL, shoR;
void loop() {
  if (radio.available())
    radio.read(inputs, sizeof(inputs));
    
  joyX = inputs[0];
  joyY = inputs[1];
  shoL = inputs[2];
  shoR = inputs[3];

  if (shoR > shoL)
    speed = shoR * sensitivityShoulder;

  else
    speed = -shoL * sensitivityShoulder;
  
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


  if (doDebug) {
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
    analogWrite(lfp, left);
    analogWrite(lbp, 0);
  } 
  else {
    analogWrite(lbp, -left);
    analogWrite(lfp, 0);
  }

  if (right > 0) {
    analogWrite(rfp, right);
    analogWrite(rbp, 0);
  } 
  else {
    analogWrite(rbp, -right);
    analogWrite(rfp, 0);
  }
}

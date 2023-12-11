/**
*   Vers: 0.0
*   Auth: Yannick Zickler
*   
*   Created: 01.06.2023
*   Last modified: 13.06.2023
*
*   Desc: Code for both robots. Handles communication and drives the motors.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BTLE.h>

/** Parameters **/

const int doDebug = 1;
/*
    0: Normal-mode
    1: Debug-mode
*/

// Which Robot is it?
const int robotIndex = 1;

#define lfp 5  //Left Forward Pin
#define lbp 10
#define rfp 6
#define rbp 9  //Right Backward Pin

#define sensitivityShoulder 2

//CSN, CE
RF24 radio(3, 4); 


/** Initialization **/

void drive(int left, int right);

int ges;
float turn = 0;

void setup() {
  if (doDebug)
    Serial.begin(9600);

  radio.begin();
    
  if (robotIndex == 0) {
    const byte address[6] = "00000";
    radio.setChannel(10);
  }

  else {
    const byte address[6] == "00001";
    radio.setChannel(200);
  }

  radio.openReadingPipe(0, address);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(lfp, OUTPUT);
  pinMode(lbp, OUTPUT);
  pinMode(rfp, OUTPUT);
  pinMode(rbp, OUTPUT);
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
    ges = shoR * sensitivityShoulder;

  else
    ges = -shoL * sensitivityShoulder;

  if (joyX > 512) {
    turn = 3 - joyX / 256.0;

    rm_soll = ges * turn;
    lm_soll = ges;
  }

  else {
    turn = (float)(joyX) / 256.0 - 1.0;

    rm_soll = ges;
    lm_soll = ges * turn;
  }

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
    Serial.println(ges);
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

void drive(int left, int right) {
  if (left > 0) {
    analogWrite(lfp, left);
    analogWrite(lbp, 0);
  } else {
    analogWrite(lbp, -left);
    analogWrite(lfp, 0);
  }

  if (right > 0) {
    analogWrite(rfp, right);
    analogWrite(rbp, 0);
  } else {
    analogWrite(rbp, -right);
    analogWrite(rfp, 0);
  }
}

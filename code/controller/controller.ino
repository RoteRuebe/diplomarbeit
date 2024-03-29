/**
*   Auth: Yannick Zickler
*
*   Desc: Code for both controllers. Reads button-values and sends them to Robot.
*/

#define controller1
#include "controller.h"

/** Parameters **/

// CE, CSN
RF24 radio(10, 9);

void rgbWrite(int r, int g, int b);

/** Initialization **/
int data[] = {0, 0, 0, 0};

void setup() {
  pinMode(p_red, OUTPUT);
  pinMode(p_green, OUTPUT);
  pinMode(p_blue, OUTPUT);

  rgbWrite(1, 0, 0);

  while (!radio.begin()) {}
  rgbWrite(0, 0, 1);
  
  radio.openWritingPipe(robotAddress);
  radio.stopListening();
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(ROBOTCHANNEL);
  
  );
  
  pinMode(p_lSchulter, INPUT);
  pinMode(p_rSchulter, INPUT);
  pinMode(p_joyX, INPUT);
  pinMode(p_joyY, INPUT);
  
  // Do nothing while not connected
  while ( !radio.write(&data, sizeof(data)) ) {}
  rgbWrite(0, 1, 0);
}

/** Main Loop **/

void loop() {
  data[0] = analogRead(p_joyX)/2 - 255;
  data[1] = analogRead(p_joyY)/2 - 255;
  data[2] = 255 - (analogRead(p_lSchulter) >> 2);
  data[3] = 255 - (analogRead(p_rSchulter) >> 2);

  if( radio.write(data, sizeof(data)) )
    rgbWrite(0, 1, 0);
  else
    rgbWrite(0, 0, 1);
}

void rgbWrite(int r, int g, int b) {
  digitalWrite(p_red, r);
  digitalWrite(p_green, g);
  digitalWrite(p_blue, b);
}

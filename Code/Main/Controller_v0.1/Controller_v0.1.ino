/**
*   Vers: 0.1
*   Auth: Yannick Zickler
*   
*   Created: 7.10.2023
*   Last modified: 7.10.2023
*
*   Desc: Code for both controllers. Reads button-values and sends them to Robot.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/** Parameters **/

#define p_lSchulter A2
#define p_rSchulter A3
#define p_joyX A1
#define p_joyY A0

// Which controller is ist?
const int controllerIndex = 1;

// CE, CSN
RF24 radio(9, 10); 

/** Initialization **/

void setup() {
  radio.begin();
  
  if (controllerIndex == 0) {
    radio.setChannel(10);
    const byte address[6] = "00000";
  }

  else {
    radio.setChannel(200);
    const byte address[6] = "00001";
  }

  radio.openWritingPipe(address);
  radio.stopListening();

  pinMode(p_lSchulter, INPUT);
  pinMode(p_rSchulter, INPUT);
  pinMode(p_joyX, INPUT);
  pinMode(p_joyY, INPUT);
}

/** Main Loop **/

int data[] = {0, 0, 0, 0};
void loop() {
  data[0] = analogRead(p_joyX);
  data[1] = analogRead(p_joyY);
  data[2] = analogRead(p_lSchulter);
  data[3] = analogRead(p_rSchulter);
  
  radio.write(&array, sizeof(array));
  delay(1);
}

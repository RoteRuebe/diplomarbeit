/**
*   Vers: 0.0
*   Auth: Yannick Zickler
*   
*   Created: 01.06.2023
*   Last modified: 13.06.2023
*
*   Desc: Code for both controllers. Reads button-values and sends them to Robot.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/** Parameters **/

#define l_schulter A2
#define r_schulter A3
#define joyX A1
#define joyY A0

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
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  pinMode(l_schulter, INPUT);
  pinMode(r_schulter, INPUT);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
}

/** Main Loop **/

int array[] = {0, 0, 0, 0};
void loop() {
  array[0] = analogRead(joyX);
  array[1] = analogRead(joyY);
  array[2] = analogRead(l_schulter);
  array[3] = analogRead(r_schulter);
  
  radio.write(&array, sizeof(array));
  delay(1);
}

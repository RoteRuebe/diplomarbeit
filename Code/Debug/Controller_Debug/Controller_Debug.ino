/**
*   Vers: 0.0
*   Auth: Yannick Zickler
*   
*   Created: 25.10.2023
*   Last modified: 25.10.2023
*
*   Desc: Code for fake controller for debugging purposes
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/** Parameters **/

#define p_forward xxx
#define p_backward xxx


// Which controller is ist?
const byte address[6] = "00001";

// CE, CSN
RF24 radio(8, 7); 

/** Initialization **/

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  
  Serial.begin(9600);
  pinMode(p_button, INTERNAL_PULLUP);
}

/** Main Loop **/

int inputs[] = {0, 0};

void loop() {
  analogRead(p_forward);
  analogRead(p_backward);
  
  radio.write(&inputs,sizeof(inputs));
  delay(1);
}

/**
*   Vers: 1.0
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
#include <printf.h>

/** Parameters **/

#define p_lSchulter A2
#define p_rSchulter A3
#define p_joyX A1
#define p_joyY A0

#define led 13

// Which controller is ist?    
const byte address[6] = "00001";

// CE, CSN
RF24 radio(9, 10); 

/** Initialization **/
int data[] = {0, 0, 0, 0};

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  printf_begin();
  
  Serial.begin(9600);
  Serial.println();
  Serial.print("Attempting to initialize radio module");
  while (!radio.begin()) {Serial.print(".");delay(500);}
  Serial.println();
  radio.openWritingPipe(address);
  radio.stopListening();
  Serial.println("Radio initialized");
  radio.printDetails();

  pinMode(p_lSchulter, INPUT);
  pinMode(p_rSchulter, INPUT);
  pinMode(p_joyX, INPUT);
  pinMode(p_joyY, INPUT);
  
  // Do nothing while not connected
  Serial.print("Attempting to write packet");
  while ( !radio.write(&data, sizeof(data)) ) {Serial.print(".");delay(500);}
  Serial.println();
  digitalWrite(led, 1); delay(250);
  digitalWrite(led, 0); delay(250);
  digitalWrite(led, 1);
  Serial.println("Packet received");
  Serial.println("Initialization complete!");
}

/** Main Loop **/

void loop() {
/*
  data[0] = analogRead(p_joyX);
  data[1] = analogRead(p_joyY);
*/
  data[0] = 0;
  data[1] = 0;

  data[2] = analogRead(p_lSchulter);
  data[3] = analogRead(p_rSchulter);
  
  radio.write(&data, sizeof(data));
  delay(1);
}

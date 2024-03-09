  /**
*   Auth: Yannick Zickler

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

#define p_red 4
#define p_green 5
#define p_blue 6

// Which controller is ist?    
const byte robotAddress[6] = "00001";

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

  printf_begin();
  
  Serial.begin(9600);
  Serial.println();
  Serial.print("Attempting to initialize radio module");
  while (!radio.begin()) {Serial.print(".");delay(500);}
  rgbWrite(0, 0, 1);
  Serial.println();
  radio.openWritingPipe(robotAddress);
  radio.stopListening();
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MIN);
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
  rgbWrite(0, 1, 0);
  Serial.println("Packet received");
  Serial.println("Initialization complete!");
}

/** Main Loop **/

void loop() {
  data[0] = analogRead(p_joyX)/2 - 255;
  data[1] = analogRead(p_joyY)/2 - 255;
  data[2] = 255 - (analogRead(p_lSchulter) >> 2);
  data[3] = 255 - (analogRead(p_rSchulter) >> 2);

  Serial.print( data[0] ); Serial.print(", "); Serial.print( data[1] ); Serial.print(", "); Serial.print( data[2] ); Serial.print(", "); Serial.print( data[3] ); Serial.print(", ");
  Serial.println( radio.write(&data, sizeof(data)) );
  delay(1);
}

void rgbWrite(int r, int g, int b) {
  digitalWrite(p_red, r);
  digitalWrite(p_green, g);
  digitalWrite(p_blue, b);
}

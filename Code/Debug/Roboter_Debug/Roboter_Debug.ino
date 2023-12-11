#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define p_red 8
#define p_green 7
#define p_blue 6

#define p_csn 11
#define p_ce 1

RF24 radio(p_ce, p_csn);

const byte address[6] = "00001";
void setup() {
  pinMode(p_red, OUTPUT);
  pinMode(p_green, OUTPUT);
  pinMode(p_blue, OUTPUT);

  radio.begin();
  radio.setChannel(200);
  radio.openReadingPipe(0, address);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

int blue_state = 0;

int bla[4];
void loop() {
  if (radio.available() ) {
    digitalWrite(p_green, 1);
    digitalWrite( p_blue, 0);
    digitalWrite( p_red, 0);
    radio.read(bla, sizeof(bla));
  }

  else if (radio.isChipConnected() ) {
    digitalWrite( p_blue, 1);
    digitalWrite( p_green, 0);
    digitalWrite( p_red, 0);
  }
  else {
    digitalWrite(p_green, 0);
    digitalWrite(p_blue, 0);
    digitalWrite( p_red, 1);
  }
}
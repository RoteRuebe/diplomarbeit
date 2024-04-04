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
  
  pinMode(p_lSchulter, INPUT);
  pinMode(p_rSchulter, INPUT);
  pinMode(p_joyX, INPUT);
  pinMode(p_joyY, INPUT);
  pinMode(p_joyButton, INPUT_PULLUP);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  delay(100);
  lcdSwish();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");
}

/** Main Loop **/

void loop() {
  crntMillis = millis();
  data[0] = analogRead(p_joyX)/2 - 255;
  data[1] = analogRead(p_joyY)/2 - 255;

  if (crntMillis - millisBoost <= BOOST_DURATION) {
    data[2] = 255 - (analogRead(p_lSchulter) >> 2);
    data[3] = 255 - (analogRead(p_rSchulter) >> 2);
  }
  
  else {
    // Subtract 80, but keep it above 0
    data[2] = max( (175-(analogRead(p_lSchulter) >> 2)), 0);
    data[3] = max( (175-(analogRead(p_rSchulter) >> 2)), 0);
  }

  if( radio.write(data, sizeof(data)) ) {
    millisLastPacket = crntMillis;
    robotConnected = 1;
  }
  
  else if (crntMillis - millisLastPacket >= 100) {
    robotConnected = 0;
  }

  if (crntMillis - prevMillisLoading >= 1000) {
    prevMillisLoading = crntMillis;

    if (loading < 16)
      loading ++;
    else {
      tone(750, p_buzzer, 500);
    }

    for(int i = 0; i < 16; i++) {
      if (i < loading) {
        lcd.setCursor(i, 0);
        lcd.print("#");
        lcd.setCursor(i, 1);
        lcd.print("#");
      }
      else {
        lcd.setCursor(i, 0);
        lcd.print(" ");
        lcd.setCursor(i, 1);
        lcd.print(" ");
      }
    }
  }

  if ( !digitalRead(p_joyButton) && loading == 16 ) {
    millisBoost = crntMillis;
    loading = 0;
  }

  if (robotConnected)
    rgbWrite(0, 1, 0);
  else
    rgbWrite(0, 0, 1);
  
  // Avoid crosstalk with other controllers / robots
  delay(10);
}

void rgbWrite(int r, int g, int b) {
  digitalWrite(p_red, r);
  digitalWrite(p_green, g);
  digitalWrite(p_blue, b);
}


void lcdSwish() {
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print("#");
    lcd.setCursor(i, 1);
    lcd.print("#");
    delay(10);
  }
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print(" ");
    lcd.setCursor(i, 1);
    lcd.print(" ");
    delay(10);
  }
}

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 3
#define S1 4
#define S2 5
#define S3 6
#define sensorOut 2

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int r_min, g_min, b_min = 9999;
int r_max, g_max, b_max = 0;

void full_calibration() {
  Serial.println("Now calibrating: red");
  Serial.println("Place Sensor close to red, then slowly move back");
  Serial.print("Starting in 3.."); delay(1000); Serial.print("2.."); delay(1000); Serial.print("1.."); delay(1000); Serial.println("Now!"); 
  calibrate(0);
  Serial.println("Red calibrated. Values are:");
  Serial.print("Min: ");
  Serial.println(r_min);
  Serial.print("Max: ");
  Serial.println(r_max);

  Serial.println("Now calibrating: green");
  Serial.println("Place Sensor close to green, then slowly move back");
  Serial.print("Starting in 3.."); delay(1000); Serial.print("2.."); delay(1000); Serial.print("1.."); delay(1000); Serial.println("Now!"); 
  calibrate(1);
  Serial.println("Red calibrated. Values are:");
  Serial.print("Min: ");
  Serial.println(g_min);
  Serial.print("Max: ");
  Serial.println(g_max);

  Serial.println("Now calibrating: blue");
  Serial.println("Place Sensor close to blue, then slowly move back");
  Serial.print("Starting in 3.."); delay(1000); Serial.print("2.."); delay(1000); Serial.print("1.."); delay(1000); Serial.println("Now!"); 
  calibrate(2);
  Serial.println("Red calibrated. Values are:");
  Serial.print("Min: ");
  Serial.println(b_min);
  Serial.print("Max: ");
  Serial.println(b_max);
}

void calibrate(int what) {
  switch (what) {
    case 0:
      //red
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);

      for (int i = 0; i < 10; i++) {
        Serial.println("Cheese!");
        redFrequency = pulseIn(sensorOut, LOW);
        if (redFrequency < r_min)
          r_min = redFrequency;
          
        else if (redFrequency > r_max)
          r_max = redFrequency;

        delay(500);
      }   
      break;

      case 1:
        //green
        digitalWrite(S2,HIGH);
        digitalWrite(S3,HIGH);
  
        for (int i = 0; i < 10; i++) {
          Serial.println("Cheese!");
          greenFrequency = pulseIn(sensorOut, LOW);
          if (greenFrequency < g_min)
            g_min = greenFrequency;
            
          else if (greenFrequency > g_max)
            g_max = greenFrequency;

          delay(500);
      }   
      break;

      case 2:
        //blue
        digitalWrite(S2,LOW);
        digitalWrite(S3,HIGH);
  
        for (int i = 0; i < 10; i++) {
          Serial.println("Cheese!");
          blueFrequency = pulseIn(sensorOut, LOW);
          if (blueFrequency < b_min)
            b_min = blueFrequency;
            
          else if (blueFrequency > b_max)
            b_max = blueFrequency;
  
          delay(500);
        }   
        break;
  }
}

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Begins serial communication
  Serial.begin(9600);
  full_calibration();
}


void loop() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, r_min, r_max, 255,0);
  
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);
  
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, g_min, g_max, 255, 0);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, b_min, b_max, 255, 0);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);

  if(redColor > greenColor && redColor > blueColor){
      Serial.println(" - RED detected!");
  }
  if(greenColor > redColor && greenColor > blueColor){
    Serial.println(" - GREEN detected!");
  }
  if(blueColor > redColor && blueColor > greenColor){
    Serial.println(" - BLUE detected!");
  }
}

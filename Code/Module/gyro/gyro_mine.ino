// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

int a_x_offset, a_y_offset, a_z_offset;
int g_x_offset, g_y_offset, g_z_offset;

int a_x, a_y, a_z;
int g_x, g_y, g_z;
int sum;
void setup(void) {
  Serial.begin(115200);
  mpu.begin();

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  Serial.print("Filter bandwidth set to: ");
  delay(100);

  //calibration
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  a_x_offset = a.acceleration.x;
  a_y_offset = a.acceleration.y;
  a_z_offset = a.acceleration.z;

  g_x_offset = g.gyro.x;
  g_y_offset = g.gyro.y;
  g_z_offset = g.gyro.z;
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  a_x = a.acceleration.x - a_x_offset;
  a_y = a.acceleration.y - a_y_offset;
  a_z = a.acceleration.z - a_z_offset;

  g_x = g.gyro.x - g_x_offset;
  g_y = g.gyro.y - g_y_offset;
  g_z = g.gyro.z - g_z_offset;

  sum = abs(a_x) + abs(a_y) + abs(a_z) + abs(g_x) + abs(g_y) + abs(g_z);
  /* Print out the values */
  Serial.print(a_x);
  Serial.print(",");
  Serial.print(a_y);
  Serial.print(",");
  Serial.print(a_z);
  Serial.print(",");
  Serial.print(g_x);
  Serial.print(",");
  Serial.print(g_y);
  Serial.print(",");
  Serial.print(g_z);
  Serial.print(",");
  Serial.print(sum);
  Serial.println("");

  //Serial.print("Temperature: ");
  //Serial.print(temp.temperature);
  //Serial.println(" degC");

  delay(100);
}

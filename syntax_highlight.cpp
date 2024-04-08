p_rf


int data[4] = [lSchulter, rSchulter, joyX, joyY]
lSchulter	0 (losgelassen)	… 255 (vollstaendig gedrueckt)
rSchulter	0 (losgelassen) 	… 255 (vollstaendig gedrueckt)
joyX 	-255 (ganz links) 	… 255 (ganz rechts)
joyY 	-255 (ganz unten) 	… 255 (ganz oben)

struct DataPayload {
    uint8_t zero;

    float gyroX;
    float gyroY; 
    float gyroZ;

    float accX;
    float accY;
    float accZ;

    float temp;
  
    uint8_t controllerConnected;
    uint8_t vibration;
};

char[32];


RF24 radio(p_CE, p_CSN);
radio.begin();

setup() {
	openReadingPipe(controllerAddress)
	openWritingPipe(serverAddress)
	startListening()
}

loop() {
	read(inputData)

	if ( 500ms passed ) {
		stopListening()

		write(sensorData)
		startListening()
	}
}

void configureRadio() {
    // Do nothing while radio module not connected
    while ( !radio.begin() ) {}
  
    openReadingPipe(controllerAddress)
    openWritingPipe(serverAddress)
    setPALevel(RF24_PA_MIN)
    setChannel(controllerChannel)
    setRetries(4, 10)
}

if (radio.failureDetected) {
    drive(0, 0);

    configureRadio();
    logMsg("Radio reconfigured after failure", 32);
}

-- controller.h –-

#ifdef controller1
    robotAddress = "c-r01";
    robotChannel = 25
#endif

#ifdef controller2
    robotAddress = "c-r02";
    robotChannel = 75
#endif 
...

-- controller.ino –

#define controller1
#include "controller.h"

loop() {
    data[0] = analogRead(p_joyX)/2 - 255
    data[1] = analogRead(p_joyY)/2 - 255
    data[2] = 255 - analogRead(p_lSchoulder)/4
    data[3] = 255 - analogRead(p_rShoulder)/4
    write(data)
}

if (crntMillis - millisBoost <= BOOST_DURATION) {
    data[2] = 255 - (analogRead(p_lSchulter) >> 2);
    data[3] = 255 - (analogRead(p_rSchulter) >> 2);
}
  
else {
    // Subtract 80, but keep it above 0
    data[2] = max( (175-(analogRead(p_lSchulter) >> 2)), 0);
    data[3] = max( (175-(analogRead(p_rSchulter) >> 2)), 0);
} 

-- robot.h –-

#ifdef robot1
    controllerAddress = “c-r01”
    serverAddress = “1-r-s”
#endif

#ifdef robot2
    controllerAddres = “c-r002”
    serverAddress = “2-r-s”
#endif
...

if (rSchulter > lSchulter)
    speed = rSchulter
else
    speed = -lSchulter

lMotor = lMotorTurn * speed
rMotor = rMotorTurn * speed

if (joyX < 0) {
    lMotorTurn = map(joyX, -1, 0, -1, 1)
    rMotorTurn = 1
}
else if (joyX >= 0) {
    lMotorTurn = 1
    rMotorTurn = map(joyX, 0, 1, 1, -1)
}
lMotor = lMotorTurn * speed
rMotor = rMotorTurn * speed

lMotor_soll = lMotorTurn * speed

if (lMotor_ist > lMotor_soll)
    lMotor_ist -= 1

else if (lMotor_ist < lMotor_soll)
    lMotor_ist += 1

drive(left, right) {
  left = -left
  if (left > 0) {
    analogWrite(p_leftForward, |left|)
    analogWrite(p_leftBackward, 0)
  }
  else {
    analogWrite(p_leftForward, 0)
    analogWrite(p_leftBackward, |left|)
  }
}


mpu.getEvent(&a, &g, &temp);

mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
mpu.setGyroRange(MPU6050_RANGE_500_DEG);
mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

int sendSensorData() {
    stopListening()

    sensors_event_t a, g, temp
    mpu.getEvent(&a, &g, &temp)

    struct DataPayload payload = {
        0x00,
        g.gyro.x, g.gyro.y, g.gyro.z, 
        a.acceleration.x, a.acceleration.y, a.acceleration.z, 
        temp.temperature,
        controllerConnected,
        digitalRead(p_vibration)
    }
    int response = radio.write(&payload, sizeof(payload))

    startListening()
    return response
}

if (packet_received) {
    timeLastPacket = crntTime
    controllerConnected = true
}

else if ( crntTime – timeLastPacket > 100)
    controllerConnected = false

int logMsg(char *x, int len) {
  int resp;
  radio.stopListening();
  resp = radio.write( &msg, len );

  radio.startListening();
  return resp;
}

### Shared Memory ###
gyroX=[deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
gyroY=[deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
...

accX = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
...

vibration=[deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
temp = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
log = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]

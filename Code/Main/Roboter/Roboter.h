
const int doDebug = 1;
const byte controllerAddress[6] = "00001";
const byte serverAddress[6] = "00002";
const int pushDataTimestamp = 215;


#define p_red 8
#define p_green 7
#define p_blue 6

#define p_lf 5  //Left Forward Pin
#define p_lb 2
#define p_rf 3
#define p_rb 4  //Right Backward Pin

//CSN, CE
RF24 radio(1, 11); 

void drive(int left, int right);
void rgbWrite(int r, int g, int b);

int speed = 0;
float turn;
float l_motor_turn, r_motor_turn;
int crntMillis, prevMillis = 0;
int lm_ist, rm_ist, lm_soll, rm_soll;

//JoyX, JoyY, l_Button, r_Button
int inputs[4];
int joyX, joyY, shoL, shoR;


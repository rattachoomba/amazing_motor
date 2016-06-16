//include Wire library to communicate with raspberry pi
//include Encoder library to read rotary position from the motor
#include <Wire.h>                      
#include <Encoder.h>
#define SLAVE_ADDRESS 0x48

// declare globle variables for i2c communication
byte number[255];
int number_bytes = 0;;
long result=0;
int i;
volatile float floatpt,floatnumber;

// declare globle variables for motor position reading
long newP = 0;
long oldP = 0;
unsigned long  newT = 0;
unsigned long oldT =0; 
long interval;
Encoder position(2, 3);
float theSpeed = 0;

// declare globle variable for PID controller, get ki from matlab simulation
float kp = 0;
float kd = 0;
float ki = 2.235;
float integral = 0;


void setup() {
  Serial.begin(9600);         // start serial for output
  Wire.begin(SLAVE_ADDRESS);  // initialize i2c as slave
  Wire.onReceive(receiveData);// define callbacks for i2c communication
  Serial.println("Ready!");  
  pinMode(4,OUTPUT);          // setup pins 4,7,8,9,10 for output
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(12,INPUT);          // set up pin 12 for input
  digitalWrite(4,HIGH);       // set the enable pin to high
}

void loop() {
  newT= millis();             // use built-in function millis() to record time
  newP = position.read();     // read position from motor
  if (newT > oldT){           // calculate the actual speed = distance/time 
    theSpeed = (float) (newP-oldP)/(newT - oldT)*1000*(2*3.1415926)/3200.0;
  }
  float desiredspeed = (floatpt/10.0);          // setpoint of 1 rad/s
  float e = desiredspeed - theSpeed;            // error
  integral = integral + ((newT- oldT)/1000.0)*e;//
  float u_output = ki*integral;                 // output voltage
  if (u_output > 6){                            // set a limit of 6 volts to avoid wind-up
    u_output = 6;
    integral = (u_output - kp*e)/ki;
  }
  if (u_output < -6){
    u_output=-6;
    integral = (u_output - kp*e)/ki;
  }
  float input_value = (u_output/6.0)*255.0;      //scale the output, 0 to 255
  analogWrite(9,min(abs(input_value),255));      //write PWM to pin 9 to control speed
  delay(10);
  oldT = newT;                  // time period
  oldP = newP;                  // distance travelled
} 




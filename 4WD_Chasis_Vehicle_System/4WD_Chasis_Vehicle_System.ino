// Setup the servo motor
#include <Servo.h>

Servo myservo;

int servposnum = 0;
int servpos = 0;

// Setup Motor A (front and rear) pins
#define ENABLE_MOTOR_A  1
#define MOTOR_A_PIN_1  3
#define MOTOR_A_PIN_2  2

// Setup Motor B (front and rear) pins
#define ENABLE_MOTOR_B  6
#define MOTOR_B_PIN_1  5
#define MOTOR_B_PIN_2  4

// Setup Ultrasonic Sensor pins
#define FROUND_ULTRASONIC_TRIGGER_PIN 8
#define FROUND_ULTRASONIC_ECHO_PIN 9

void setup() {
  
  // The setup code goes here and runs once only
  // Configure the pin modes for each drive motor
  
   pinMode (ENABLE_MOTOR_A, OUTPUT);
   pinMode (MOTOR_A_PIN_1, OUTPUT);
   pinMode (MOTOR_A_PIN_2, OUTPUT);
   pinMode (ENABLE_MOTOR_B, OUTPUT);
   pinMode (MOTOR_B_PIN_1, OUTPUT);
   pinMode (MOTOR_B_PIN_2, OUTPUT); 
   
   // Configure the pin modes for the Ultrasonic Sensor
   
   pinMode(FROUND_ULTRASONIC_TRIGGER_PIN, OUTPUT);
   pinMode(FROUND_ULTRASONIC_ECHO_PIN, INPUT);
   
   // Turn pin into servo driver. Calls pinMode. Returns 0 on failure.
   myservo.attach(7);
}

void loop() {
  // Main code goes here and will run repeatedly:
     car(); // function keeps moving car forward while distance > 15cm
     avoid(); // function makes car go back, turn slightly right to move forward in new direction
     
}

/** 
 * Create motor functions 
 */

void motorAforward() {
 digitalWrite (MOTOR_A_PIN_1, HIGH);
 digitalWrite (MOTOR_A_PIN_2, LOW);
}

void motorBforward() {
 digitalWrite (MOTOR_B_PIN_1, LOW);
 digitalWrite (MOTOR_B_PIN_2, HIGH);
}

void motorAbackward() {
 digitalWrite (MOTOR_A_PIN_1, LOW);
 digitalWrite (MOTOR_A_PIN_2, HIGH);
}

void motorBbackward() {
 digitalWrite (MOTOR_B_PIN_1, HIGH);
 digitalWrite (MOTOR_B_PIN_2, LOW);
}

void motorAstop() {
 digitalWrite (MOTOR_A_PIN_1, HIGH);
 digitalWrite (MOTOR_A_PIN_2, HIGH);
}

void motorBstop() {
 digitalWrite (MOTOR_B_PIN_1, HIGH);
 digitalWrite (MOTOR_B_PIN_2, HIGH);
}

void motorAcoast() {
 digitalWrite (MOTOR_A_PIN_1, LOW);
 digitalWrite (MOTOR_A_PIN_2, LOW);
}

void motorBcoast() {
 digitalWrite (MOTOR_B_PIN_1, LOW);
 digitalWrite (MOTOR_B_PIN_2, LOW);
}

void motorAon() {
 digitalWrite (ENABLE_MOTOR_A, HIGH);
}

void motorBon() {
 digitalWrite (ENABLE_MOTOR_B, HIGH);
}

void motorAoff() {
 digitalWrite (ENABLE_MOTOR_A, LOW);
}

void motorBoff() {
 digitalWrite (ENABLE_MOTOR_B, LOW);
}

/** 
 * Setup movement functions 
 */
 
void forward (int duration) {
 motorAforward();
 motorBforward();
 delay (duration);
}

void backward (int duration) {
 motorAbackward();
 motorBbackward();
 delay (duration);
}

void right (int duration) {
 motorAbackward();
 motorBforward();
 delay (duration);
}

void left (int duration) {
 motorAforward();
 motorBbackward();
 delay (duration);
}

void coast (int duration) {
 motorAcoast();
 motorBcoast();
 delay (duration);
}

void breakRobot (int duration) {
 motorAstop();
 motorBstop();
 delay (duration);
}

void disableMotors() {
 motorAoff();
 motorBoff();
}

void enableMotors() {
 motorAon();
 motorBon();
}

/* 
 * Setup Ultrasonic Sensor distance measuring 
 */
 
int distance() {
  
  int duration, distance;
  digitalWrite(FROUND_ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(FROUND_ULTRASONIC_TRIGGER_PIN, LOW);
  duration = pulseIn(FROUND_ULTRASONIC_ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
  
}

/* 
 * Setup the main car function 
 */
 
void car() {
  int distance_0;
  distance_0 = distance();
    // Keep moving forward in a straight line while distance of objects > 15cm
    while(distance_0 > 15)
    {
       // Keep moving servo motor back and forth to scan surroundings
       // This allows the ultrasonic sensor to see more to its left and right
       if(servposnum == 0)
       {
        myservo.writeMicroseconds (1900);
        servposnum = 1;
        delay(100);     
       }
       else if(servposnum == 1)
       {
        myservo.writeMicroseconds (2200);
        servposnum = 2;
       delay(100);
       }
       else if(servposnum  == 2)
       {
        myservo.writeMicroseconds (1900);
        servposnum = 3;
        delay(100);
       }
       else if(servposnum == 3)
       {
        myservo.writeMicroseconds (1600);
        servposnum = 1;
        delay(100);
       }
       motorAon();
       motorBon();
       forward(1);    
       distance_0 = distance();
       
    }
    breakRobot(0);
}

void avoid(){
  
    // Go back and turn slightly right to move car in new direction if object detected < 15cm away
    
    backward(500);
    right(360);
    
}

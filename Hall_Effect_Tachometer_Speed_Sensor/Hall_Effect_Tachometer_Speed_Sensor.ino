#include <Wire.h>

#define DEBUG true

#define TACHOMETER_SYSTEM_ADDRESS 0xC3

#define HALL_SENSOR_PIN 2

volatile byte counter; //for incrementing each time the wheel truns

unsigned int roundsPerMinute;

unsigned long elapsedTime;

/**
 * This interrupt function is run twice for each revolution
 */
void interuptServiceRoutine(){
  counter++;
}

void setup() {

  Wire.begin(TACHOMETER_SYSTEM_ADDRESS);
  Wire.onRequest(requestEventHandler);

  if(DEBUG){
    Serial.begin(9600);
  }

  attachInterrupt(0, interuptServiceRoutine, RISING);

  pinMode(HALL_SENSOR_PIN, INPUT);

  counter = 0;

  roundsPerMinute = 0;

  elapsedTime = 0;

}

void loop() {
  
  delay(1000); //Update RPM every second

  detachInterrupt(0); //Interrupts are disabled
  
  roundsPerMinute = 60*1000/(millis() - elapsedTime)*counter;
  
  elapsedTime = millis();
  
  counter = 0;
  
  if(DEBUG){
    
    Serial.print("RPM=");
  
    Serial.println(roundsPerMinute); //Print out result to monitor
    
  }
  
  attachInterrupt(0, interuptServiceRoutine, RISING);   //Restart the interrupt processing
  
}

void requestEventHandler(){
  Wire.write("SPD:");
  Wire.write(roundsPerMinute);
  Wire.write("\n");
}


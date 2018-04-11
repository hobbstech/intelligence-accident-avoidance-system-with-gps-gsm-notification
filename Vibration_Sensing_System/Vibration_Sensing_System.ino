/**
 * This sketch reads a piezo element to detect a knocking sound.
 * It reads an analog pin and compares the result to a set threshold.
 * If the result is greater than the threshold, it writes "knock" to the serial
 * port
 * 
 * The circuit:
 *   - positive connection of the piezo attached to analog in 0
 *   - negative connection of the piezo attached to ground
 *   - 1 megohm resistor attached from analog in 0 to ground
 */

#include <Wire.h>

#define DEBUG true

#define VIBRATION_SYSTEM_ADDRESS 0xC7

#define PIEZO_PIN A0

const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not

boolean highImpactOccured = false;

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin

void setup() {

  Wire.begin(VIBRATION_SYSTEM_ADDRESS); // Joins the uC to the I2C bus as a slave with the given address
  Wire.onRequest(onRequestHandler);
  Serial.begin(9600);       // use the serial port
}

void loop() {
  
  // read the sensor and store it in the variable sensorReading:
  int  piezoADC = analogRead(PIEZO_PIN);

  #ifdef DEBUG
    float piezoV = piezoADC / 1023.0 * 5.0;
    Serial.print("Piezo Voltage : ");
    Serial.println(piezoV);
  #endif
  
  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    highImpactOccured = true;
  }else{
    highImpactOccured = false;
  }
  
}

void onRequestHandler(){

 if(highImpactOccured){
   Wire.write("IMP:");
   Wire.write("YES");
 }else{
   Wire.write("IMP:");
   Wire.write("NO");
 }
  
}


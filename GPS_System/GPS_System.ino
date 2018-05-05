#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define GPS_SYSTEM_ADDRESS 0xC2

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600; //Replace with your baud rate

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Wire.begin(GPS_SYSTEM_ADDRESS);
  Wire.onRequest(requestHandler);
  
}

void requestHandler(){

  //sending the time to master
  Wire.print('T');
  Wire.print(gps.time.hour());Wire.print(':');
  Wire.print(gps.time.minute());Wire.print(':');
  Wire.print(gps.time.second());
  
  //sending the location to master
  Wire.print("L");
  Wire.print(gps.location.lat(), 6);
  Wire.print(",");
  Wire.print(gps.location.lng(), 6);
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      //displayInfo();
      Serial.print('T');
      Serial.print(gps.time.hour());Serial.print(':');
      Serial.print(gps.time.minute());Serial.print(':');
      Serial.print(gps.time.second());
      
      //sending the location to master
      Serial.print("L");
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.print(gps.location.lng(), 6);
      Serial.println();
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

//void displayInfo()
//{
//  Serial.print(F("Location: ")); 
//  if (gps.location.isValid())
//  {
//    Serial.print(gps.location.lat(), 6);
//    Serial.print(F(","));
//    Serial.print(gps.location.lng(), 6);
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
//
//  Serial.print(F("  Date/Time: "));
//  if (gps.date.isValid())
//  {
//    Serial.print(gps.date.month());
//    Serial.print(F("/"));
//    Serial.print(gps.date.day());
//    Serial.print(F("/"));
//    Serial.print(gps.date.year());
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
//
//  Serial.print(F(" "));
//  if (gps.time.isValid())
//  {
//    if (gps.time.hour() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.hour());
//    Serial.print(F(":"));
//    if (gps.time.minute() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.minute());
//    Serial.print(F(":"));
//    if (gps.time.second() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.second());
//    Serial.print(F("."));
//    if (gps.time.centisecond() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.centisecond());
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
//
//  Serial.println();
//}

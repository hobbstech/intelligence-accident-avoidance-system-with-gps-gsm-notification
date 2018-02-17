#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define GPS_TRANSMITTER_PIN 2   // The pin of the arduino that is connected to the rx of the gps shield
#define GPS_RECEIVER_PIN 3      // The pin of the arduino that is connected to the tx of the gps shield

#define DEBUG_GPSECHO true //set to false to turn off GPS data echoing/send to Serial console

#define GPS_SYSTEM_ADDRESS 0xC2

SoftwareSerial GPSSerial(GPS_TRANSMITTER_PIN, GPS_RECEIVER_PIN);

Adafruit_GPS GPS(&GPSSerial); 
/**
 * if using the hardware serial then the software serial above is not neccessary
 * hence use the following to build the Adafruit_GPS object
 * 
 * Adafruit_GPS GPS(&Serial1);
 * 
 */

void setup() {

  Wire.begin(GPS_SYSTEM_ADDRESS);
  Wire.onRequest(requestHandler);
  Wire.onReceive(receiveHandler);

  if(DEBUG_GPSECHO){
    
    Serial.begin(115200);
    Serial.println("Initializing the GPS, in the Debug Mode");
 
  }

  GPS.begin(9600); //default baudrate for the gps breakout, some use 4800

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  //turning on RMC (recommended minimum) and GGA (fix data) including altitude

  // Set the update rate
  // Note you must send both commands below to change both the output rate (how often the position
  // is written to the serial line), and the position fix rate.
  // 1 Hz update rate- for 9600 baud you'll have to set the output to RMC or RMCGGA only (see above)
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);

  GPS.sendCommand(PGCMD_ANTENNA);   //Can report if antenna is connected or not
  
  delay(500);

  delay(500);
  Serial.print("\nSTARTING LOGGING....");
  if (GPS.LOCUS_StartLogger())
    Serial.println(" STARTED!");
  else
    Serial.println(" no response :(");
  delay(500);

  
  //GPS.sendCommand("$PMTK622,1*29");
  
}

void loop() {

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA()); //This is going to parse the last NMEA sentence the Arduino has received, breaking it down into its constituent parts.
  }

  
  if(DEBUG_GPSECHO){
    
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    
    if (GPS.fix) {
      
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", ");
      Serial.println(GPS.longitudeDegrees, 4);
  
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      
    }
    
  }
  
}

void requestHandler(){

  //sending the time to master
  Wire.print('T');
  Wire.print(GPS.hour, DEC);Wire.print(':');
  Wire.print(GPS.minute, DEC);Wire.print(':');
  Wire.print(GPS.seconds, DEC);Wire.print('.');
  Wire.print(GPS.milliseconds);
  
  //Sending the date to master
  Wire.write('D');
  Wire.print(GPS.day, DEC); Wire.print('/');
  Wire.print(GPS.month, DEC); Wire.print("/20");
  Wire.println(GPS.year, DEC);
  
  //sending the location to master
  Wire.print("LOC");
  Wire.print(GPS.latitude, 4); Wire.print(GPS.lat);
  Wire.print(",");
  Wire.print(GPS.longitude, 4); Wire.println(GPS.lon);

  //Sending the location in degree to master
  Wire.print("LOCD");
  Wire.print(GPS.latitudeDegrees, 4);
  Wire.print(",");
  Wire.println(GPS.longitudeDegrees, 4);

  //Sending addition information about the relative position to master
  Wire.print("SP"); Wire.println(GPS.speed);
  Wire.print("ANG"); Wire.println(GPS.angle);
  Wire.print("ALT"); Wire.println(GPS.altitude);
  Wire.print("SAT"); Wire.println((int)GPS.satellites);
  
}

void receiveHandler(int byteReceived){}



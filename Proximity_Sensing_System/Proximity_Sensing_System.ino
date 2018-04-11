/**
 * @author Wilson Chiviti
 * @Date : 11/04/2018
 * 
 * HC-SR04, measures distances from 2 cm up to 4oo cm with an accuracy of ±3 mm.
 * 
 * Ultrasonic range finders measure distance by emitting a pulse of ultrasonic 
 * sound that travels through the air until it hits an object. When that pulse 
 * of sound hits an object, it’s reflected off the object and travels back to 
 * the ultrasonic range finder. The ultrasonic range finder measures how long 
 * it takes the sound pulse to travel in its round trip journey from the sensor 
 * and back. It then sends a signal to the Arduino with information about how 
 * long it took for the sonic pulse to travel.
 * 
 * Knowing the time it takes the ultrasonic pulse to travel back and forth to 
 * the object, and also knowing the speed of sound, the Arduino can calculate 
 * the distance to the object. The formula relating the speed of sound, 
 * distance, and time traveled is:
 * 
 * distance = speed * time
 * 
 * The time variable is the time it takes for the ultrasonic pulse to leave 
 * the sensor, bounce off the object, and return to the sensor. We actually 
 * divide this time in half since we only need to measure the distance to the 
 * object, not the distance to the object and back to the sensor. The speed 
 * variable is the speed at which sound travels through air.
 * 
 * The speed of sound in air changes with temperature and humidity. Therefore, 
 * in order to accurately calculate distance, we’ll need to consider the ambient 
 * temperature and humidity. The formula for the speed of sound in air with 
 * temperature and humidity accounted for is:
 * 
 * speed = 331.4 + (0.606 x Temprature) + (0.0124 x H)
 * 
 * For example, at 20 °C and 50% humidity, sound travels at a speed of:
 * 
 * speed = 331.4 + (0.606 x 20) + (0.0124 x 50)  
 * speed = 344.02 m/s 
 */

#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>

#define PROXIMITY_SYSTEM_ADDRESS    0xC5 //The Address that will be used by the I2C protocol

#define DHTPIN                      4 //pin for the digital temprature and humidity sensor

#define DHTTYPE                     DHT11 // type of dhtxx series to be used

#define ULTRASONIC_TRIG_PIN_FRONT        9

#define ULTRASONIC_ECHO_PIN_FRONT        10

#define ULTRASONIC_TRIG_PIN_REAR        11

#define ULTRASONIC_ECHO_PIN_REAR        12

#define MINIMUM_PROXIMAL_DISTANCE   1 //distance in metres which will be the minimum safest distances

float humidity; // humidity value from the dht11 sensor

float dht11temp; // temprature value from the dht11 sensor

boolean proximityFrontDanger = false;

float proximityFrontDistance;

boolean proximityRearDanger = false;

float proximityRearDistance;

unsigned int duration;

DHT dht11(DHTPIN,DHTTYPE );

void setup() {

  Wire.begin(PROXIMITY_SYSTEM_ADDRESS);

  Wire.onRequest(onRequestEventHandler);
  
  Serial.begin(9600);

  pinMode(ULTRASONIC_TRIG_PIN_FRONT, OUTPUT);

  pinMode(ULTRASONIC_ECHO_PIN_FRONT, INPUT);

  pinMode(ULTRASONIC_TRIG_PIN_REAR, OUTPUT);

  pinMode(ULTRASONIC_ECHO_PIN_REAR, INPUT);

}

void loop() {

  dht11temp = readDHT11Temp(dht11); //read the dht11 sensor values for temprature
  humidity = readDHT11Humidity(dht11);//read the dht11 sensor values for humidity

  digitalWrite(ULTRASONIC_TRIG_PIN_FRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN_FRONT, LOW);

  duration = pulseIn(ULTRASONIC_ECHO_PIN_FRONT, HIGH);
  float durationInSeconds = duration / 1000000;
  float soundSpeed = 331.4 + (0.606 * dht11temp) + (0.0124 * humidity);
  float distance = soundSpeed * durationInSeconds;

  if(distance < MINIMUM_PROXIMAL_DISTANCE){

    proximityFrontDanger = true;
    proximityFrontDistance = distance;
    
  }else{

    proximityFrontDanger = false;
    
  }

  digitalWrite(ULTRASONIC_TRIG_PIN_REAR, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN_REAR, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN_REAR, LOW);
  
  duration = pulseIn(ULTRASONIC_ECHO_PIN_REAR, HIGH);
  durationInSeconds = duration / 1000000;
  soundSpeed = 331.4 + (0.606 * dht11temp) + (0.0124 * humidity);
  distance = soundSpeed * durationInSeconds;

  if(distance < MINIMUM_PROXIMAL_DISTANCE){

    proximityRearDanger = true;
    proximityRearDistance = distance;
    
  }else{

    proximityRearDanger = false;
    
  }
}

float readDHT11Temp (DHT dht){ 
  return dht.readTemperature();  
}

float readDHT11Humidity(DHT dht){
  return dht.readHumidity();
}

void onRequestEventHandler(){

  if(proximityFrontDanger){
    
    Wire.write("F");
    String frontDistance = String(proximityFrontDistance);
    Wire.write(frontDistance.c_str());
    Wire.write(":");
    
  }else{

    Wire.write("F");
    Wire.write(0);
    Wire.write(":");
    
  }

  if(proximityRearDanger){
    
    Wire.write("R");
    String rearDistance = String(proximityRearDistance);
    Wire.write(rearDistance.c_str());
    Wire.write(";");
    
  }else{

    Wire.write("R");
    Wire.write(0);
    Wire.write(";");
    
  }
   
}

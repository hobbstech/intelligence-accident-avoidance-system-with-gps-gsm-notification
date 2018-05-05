
#define LEFT_SIDE_TILT_SENSOR_IN 4

void setup() {

  Serial.begin(9600);

  pinMode(LEFT_SIDE_TILT_SENSOR_IN, INPUT);

  pinMode(3, OUTPUT);
  
}

void loop() {

  while(digitalRead(LEFT_SIDE_TILT_SENSOR_IN)){

    Serial.println(HIGH);
    
  }

  digitalWrite(3, LOW);

}

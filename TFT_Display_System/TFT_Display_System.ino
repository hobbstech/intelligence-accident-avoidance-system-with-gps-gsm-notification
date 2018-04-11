#include <Wire.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#include <SD.h>

#define DISPLAY_ADDRESS 0xC4

#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8
#define _sd_cs 7

#define BACKGROUND_COLOR ILI9340_BLACK
#define FOREGROUND_COLOR ILI9340_WHITE

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst); //initialising the Adafruit TFT screen

void setup() {
  // put your setup code here, to run once:

  Wire.begin(DISPLAY_ADDRESS); //Initialize the wire library for I2C communication

  Serial.begin(9600); //intialize the Serial port communication

  tft.begin(); //initialize the tft display 
  
  Serial.print("Testing Screen");
  
  Serial.println(testFillScreen());
  
  tft.fillScreen(BACKGROUND_COLOR); //fill the tft screen with background color
  
  tft.setTextColor(FOREGROUND_COLOR);//fill the tft screen with foreground color
  
  tft.setTextSize(2); //set the text size and font for the TFT display

  SD.begin(_sd_cs); //this initializes saving to the sd card

  Wire.onReceive(wireDataReceiveEvent);

}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
 * Testing the TFT screen, by filling with different colours
 */

unsigned long testFillScreen() {
  
  unsigned long start = micros();
  
  tft.fillScreen(ILI9340_BLACK);
  
  tft.fillScreen(ILI9340_RED);
  
  tft.fillScreen(ILI9340_GREEN);
  
  tft.fillScreen(ILI9340_BLUE);
  
  tft.fillScreen(ILI9340_BLACK);
  
  return micros() - start;
  
}

float mapFloat(float x, float in_min, float in_max, float out_max, float out_min) {
  
  return (x - in_min) * (out_max - out_min) / (in_max - in_min);
  
}

void wireDataReceiveEvent(int receivedDataSize){

  Serial.println("Received data from the master : ");
  Serial.print("Size : ");
  Serial.println(receivedDataSize);
  
}


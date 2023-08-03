// This code uses the calibration values stored in the Arduino's EEPROM by the calibration code.
  
// Including the libraries
#include <Arduino.h>
#include "HX711.h"         //Including the library HX711
#include <EEPROM.h>        //Including the EEPROM library to save the calibration value
#include <TM1637Display.h> //Including the TM1637 display library

// HX711 wiring
#define LOADCELL_DOUT_PIN 5
#define LOADCELL_SCK_PIN 6
HX711 scale; // creating the scale object

// Display wiring
#define CLK_PIN 4
#define DIO_PIN 3
TM1637Display display(CLK_PIN, DIO_PIN);

// Tare  button wiring
#define Button 2

const byte calibValAddress = 0;  // EEPROM memory address used for storing the calibration value in KitchenScaleCalibrationCode
const byte calibSignAddress = 1; // EEPROM memory address used for storing the sign of calibration value in KitchenScaleCalibrationCode

const int displayBrightness = 5;

// defining the variable used for delay
const int halfSecDelay = 500;


void tareFunction(void)
{
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    display.setSegments(data);
    scale.tare();
}

void scaleCalibInput()
{
  int calibVal = EEPROM.read(calibValAddress);
  int calibValSign = EEPROM.read(calibSignAddress);
  if (calibValSign == 1) // if calibration value is positive, we save have stored one as its sign. Otherwise, it was set to zero.
  {
    scale.set_scale(calibVal);
  }
  else
  {
    scale.set_scale(-calibVal);
  }
  scale.tare();
}

void setup()
{
  // initiating the scale
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // bringing the tare button to the high state
  pinMode(Button, INPUT_PULLUP);

  scaleCalibInput(); // inputting the scale calibration value found by the KitchenScaleCalibrationCode

  display.setBrightness(displayBrightness);
}

void loop()
{
  String weightOnScale = String(scale.get_units(5), 1); // taking the average of five scale readings
  int weightOnDisplay = weightOnScale.toInt(); // converting the scale readings to an int value
  display.showNumberDecEx(weightOnDisplay, false); //displaying the scale readings on the display

  if (digitalRead(Button) == LOW) // scale will be tared when the button is pressed
  {
    tareFunction();
    
  }
}

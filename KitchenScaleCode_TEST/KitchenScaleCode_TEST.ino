// This code uses the calibration values stored in the Arduino's EEPROM by the calibration code "KitchenScaleCalibrationCode.h"

// Including the libraries
#include <Arduino.h>
#include "HX711.h"         //Including the library HX711
#include <EEPROM.h>        //Including the EEPROM library to save the calibration value
#include <TM1637Display.h> //Including the TM1637 display library

// HX711 wiring
#define LOADCELL_DOUT_PIN 5
#define LOADCELL_SCK_PIN 6
0,0HX711 scale; // creating the scale object

// Display wiring
#define CLK_PIN 3
#define DIO_PIN 4
TM1637Display display(CLK_PIN, DIO_PIN);

// Tare  button wiring
const byte interruptPin = 2;
//volatile byte state = LOW;

const byte calibValAddress = 0;  // EEPROM memory address used for storing the calibration value in KitchenScaleCalibrationCode
const byte calibSignAddress = 1; // EEPROM memory address used for storing the sign of calibration value in KitchenScaleCalibrationCode

const int displayBrightness = 3;

byte doTare = 0; // this will act as a flage for taring the scale at the press of interrupt switch

void tareFunction(void) //this function will run at the press of the interrupt switch
{
  doTare = 1;
}

//this function will read the calibration values from EEPROM and set it the the scale
//the calibration values were stored in EEPROM by KitchenScaleCalibrationCode
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
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // initiating the scale

  // interrupt pin will be used to change a flag which will be used to run the tare function
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), tareFunction, FALLING);

  scaleCalibInput(); // inputting the scale calibration value found by the KitchenScaleCalibrationCode

  display.setBrightness(displayBrightness); // setting up the display brightness

}

void loop()
{
  
  String weightOnScale = String(scale.get_units(5), 1); // taking the average of five scale readings
  int weightOnDisplay = weightOnScale.toInt();          // converting the scale readings to an int value
  display.showNumberDecEx(weightOnDisplay, false);      // displaying the scale readings on the display

  if (doTare == 1) // the flag doTare will be changed by the press of the interrupt switch
  {
    Serial.println("The button was pressed");
    //brightening up the whole display
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    display.setSegments(data);
    
    scale.tare(); //running the tare function
    
    doTare = 0; //changing the doTare flag
  }
  
}

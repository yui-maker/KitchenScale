// This code uses the calibration values stored in the Arduino's EEPROM by the calibration code "KitchenScaleCalibrationCode.ino"

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
#define CLK_PIN 3
#define DIO_PIN 4
TM1637Display display(CLK_PIN, DIO_PIN);

// Tare  button wiring
const byte interruptPin = 2;
//volatile byte state = LOW;

const int calibValLenAddress = 0;  //EEPROM memory address used for storing the length of the calibration value in KitchenScaleCalibrationCode

const int calibValAddress = 1; // EEPROM memory address where first digit of calibration value was stored in KitchenScaleCalibrationCode

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
  int calibValRedLength = EEPROM.read(calibValLenAddress); //reading the length of calibration value
  Serial.println("Length of stored digits : " + String(calibValRedLength));

  String readVal = ""; //for reading digits from EEPROM and concatenating them
  for (int i = 0; i < calibValRedLength; i++) // Waiting for the next ten seconds for the user to put the known weight on the scale
  {
    readVal += EEPROM.read(calibValAddress + i);
  }
  int readValInt = readVal.toInt();

  if (EEPROM.read(calibValAddress) == 0) // if the first digit of stored EEPROM calibration digits is zero, it indicates a negative calibration value
  {
    readValInt = -readValInt;
  }

  Serial.println("Calibration value stored in EEPROM : " + String(readValInt));
  
  scale.set_scale(readValInt); //calibrating the scale based on the calibration value that was stored in EEPROM by KitchenScaleCalibrationCode

  scale.tare();
}

void setup()
{
  Serial.begin(57600);
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
    //brightening up the whole display
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    display.setSegments(data);

    scale.tare(); //running the tare function

    doTare = 0; //changing the doTare flag
  }

}

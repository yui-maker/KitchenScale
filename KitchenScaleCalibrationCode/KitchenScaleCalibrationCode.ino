// This code is to store the scale's calibration value to EEPROM

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

const int knownWeight = 189; // modify this value to the weight of the object you will use for calibration. (As I don't have another scale to know weights of other objects I am using my phone).

const int calibValLenAddress = 0;  //storing length of the calibration value so Arduino knows how much we need to read in calibration code

const int calibValAddress = 1; // start value of the EEPROM memory address for storing the calibration value

const int displayBrightness = 5;

// defining the variable used for delay
const int oneSecDelay = 1000;
const int halfSecDelay = 500;
const int fiveSecLoop = 5;
const int tenSecLoop = 10;

void scaleCalibration(void)
{
  if (scale.is_ready())
  {
    scale.set_scale();
    Serial.println("Please remove any weights from the scale within the next 5 seconds.");
    delayDisplayLoop(fiveSecLoop); // running an animation on display for the next 5 seconds
    Serial.println("Starting scale tare.");
    scale.tare();
    Serial.println("Tare accomplished.");
    Serial.println("Please place the known weight on the scale within the next 10 seconds.");
    for (int i = 0; i < tenSecLoop; i++) // Waiting for the next ten seconds for the user to put the known weight on the scale
    {
      delay(oneSecDelay);
      Serial.println("Time left: " + String(tenSecLoop - i) + " seconds");
    }
    long reading = scale.get_units(10);    // taking the average of ten scale readings
    int calibVal = reading / knownWeight; // dividing the average scale reading with the known weight to find the calibration value and converting it to int
    scale.set_scale(calibVal);             // calibrating the scale

    Serial.println("Non-calibrated scale reading: " + String(reading));
    Serial.println("CalibValue: " + String(calibVal));
    
    //storing data to EEPROM
    String calibValForEEPROM = String(calibVal); // converting the calibration value to string to find it's length
    int calibValLength = calibValForEEPROM.length(); //finding length of calibration value

    EEPROM.update(calibValLenAddress, calibValLength);
    Serial.println("Calibration value length : " + String(calibValLength));

    for (int i = 0; i < calibValLength; i++) // going through each digit in calibration value and storing them to EEPROM
    {
      EEPROM.update(calibValAddress + i, calibValForEEPROM.substring(i, i + 1).toInt()); //reading subsections of the string calibValForEEPROM and storing them in EEPROM
    }

    //For printing the value written in EEPROM to confirm we have stored the right value
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
    
    //continuing with the calibration process
    Serial.println("Please remove the weight from the scale within the next 5 seconds.");
    delayDisplayLoop(fiveSecLoop); // running an animation on display for the next 5 seconds

    Serial.println("Starting scale tare.");
    scale.tare();
    Serial.println("Tare accomplished.");
  }
  else
  {
    Serial.println("HX711 not found.");
  }
  delay(oneSecDelay);
}

void delayDisplayLoop(int delayTime)
{
  for (int i = 0; i < delayTime; i++)
  {
    display.setBrightness(displayBrightness);
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    display.setSegments(data);
    delay(halfSecDelay);
    display.clear();
    display.setBrightness(0); // Turn off the display
    delay(halfSecDelay);
    Serial.println("Time left: " + String(delayTime - i) + " seconds");
  }
  display.setBrightness(displayBrightness);
}

void setup()
{
  Serial.begin(57600); // make sure your serial monitor is set to the same baud rate.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  display.setBrightness(displayBrightness);
  scaleCalibration(); // running the scale calibration function
}

void loop()
{
  String weightOnScale = String(scale.get_units(5), 1); // taking the average of five scale readings
  int weightOnDisplay = weightOnScale.toInt();          // converting the scale readings to an int value
  display.showNumberDecEx(weightOnDisplay, false); //displaying the scale readings on the display
  Serial.println("Weight on scale: " + String(weightOnDisplay));
  delay(halfSecDelay);
}

//https://randomnerdtutorials.com/arduino-load-cell-hx711/
//This code can Kitchen scale weights

//Including the libraries
#include <Arduino.h>
#include "HX711.h"  //Including the scale amplifier library HX711
#include <EEPROM.h> //Including the EEPROM to save the calibration value
#include <TM1637Display.h>  //Including the display library



// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;
HX711 scale;
int knownWeight = 189; //use a known weight to calibrate, I am using my phone.

// defining the button
#define tareButt 2
unsigned long Timestamp_Button_Pressed;
int address = 0; //EEPROM memory address for storing the calibration value

// display wiring
#define CLK 4
#define DIO 3
TM1637Display display(CLK, DIO);

void scaleCalibration(void) {
  if (scale.is_ready()) {
    scale.set_scale();
    Serial.println("Tare... remove any weights from the scale.");
    //Add an animation here to remove weight.
    for (int i = 0; i < 5; i++)
    {
      display.setBrightness(5);
      uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
      display.setSegments(data);
      delay(500);
      display.clear();
      display.setBrightness(0); // Turn off the display
      delay(500);
    }
    byte bottomPatterns[] = {
      B00000001,  // Bottom cell of first row
      B00000001,//B00000010,  // Bottom cell of second row
      B00000001,//B00000100,  // Bottom cell of third row
      B00000001,//B00001000   // Bottom cell of fourth row
    };
      for (int i = 0; i < sizeof(bottomPatterns); i++) {
    display.setSegments(&bottomPatterns[i], 1, i); // Set bottom cell pattern for specific row
    delay(1000);  // Delay between row patterns
  }
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    //Let the user input their known weight and wait till they have input it.
    //Save the user input known weight in EEPROM
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
    long calibVal = reading / knownWeight;
    Serial.print("CalibValue: ");
    Serial.println(calibVal);
    EEPROM.update(address, abs(calibVal));
    scale.set_scale(reading / knownWeight);
    Serial.println("Remove your weight");
    {
      display.setBrightness(5);
      uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
      display.setSegments(data);
      delay(500);
      display.clear();
      display.setBrightness(0); // Turn off the display
      delay(500);
    }
    scale.tare();
  }
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);

}

void buttPress(void)
{
  display.clear();

  uint8_t data[] = {};
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(0);
  data[2] = display.encodeDigit(0);
  data[3] = display.encodeDigit(0);
  display.setSegments(data);

  Serial.println("Started the butt press");
  Timestamp_Button_Pressed = millis();

  while (digitalRead(tareButt) == LOW) {
    Serial.println("waiting for butt release");
  }

  if ((digitalRead(tareButt) == HIGH))
  {

    //Check if the button was pressed for a full 3 seconds then run calibration
    if (millis() - Timestamp_Button_Pressed > 3000)
    {
      Serial.println("Button was loooooooooooong pressed");
      Serial.println(millis() - Timestamp_Button_Pressed);
      scaleCalibration();
    }
    //If button was pressed for lesser than just run the tare function
    else
    {
      display.clear();
      Serial.println("Button was short pressed");
      scale.tare();

    }

  }

}


void setup()
{
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //making the button to high position
  pinMode(tareButt, INPUT_PULLUP);
  //reading the calibration data from EEPROM and taring the scale
  scale.set_scale(-EEPROM.read(address));
  scale.tare();
  Serial.print("CalibValue Read: ");
  Serial.println(EEPROM.read(address));


  display.setBrightness(5);
}

void loop()
{
  String weightOnScale = String(scale.get_units(5), 1);
  Serial.println(weightOnScale);
  int weightOnDisplay = weightOnScale.toInt();
  Serial.println(weightOnDisplay);
  display.showNumberDecEx(weightOnDisplay, false);
  if (digitalRead(tareButt) == LOW)
  {
    // If we detect LOW signal, button is pressed
    buttPress(); //turn this into an interrupt switch
  }
}

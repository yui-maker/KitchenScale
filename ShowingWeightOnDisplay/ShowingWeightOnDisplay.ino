//https://randomnerdtutorials.com/arduino-load-cell-hx711/
//This code can show grams and milligrams on display. No Kilograms

#include <Arduino.h>
#include "HX711.h"
#include <EEPROM.h>
#include <TM1637Display.h>



// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
float knownWeight = 189; //use a known weight to calibrate, I am using my phone.

// defining the button
#define tareButt 4
unsigned long lastButtonPress = 0;
unsigned long Timestamp_Button_Pressed;
int address = 0; //EEPROM memory address for storing the calibration value

HX711 scale;

#define CLK 15
#define DIO 14
TM1637Display display(CLK, DIO);

void scaleCalibration(void) {
  if (scale.is_ready()) {
    scale.set_scale();
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
    long calibVal = reading / knownWeight;
    Serial.print("CalibValue: ");
    Serial.println(calibVal);
    EEPROM.update(address, abs(calibVal));
    scale.set_scale(reading / knownWeight);
    Serial.println("Remove your weight");
    delay(5000);
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

  // Put in a slight delay to help debounce the reading
  delay(1);

}

void floatToString(float number, char* str) {
  dtostrf(number, 0, 4, str);
}

void OnDisplay(float weightOnDisplay) {
  if (weightOnDisplay < 0.5 && scale.get_units() > -0.5)
  {
    display.showNumberDec(0);
  }
  else {
    float number = weightOnDisplay;
    int integerPart = int(number);
    float afterDec = number - integerPart;
    String strIntPart = String(integerPart);
    int integerLength = strIntPart.length();



    if (afterDec > 0) {
      char numberString[5];
      floatToString(afterDec, numberString);

      int afterDecLength = String(numberString).length();

      String totalDisplay = strIntPart + numberString[2];
      int toShow = totalDisplay.toInt();
      Serial.println(integerLength);
      Serial.println(totalDisplay.toInt());
      display.showNumberDecEx(toShow, (0x80 >> 2), false);
    }

    else {
      display.showNumberDec(integerPart, false);
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
  String weightOnScale = String(scale.get_units(10), 1);
  float weightOnDisplay = weightOnScale.toFloat();
  OnDisplay(weightOnDisplay);
  if (digitalRead(tareButt) == LOW)
  {
    // If we detect LOW signal, button is pressed
    buttPress(); //turn this into an interrupt switch
  }
}

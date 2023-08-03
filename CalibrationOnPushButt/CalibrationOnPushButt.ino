//https://randomnerdtutorials.com/arduino-load-cell-hx711/
//This code can calibrate on long push and tare on short

#include <Arduino.h>
#include "HX711.h"
#include <EEPROM.h>

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

  // If we detect LOW signal, button is pressed
  if (digitalRead(tareButt) == LOW)
  {
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
        Serial.println("Button was short pressed");
        scale.tare();

      }

    }

    // Put in a slight delay to help debounce the reading
    delay(1);
  }
}


void setup()
{
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //

  pinMode(tareButt, INPUT_PULLUP);

  scale.set_scale(-EEPROM.read(address));
  scale.tare();
  Serial.print("CalibValue Read: ");
  Serial.println(EEPROM.read(address));
}

void loop() {
  //put your main code here, to run repeatedly:
  Serial.print("one reading:\t");
  Serial.println(scale.get_units(), 1);
  //  Serial.print("\t| average:\t");
  //  Serial.println(scale.get_units(10), 5);
  buttPress();
}

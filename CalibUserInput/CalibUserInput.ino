//https://randomnerdtutorials.com/arduino-load-cell-hx711/
//The calibration code from this website

#include "HX711.h"
#include <TM1637Display.h>  //Including the display library

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;


// display wiring
#define CLK 15
#define DIO 14
TM1637Display display(CLK, DIO);

HX711 scale;

int knownWeight = 189; //use a known weight to calibrate, I am using my phone.
int oldReading;
unsigned long Timestamp_Button_Pressed;
int timesPressed = 0;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

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
    display.setBrightness(5);
    display.setSegments(bottomPatterns);
    
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    oldReading = scale.get_units(5);
    while(scale.get_units()-oldReading <=10){
        //do nothing as long as user has not put anything on it
      }
    display.showNumberDec(0, true);  // Expect: 0000
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
    if (millis() - Timestamp_Button_Pressed > 2000)
    {
      Serial.println("Button was loooooooooooong pressed");
      Serial.println(millis() - Timestamp_Button_Pressed);
      //select the number input
    }
    //If button was pressed for lesser than just run the tare function
    else
    {
      display.clear();
      Serial.println("Button was short pressed");
      timesPressed++;
      display.showNumberDec(i, true);  // Expect: 0000
    }

  }
  }
    
    delay(5000);
    //Let the user input their known weight and wait till they have input it.
    //Save the user input known weight in EEPROM
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
    long calibVal = reading / knownWeight;
    Serial.print("CalibValue: ");
    Serial.println(calibVal);
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

//calibration factor will be the (reading)/(known weight)

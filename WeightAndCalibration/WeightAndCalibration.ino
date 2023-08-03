//https://randomnerdtutorials.com/arduino-load-cell-hx711/
//Combining weight and calibration codes from this website

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
float knownWeight = 189; //use a known weight to calibrate, I am using my phone. 

HX711 scale;

void scaleCalibration(void){
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
    scale.set_scale(reading/knownWeight);
    Serial.println("Remove your weight");
    delay(5000);
    scale.tare(); 
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
    
} 



void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);     
  scaleCalibration();
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  delay(1000);
}

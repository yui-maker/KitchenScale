#include <Arduino.h>
#include <TM1637Display.h>
//The sketch begins by including the library and defining the TM1637 display’s connection pins.

//The next step is to create an object of TM1637Display class. 
//The TM1637Display constructor accepts two inputs: the CLK pin and the DIO pin.
// Module connection pins (Digital Pins)
#define CLK 11
#define DIO 12

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000


TM1637Display display(CLK, DIO);
//String numToDisplay = "0x";
//int numReceived= String(5, HEX);
String hexString = "0x1";  // Example hexadecimal string
long hexValue = strtol(hexString.c_str(), NULL, 16);
 
int lenToDisplya;
void setup() {
  display.setBrightness(7); // Set the brightness (0-7)
  display.clear();          // Clear the display
}

void loop() {
  //numToDisplay = numToDisplay + numReceived;
  lenToDisplya = hexString.length()-2;
  display.showNumberHexEx(1237, 0, false,lenToDisplya);

  delay(1000);  // Delay for 1 second
}

/*
void setup()
{
  display.setBrightness(5);

  //setBrightness(brightness,on)

//This function is used to adjust the display’s brightness. 
//You can choose a brightness level ranging from 0 (lowest brightness) to 7 (highest brightness).

//The second argument is optional and can be used to turn on or off the display. 
//You can either pass true (display ON) or false (display OFF). 
//For example, display.setBrightness(5) sets the brightness to 5, whereas display.setBrightness(5, false) turns off the display.

}

void loop()
{

      //showNumberDec(number,leading_zeros,length,position)
      //showNumberDecEx(number,dots,leading_zeros,length,position)
      display.showNumberDecEx(1000, 0b01000000, true, 4, 0);
      delay(TEST_DELAY);


}
*/

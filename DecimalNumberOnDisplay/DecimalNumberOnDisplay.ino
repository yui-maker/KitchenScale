//This code can show decimal numbers with only one digit after decimal point on TM1637Display

#include <TM1637Display.h>
#define CLK 15
#define DIO 14
TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  display.setBrightness(5);
}

void loop() {

  String userInput = Serial.readStringUntil('\n');
  float number = userInput.toFloat();
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

  else{
    display.showNumberDec(integerPart,false);
  }

  delay(500);  // Delay for readability
  /*
  String userInput = Serial.readStringUntil('\n');
  int number = userInput.toInt();
  display.showNumberDecEx(number, (0x80 >> 3), false);
    */
}

void floatToString(float number, char* str) {
  dtostrf(number, 0, 4, str);
}

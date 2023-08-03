//This code takes 4 values of input via one push button from user

#include <TM1637Display.h>  //Including the display library

// display wiring
#define CLK 4
#define DIO 3
TM1637Display display(CLK, DIO);

#define tareButt 2

int knownWeight = 189; //use a known weight to calibrate, I am using my phone.
int oldReading;
unsigned long Timestamp_Button_Pressed;
int timesPressed = 0;
int longTimePressed = 0;
String userInput = "";
int numLoop = 1;

void setup() {
  Serial.begin(57600);
  display.setBrightness(3);
  //making the button to high position
  pinMode(tareButt, INPUT_PULLUP);
}

void loop() {
  //  byte bottomPatterns[] = {
  //    B00000001,  // Bottom cell of first row
  //    B00000001,//B00000010,  // Bottom cell of second row
  //    B00000001,//B00000100,  // Bottom cell of third row
  //    B00000001,//B00001000   // Bottom cell of fourth row
  //  };
  //  display.setBrightness(5);
  //  display.setSegments(bottomPatterns);
  //  delay(250);
  //  Serial.println("Tare done...");
  //  Serial.print("Place a known weight on the scale...");
  //  display.clear();
  //  display.showNumberDec(0, true);  // Expect: 0000
  if (digitalRead(tareButt) == LOW)
  {
    Serial.println("Started the butt press");
    Timestamp_Button_Pressed = millis();

    while (digitalRead(tareButt) == LOW) {
      Serial.println("waiting for butt release");
    }

    if ((digitalRead(tareButt) == HIGH))
    {

      //Check if the button was pressed for a full 2 seconds then run calibration
      if (millis() - Timestamp_Button_Pressed > 1000 && longTimePressed < 4)
      {
        longTimePressed++;
        Serial.println("Button was loooooooooooong pressed");
        //select the number input
        userInput += String(timesPressed);
        String userInputDis = userInput + "0";
        int userInputDisN = userInputDis.toInt();
        timesPressed = userInputDisN;
        display.showNumberDec(userInputDisN, false);
        Serial.println("timespressed: " + String(userInput));
        Serial.println("userInputDis: " + String(userInputDis));
        Serial.println("userInputDis: " + String(userInputDis));
        if (longTimePressed == 4)
        {
          knownWeight = userInput.toInt();
          Serial.println("You inputed: " + String(knownWeight));
          //get out of this loop
        }
        numLoop = 0;
      }
      //If button was short pressed then just increase the number by 1
      else
      {
        String timesPressedStr = String(timesPressed);
        numLoop++;
        if (numLoop == 10) {
          numLoop = 0;
        }
        if (timesPressedStr.length() == 1) {
          display.clear();
          display.showNumberDec(numLoop, false);
          timesPressed = numLoop;
        }
        Serial.println("Button was short pressed");
        String timeReceived = String(timesPressed);
        String timeReceivedStr = timeReceived.substring(0, timeReceived.length() - 1);
        Serial.println("Timer received " + String(timeReceived));
        Serial.println("Timer received minus last dig " + String(timeReceivedStr));
        String disStr = timeReceivedStr + String(numLoop);
        int disInt = disStr.toInt();
        Serial.println("Timer to be shown in string " + String(disStr));
        Serial.println("Timer to be shown in int " + String(disInt));
        display.clear();
        display.showNumberDec(disInt, false);
        timesPressed = disInt;
        userInput = "";

      }

    }
  }
}

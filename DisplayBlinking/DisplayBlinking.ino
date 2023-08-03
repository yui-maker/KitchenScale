#include <TM1637Display.h>

#define CLK_PIN 15   // CLK pin of TM1637 connected to Arduino pin 2
#define DIO_PIN 14   // DIO pin of TM1637 connected to Arduino pin 3

TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  display.setBrightness(7); // Set the brightness (0-7)
  display.clear();          // Clear the display
}

void loop() {
  display.setBrightness(5);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  display.setSegments(data);
  delay(500);  
  display.clear();
  display.setBrightness(0); // Turn off the display
  delay(500);               // Delay for display off time
}
/*#include <TM1637Display.h>

#define CLK_PIN 15   // CLK pin of TM1637 connected to Arduino pin 2
#define DIO_PIN 14     // DIO pin of TM1637 connected to Arduino pin 3

TM1637Display display(CLK_PIN, DIO_PIN);

// Custom patterns for bottom cells of each row
byte bottomPatterns[] = {
  B00000001,  // Bottom cell of first row
  B00000001,//B00000010,  // Bottom cell of second row
  B00000001,//B00000100,  // Bottom cell of third row
  B00000001,//B00001000   // Bottom cell of fourth row
};

void setup() {
  display.setBrightness(7); // Set the brightness (0-7)
  display.clear();          // Clear the display
}

void loop() {
  for (int i = 0; i < sizeof(bottomPatterns); i++) {
    display.setSegments(&bottomPatterns[i], 1, i); // Set bottom cell pattern for specific row
    delay(1000);  // Delay between row patterns
  }
}
*/

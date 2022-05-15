/*
 * STUDENT: (Type your name here)
 */

/*
 * PollingLab (c) 2021-22 Christopher A. Bohn
 */

#include "cowpi.h"

#define BUTTON_NO_REPEAT_TIME 500u
#define DEBOUNCE_TIME 20u
#define ILLUMINATION_TIME 500u
#define NUMBER_OF_DIGITS 8

uint8_t getKeyPressed();
void displayData(uint8_t address, uint8_t value);
void testSimpleIO();

cowpi_ioPortRegisters *ioPorts;   // an array of I/O ports
cowpi_spiRegisters *spi;          // a pointer to the single set of SPI registers
unsigned long lastLeftButtonPress = 0;
unsigned long lastRightButtonPress = 0;
unsigned long lastLeftSwitchSlide = 0;
unsigned long lastRightSwitchSlide = 0;
unsigned long lastKeypadPress = 0;


// Layout of Matrix Keypad
//        1 2 3 A
//        4 5 6 B
//        7 8 9 C
//        * 0 # D
// This array holds the values we want each keypad button to correspond to
const uint8_t keys[4][4] = {
  {},
  {},
  {},
  {}
};

// Seven Segment Display mapping between segments and bits
// Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
//  DP   A    B    C    D    E    F    G
// This array holds the bit patterns to display each hexadecimal numeral
const uint8_t sevenSegments[16] = {

};

void setup() {
  Serial.begin(9600);
  cowpi_setup(SPI | MAX7219);
  ioPorts = (cowpi_ioPortRegisters *) (cowpi_IObase+0x03);
  // spi = ...
}

void loop() {
  testSimpleIO();
  // if ((...) && (millis() - lastKeypadPress > BUTTON_NO_REPEAT_TIME)) {
  //   uint8_t keypress = getKeyPressed();
  //   if (keypress < 0x10) {
  //     Serial.print("Key pressed: ");
  //     Serial.println(keypress, HEX);
  //     // displayData(1, sevenSegments[keypress]);
  //   } else {
  //     Serial.println("Error reading keypad.");
  //   }
  // }
}

uint8_t getKeyPressed() {
  uint8_t keyPressed = 0xFF;
  unsigned long now = millis();
  if (now - lastKeypadPress > DEBOUNCE_TIME) {
    lastKeypadPress = now;
    // ...
  }
  return keyPressed;
}

void displayData(uint8_t address, uint8_t value) {
  // address is MAX7219's register address (1-8 for digits; otherwise see MAX7219 datasheet Table 2)
  // value is the bit pattern to place in the register
  cowpi_spiEnable;
  // ...
  cowpi_spiDisable;
}

uint8_t leftSwitchLastPosition = 0;
uint8_t rightSwitchLastPosition = 0;

void testSimpleIO() {
  uint8_t printedThisTime = 0;
  uint8_t leftSwitchCurrentPosition = ioPorts[A0_A5].input& 0x10;
  uint8_t rightSwitchCurrentPosition = ioPorts[A0_A5].input& 0X20;
  uint8_t leftButtonCurrentPosition = digitalRead(8);
  uint8_t rightButtonCurrentPosition = digitalRead(9);
  digitalWrite(12, leftSwitchCurrentPosition && rightSwitchCurrentPosition);
  unsigned long now = millis();
  if ((leftSwitchCurrentPosition != leftSwitchLastPosition) && (now - lastLeftSwitchSlide > DEBOUNCE_TIME)) {
    Serial.print(now);
    Serial.print("\tLeft switch changed: ");
    Serial.print(leftSwitchCurrentPosition);
    leftSwitchLastPosition = leftSwitchCurrentPosition;
    printedThisTime = 1;
    lastLeftSwitchSlide = now;
  }
  if ((rightSwitchCurrentPosition != rightSwitchLastPosition) && (now - lastRightSwitchSlide > DEBOUNCE_TIME)) {
    if (!printedThisTime) {
      Serial.print(now);
    }
    Serial.print("\tRight switch changed: ");
    Serial.print(rightSwitchCurrentPosition);
    rightSwitchLastPosition = rightSwitchCurrentPosition;
    printedThisTime = 1;
    lastRightSwitchSlide = now;
  }
  if (!leftButtonCurrentPosition && (now - lastLeftButtonPress > BUTTON_NO_REPEAT_TIME)) {
    if (!printedThisTime) {
      Serial.print(now);
    }
    Serial.print("\tLeft button pressed");
    printedThisTime = 1;
    lastLeftButtonPress = now;
  }
  if (!rightButtonCurrentPosition && (now - lastRightButtonPress > BUTTON_NO_REPEAT_TIME)) {
    if (!printedThisTime) {
      Serial.print(now);
    }
    Serial.print("\tRight button pressed");
    printedThisTime = 1;
    lastRightButtonPress = now;
  }
  if(printedThisTime) {
    Serial.println();
  }
}

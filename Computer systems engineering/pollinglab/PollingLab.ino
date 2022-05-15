/*
   STUDENT: Harry Endrulat
*/

/*
   PollingLab (c) 2021-22 Christopher A. Bohn
*/

#include "cowpi.h"
#include "limits.h"

#define BUTTON_NO_REPEAT_TIME 500u
#define DEBOUNCE_TIME 20u
#define ILLUMINATION_TIME 500u
#define NUMBER_OF_DIGITS 8

uint8_t getKeyPressed();
void displayData(uint8_t address, uint8_t value);
void testSimpleIO();
unsigned long loopNow = 0;

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
  {1, 2, 3, 0xA},
  {4, 5, 6, 0xB},
  {7, 8, 9, 0xC},
  {0xF, 0, 0xE, 0xD}
};

// Seven Segment Display mapping between segments and bits
// Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
//  DP   A    B    C    D    E    F    G
// This array holds the bit patterns to display each hexadecimal numeral
const uint8_t sevenSegments[16] = {
  0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01110011, 0b01110111, 0b00011111, 0b00001101, 0b00111101, 0b01001111, 0b01000111
};

void setup() {
  Serial.begin(9600);
  cowpi_setup(SPI | MAX7219);
  ioPorts = (cowpi_ioPortRegisters *) (cowpi_IObase + 0x03);
  spi = (cowpi_spiRegisters *) (cowpi_IObase + 0x2C);
}

void demo_mode(uint8_t keypress, uint8_t numberbase, uint8_t clearflag) {
  
  if (!numberbase) {
      Serial.print("Key pressed: ");
      Serial.println(keypress, HEX);
    if (keypress > 9) {
      keypress = 0xff;
//      displayData(1, sevenSegments[keypress]);
    }
    else {
      displayData(1, sevenSegments[keypress]);
      loopNow = millis();
    }
  }
  else {
      Serial.print("Key pressed: ");
      Serial.println(keypress, HEX);
    displayData(1, sevenSegments[keypress]);
    loopNow = millis();
  }
}

void build_mode(uint8_t keypress, uint8_t numberbase, uint8_t clearflag) {

}

//the loop function can take care of which mode we're in, whether a key on the kepad is pressed and whether a button is pressed
//if the right button is pressed set a clear flag to 1 and clear the clock output
//if the left button is pressed change the negation flag to 1
//TODO: create build_mode and demo_mode functions
//pass keypressed
//demo mode: paramereters: key pressed, number base, clear flag, now
//again loop takes care of the mode depending on
//left/right switch to the right means a 1 and to the left mean a 0
//left switch is pin A4
//right switch is pin A5
//left button is pin D8
//right button is pin D9
//now-lastleftbuttonpress>button no repeat time
//read input then negate if(now-leftlastbutton)

void loop() {
  uint8_t mode_flag = ioPorts[A0_A5].input & 0x10;//left switch, left is demo right is build
  uint8_t numberbase = ioPorts[A0_A5].input & 0x20;//right switch, left is decimal, right is hex
  uint8_t negation_flag = ioPorts[D8_D13].input & 0x01; //when not pressed the buttons are 1
  uint8_t clearflag = ioPorts[D8_D13].input & 0x02;
//  if (((ioPorts[A0_A5].input & 0x0f) < 0x0f) && (millis() - lastKeypadPress > BUTTON_NO_REPEAT_TIME)) {
//    uint8_t keypress = getKeyPressed();
//    if (keypress < 0x10) {
//      Serial.print("Key pressed: ");
//      Serial.println(keypress, HEX);
//    } else {
//      Serial.println("Error reading keypad.");
//    }
//  }//a useful function in the past
      if (millis() < loopNow + ILLUMINATION_TIME) {
        ioPorts[D8_D13].output |= 0x10;
      }
      else{
        ioPorts[D8_D13].output &= 0xef;
      }
  if (!mode_flag && ((ioPorts[A0_A5].input & 0x0f) < 0x0f) && (millis() - lastKeypadPress > BUTTON_NO_REPEAT_TIME)) {
    uint8_t keypress = getKeyPressed();
    demo_mode(keypress, numberbase, clearflag);
  }
  else if (mode_flag && ((ioPorts[A0_A5].input & 0x0f) < 0x0f) && (millis() - lastKeypadPress > BUTTON_NO_REPEAT_TIME)) {
    uint8_t keypress = getKeyPressed();
    build_mode(keypress, numberbase, clearflag);
  }
  
  if (!clearflag) {    
    displayData(1, 0x00);
  }
}

uint8_t getKeyPressed() {
  uint8_t keyPressed = 0xFF;
  unsigned long now = millis();
  if (now - lastKeypadPress > DEBOUNCE_TIME) {
    lastKeypadPress = now;
    int iTrack = 0;
    int jTrack = 0;
    int i = 0b00010000;
    int j = 0b00000001;//i found this easier to visualize what was happening with the bit shifts and bit operators
    while (i < 129) {
      ioPorts[D0_D7].output |= 0xf0;
      ioPorts[D0_D7].output ^= i;
      i = i << 1;
      if ((ioPorts[A0_A5].input & 0x0f) < 0x0f) {
        while (j < 9) {
          if ((ioPorts[A0_A5].input & j) == 0) {
            keyPressed = keys[iTrack][jTrack];
          }
          jTrack++;
          j = j << 1;
        }
      }
      iTrack++;
    }
    ioPorts[D0_D7].output &= 0x0f;
  }
  return keyPressed;
}

void displayData(uint8_t address, uint8_t value) {
  // address is MAX7219's register address (1-8 for digits; otherwise see MAX7219 datasheet Table 2)
  // value is the bit pattern to place in the register
  cowpi_spiEnable;
  // ...
  ioPorts[D8_D13].output &= 0xfb;
  spi -> data = address;
  while ((spi-> status) == 0) {
    //wait for SPI to send the data
  }
  spi-> data = value;
  while ((spi-> status) == 0) {
    //wait for SPI to send the data
  }
  ioPorts[D8_D13].output |= 0x04;
  cowpi_spiDisable;
}

uint8_t leftSwitchLastPosition = 0;
uint8_t rightSwitchLastPosition = 0;

void testSimpleIO() {
  uint8_t printedThisTime = 0;
  uint8_t leftSwitchCurrentPosition = ioPorts[A0_A5].input & 0x10;
  uint8_t rightSwitchCurrentPosition = ioPorts[A0_A5].input & 0x20;
  uint8_t leftButtonCurrentPosition = ioPorts[D8_D13].input & 0x01;
  uint8_t rightButtonCurrentPosition = ioPorts[D8_D13].input & 0x02;
  uint8_t left_right_switch = (leftSwitchCurrentPosition && rightSwitchCurrentPosition);
//  if (left_right_switch) {
//    ioPorts[D8_D13].output |= 0x10;
//  }
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
  if (printedThisTime) {
    Serial.println();
  }
}

/*
 * STUDENT: Harry Endrulat
 */

/*
 * InterruptLab (c) 2021-22 Christopher A. Bohn
 */

#include "cowpi.h"

#define DEBOUNCE_TIME 20u
#define SINGLE_CLICK_TIME 150u
#define DOUBLE_CLICK_TIME 500u
#define NUMBER_OF_DIGITS 8

void setupTimer();
void handleButtonAction();
void handleKeypress();
void displayData(uint8_t address, uint8_t value);


/* Memory-mapped I/O */
cowpi_ioPortRegisters *ioPorts;     // an array of I/O ports
cowpi_spiRegisters *spi;            // a pointer to the single set of SPI registers
cowpi_timerRegisters16bit *timer1;  // a pointer to one 16-bit timer
cowpi_timerRegisters8bit *timer2;   // a pointer to one 8-bit timer

/* Variables for software debouncing */
volatile unsigned long lastLeftButtonAction = 0;
volatile unsigned long lastRightButtonAction = 0;

volatile unsigned long oldLeftButtonPosition =1;
volatile unsigned long oldRightButtonPosition =1;

volatile unsigned long lastLeftPress =0;// from button up position to down
volatile unsigned long lastRightPress =0;

volatile unsigned long lastLeftClick =0;

volatile unsigned long lastLeftSwitchSlide = 0;
volatile unsigned long lastRightSwitchSlide = 0;

volatile unsigned long leftSwitchPosition = 0;//local variables
volatile unsigned long rightSwitchPosition = 0;

volatile unsigned long lastKeypadPress = 0;
volatile unsigned long timerCounter=0;

volatile boolean conversionFlag=false;
volatile boolean doubleClick=false;
volatile boolean LEDFlag=false;

volatile uint8_t displayArray[8] = {0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
volatile int MSD=0;//depricated
volatile unsigned long total=0;


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
const uint8_t sevenSegments[17] = {
  0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01110011, 0b01110111, 0b00011111, 0b00001101, 0b00111101, 0b01001111, 0b01000111, 0b00000000
};

void setup() {
  Serial.begin(9600);
  cowpi_setup(SPI | MAX7219);
  attachInterrupt( digitalPinToInterrupt(2) , handleButtonAction , CHANGE );
  attachInterrupt( digitalPinToInterrupt(3) , handleKeypress , CHANGE );
  ioPorts = (cowpi_ioPortRegisters *) (cowpi_IObase + 0x03);
  spi = (cowpi_spiRegisters *) (cowpi_IObase + 0x2C);
  setupTimer();
  displayArrayFunction();
}

void loop() {
  // You can have code here while you're working on the assignment,
  // but be sure there isn't any code here by the time that you're finished.
  ;
}

void setupTimer() {
   timer1->control = 0x0B00;
   timer1->compareA = 250-1;
   TIMSK1 = 0x02;
}

ISR(TIMER1_COMPA_vect){
  timerCounter++;
  if((timerCounter%250)==0){//about half a second
    Serial.println(timerCounter);
  }
}

//two functions start conversion and stop conversion
//

void clearFunction(){
  for(int i=1; i<9; i++){
    displayData(i, 0x00);
    displayArray[i-1]=0x11;
  }
  displayArray[0]=0;
  displayArrayFunction();
}

void handleButtonAction() {//CHECK THE TODO'S
  unsigned long now=millis();
  unsigned long newLeftButtonPosition =0;
  unsigned long newRightButtonPosition =0;
  
  if((now-lastLeftButtonAction)>DEBOUNCE_TIME){
    newLeftButtonPosition=digitalRead(8);
  }
  else{
    newLeftButtonPosition = oldLeftButtonPosition;
  }
  if((now-lastRightButtonAction)>DEBOUNCE_TIME){
    newRightButtonPosition=digitalRead(9);
  }
  else{
    newRightButtonPosition = oldRightButtonPosition;
  }
  if((oldLeftButtonPosition==newLeftButtonPosition) && (oldRightButtonPosition==newRightButtonPosition)){
    exit;
  }
  else{
    if(oldRightButtonPosition && !newRightButtonPosition){//case 1 right button pressed
      lastRightPress=now;
      Serial.println("Right button pressed");
    }
    else if(!oldRightButtonPosition && newRightButtonPosition){//case 2 right button released
      
      if((lastRightPress+SINGLE_CLICK_TIME) < now){
        //clear the display
        clearFunction();//for some reason the way i am clearing interrupts my function
        Serial.println("Right button released: the display has been cleared");
        total =0;
        
      }
      //maybe have an else case for a too fast button press
    }
    else if(oldLeftButtonPosition && !newLeftButtonPosition){//case 3 left button pressed
      lastLeftPress = now;
      Serial.println("Left button pressed");
      if((lastLeftClick+DOUBLE_CLICK_TIME) > now){
        //set double click to true?
        //TODO: Call the convert base function here
        //digitl write(12, high)
        //reset timer1->counter=0;
        //call start conversion -- inside start converion after 7.5 or 20 seconds have passed call stop conversion
        
        Serial.println("Left button correctly double pressed");
      }
    }
    else if(!oldLeftButtonPosition && newLeftButtonPosition){//case 4 left button released
      if (doubleClick){
        doubleClick=false;
      }
      else if((lastLeftPress+SINGLE_CLICK_TIME) < now){
        //negate TODO: call a negate helper function or negate the display
        lastLeftClick=0;
        Serial.print("Left button released: ");
        Serial.println("Single click, display negated");
      }
      else{
        lastLeftClick=now;//should i call millis() everytime or set variables to now?
      }
    }
  }
  oldLeftButtonPosition = newLeftButtonPosition;
  oldRightButtonPosition = newRightButtonPosition;
}

void handleKeypress() {//TODO: make sure this is correct
  //digital read switches?, start timer left switch to the left :20 seconds, to the right:7.5 seconds
  //right switch to the left:decimal, to the right:Hex
  //leftSwitch = timer; rightSwitch = number base;
  //switch to the right is a 1, to the left is 0
  uint8_t timerFlag = ioPorts[A0_A5].input & 0x10;
  uint8_t numberBase = ioPorts[A0_A5].input & 0x20;//when =0 decimal mode; when 1 Hex mode
  
  uint8_t keyPressed = 0xFF;//maybe get rid of this and just use keys[][]
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
            buildNumber(numberBase, keyPressed);            
            Serial.print("A key on the keypad has been pressed and added to the display: ");
            Serial.print("Key pressed: ");
            Serial.println(keyPressed, HEX);
          }
          jTrack++;
          j = j << 1;
        }
      }
      iTrack++;
    }
    ioPorts[D0_D7].output &= 0x0f;
  }
}

void buildNumber(uint8_t numberBase, uint8_t keyPressed){
  if(numberBase){//Hex mode
    buildDisplayArray(numberBase, keyPressed);
  }
  else{//decimal mode
    if(keyPressed<10){//0-9
      buildDisplayArray(numberBase, keyPressed);
    }
    else{
      //invalid input. in decimal mode but a hex was entered.
    }
  }
}

void buildDisplayArray(uint8_t numberBase, uint8_t keyPressed){
  for(int k=7; k>=0; k--){//this shifts the display / display array/display
    if(k>0){
      displayArray[k] = displayArray[k-1];//builds the display array
    }
    else{
      if(total==0){
        displayArray[1]=0x11;
      }
      displayArray[k] = keyPressed;
      if(numberBase){
        total = total<<4;
        total = total|keyPressed;
      }
      else{
        total=((total*10)+keyPressed);
      }
      Serial.print("The current total is: ");
      Serial.println(total);
      
    } 
  }
  if((total>0x7fffffff) && numberBase){//this is only for hex mode, one is needed for decimal too as FFFFFFFF != 99999999
    //display "too big"
    displayTooBig();
  }
  else if((total>99999999) && !numberBase){
    displayTooBig();
  }
  else{
    displayArrayFunction();
  }
}

void displayTooBig(){//G               I          b       ""         o          o           t        ""
  uint8_t tooBig[8]={0b01011110, 0b00000110, 0b00011111, 0x00, 0b00011101, 0b00011101, 0b00001111, 0x00};
  int displayPort=8;//this is simply i+1
  for (int i=7; i>=0; i--){//maybe start at 0
    displayData(displayPort, tooBig[i]);
    displayPort--;
  }
}

void displayArrayFunction(){
  int displayPort=8;
  for (int i=7; i>=0; i--){//maybe start at 0
    displayData(displayPort, sevenSegments[displayArray[i]]);
    displayPort--;
  }
}

void displayData(uint8_t address, uint8_t value) {
  // address is MAX7219's register address (1-8 for digits; otherwise see MAX7219 datasheet Table 2)
  // value is the bit pattern to place in the register
  cowpi_spiEnable;
  
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

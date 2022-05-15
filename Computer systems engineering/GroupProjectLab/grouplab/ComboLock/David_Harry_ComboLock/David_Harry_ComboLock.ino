/*
   STUDENTS: David Ryckman && Harry Endrulat
*/

/*
   CombinationLock GroupLab (c) 2022 Christopher A. Bohn
*/


#include <EEPROM.h>
#include "cowpi.h"

#define DEBOUNCE_TIME 20u
#define DOUBLE_CLICK_TIME 500u
#define DASH 17
#define CURSOR 18
#define FLAG_VALUE 16

cowpi_timerRegisters16bit *timer1; // a pointer to one 16-bit timer
short selectiveDecrement(short x);
short assertCorrect();

short cursored = 7;
bool should_cursor = true;
short selected = 7;
short fail_attempts = 0;

char mode = 'L';
bool flash_LED = false;
bool lock_out = false;

const uint8_t sevenSegments[30] = {
  0b01111110, // 0
  0b00110000, // 1
  0b01101101, // 2
  0b01111001, // 3
  0b00110011, // 4
  0b01011011, // 5
  0b01011111, // 6
  0b01110000, // 7
  0b01111111, // 8
  0b01111011, // 9
  0b01110111, // A
  0b00011111, // B
  0b00001101, // C
  0b00111101, // d
  0b01001111, // E
  0b01000111, // F
  0b00000000, // FLAG VALUE
  0b00000001, // - *DASH*
  0b10000000, // . *cursor*
  0b00001111, // t -19
  0b00000101, // r -20
  0b00111011, // y -21
  0b00011101, // o -22
  0b01100111, // P -23
  0b01110110, // N -24
  0b00001110, // L -25
  0b10100000, // ! -26
  0b00110111, // H -27
  0b01011110, // G -28
  0b01011011  // S -29
};

uint8_t closed[8] = {
  sevenSegments[12],
  sevenSegments[25],
  sevenSegments[22],
  sevenSegments[29],
  sevenSegments[14],
  sevenSegments[13],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t noChange[8] = {
  sevenSegments[24],
  sevenSegments[22],
  sevenSegments[12],
  sevenSegments[27],
  sevenSegments[10],
  sevenSegments[24],
  sevenSegments[28],
  sevenSegments[14]
};

uint8_t changed[8] = {
  sevenSegments[12],
  sevenSegments[27],
  sevenSegments[10],
  sevenSegments[24],
  sevenSegments[28],
  sevenSegments[14],
  sevenSegments[13],
  sevenSegments[FLAG_VALUE]
};

uint8_t reEnter[8] = {
  sevenSegments[20],
  sevenSegments[14],
  sevenSegments[DASH],
  sevenSegments[14],
  sevenSegments[24],
  sevenSegments[19],
  sevenSegments[14],
  sevenSegments[20]
};

uint8_t enter[8] = {
  sevenSegments[14],
  sevenSegments[24],
  sevenSegments[19],
  sevenSegments[14],
  sevenSegments[20],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t alert[8] = {
  sevenSegments[10],
  sevenSegments[25],
  sevenSegments[14],
  sevenSegments[20],
  sevenSegments[19],
  sevenSegments[26],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t labOpen[8] = {
  sevenSegments[25],
  sevenSegments[10],
  sevenSegments[11],
  sevenSegments[FLAG_VALUE],
  sevenSegments[22],
  sevenSegments[23],
  sevenSegments[14],
  sevenSegments[24]
};

uint8_t badTry1[8] = {
  sevenSegments[11],
  sevenSegments[10],
  sevenSegments[13],
  sevenSegments[FLAG_VALUE],
  sevenSegments[19],
  sevenSegments[20],
  sevenSegments[21],
  sevenSegments[1]
};

uint8_t badTry2[8] = {
  sevenSegments[11],
  sevenSegments[10],
  sevenSegments[13],
  sevenSegments[FLAG_VALUE],
  sevenSegments[19],
  sevenSegments[20],
  sevenSegments[21],
  sevenSegments[2]
};

uint8_t badTry3[8] = {
  sevenSegments[11],
  sevenSegments[10],
  sevenSegments[13],
  sevenSegments[FLAG_VALUE],
  sevenSegments[19],
  sevenSegments[20],
  sevenSegments[21],
  sevenSegments[3]
};

uint8_t display_array[8] = {
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t num_array[8] = {
  1,
  2,
  sevenSegments[DASH],
  3,
  4,
  sevenSegments[DASH],
  5,
  6
};

uint8_t possible_array[8] = {
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};


uint8_t resetArray[8] = {
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t EEPROM_array[8] = {
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE],
  sevenSegments[DASH],
  sevenSegments[FLAG_VALUE],
  sevenSegments[FLAG_VALUE]
};

uint8_t error[8] = {
  sevenSegments[14],
  sevenSegments[20],
  sevenSegments[20],
  sevenSegments[22],
  sevenSegments[20],
  sevenSegments[CURSOR],
  sevenSegments[CURSOR],
  sevenSegments[CURSOR]
};


void read_from_EEPROM();
void persist_input();
bool attempt_change();

void setup() {
  Serial.begin(9600);
  cowpi_setup(SPI | MAX7219);

  setupTimer();
  attachInterrupt(digitalPinToInterrupt(2), handleButtonLocked, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), handleKeypress, CHANGE);
  show_display_array();
  read_from_EEPROM();
}
char preMode = 'N';
void loop() {
  if (mode != preMode) {
    preMode = mode;
    switch (mode) {
      case 'U'://UNLOCKED
        attachInterrupt(digitalPinToInterrupt(2), handleButtonUnlocked, CHANGE);
        Serial.println(mode);
        break;
      case 'L'://LOCKED
        attachInterrupt(digitalPinToInterrupt(2), handleButtonLocked, CHANGE);
        Serial.println(mode);
        break;
      case 'C'://CHANGING
        attachInterrupt(digitalPinToInterrupt(2), handleButtonChanging, CHANGE);
        Serial.println(mode);
        break;
      case 'O'://CONFIRMING
        attachInterrupt(digitalPinToInterrupt(2), handleButtonConfirming, CHANGE);
        Serial.println(mode);
        break;
    }
  }
}

volatile uint8_t *timer_interupt = cowpi_IObase + 0x4E;
void setupTimer()
{
  timer1 = (cowpi_timerRegisters16bit *)(cowpi_IObase + 0x60);
  // Time1
  // Control String
  //        W13 W12 C12 C11 C10 ? ? ? ? - - W11 W10
  //         0   1   1   0   0  0 0 0 0 0 0  0   0
  timer1->control = 0b110000000000;
  timer1->compareA = 15624; // STOP EVERY .25 SECONDS with Prescale of 256
  timer_interupt[1] = 0x02;
}

double seconds = 0;
bool cursorLightOn = true;
double alarm = 0;
double alert_alarm = 0;
bool LED_ON = false;
ISR(TIMER1_COMPA_vect)
{
  seconds += 0.25;
  if ((seconds - (int)seconds == .5) || (seconds - (int)seconds == .0))
  {
    //Half Second Pasted
    if ((!cursorLightOn) && (should_cursor)) {
      cursorLightOn = true;
      display_array[cursored] |= sevenSegments[CURSOR];
      display_array[cursored - 1] |= sevenSegments[CURSOR];
    } else {
      cursorLightOn = false;
      display_array[cursored] &= ~sevenSegments[CURSOR];
      display_array[cursored - 1] &= ~sevenSegments[CURSOR];
    }
    show_display_array();
  }

  if (flash_LED) {
    if (LED_ON) {
      LED_ON = false;
      digitalWrite(12, LOW);
    } else {
      LED_ON = true;
      digitalWrite(12, HIGH);
    }
  }

  if (seconds == alarm) {//what are these doing?
    emptyDisplay();
  }
  if (seconds == alert_alarm) {//what are these doing?
    Alarm();
  }
}

void setAlarm(double x) {
  timer1->counter = 0;
  seconds = 0;
  alarm = x;
}

void setAlertAlarm(double x) {
  timer1->counter = 0;
  seconds = 0;
  alert_alarm = x;
}

void Alarm() {
  special_display(alert);
  flash_LED = true;
}

void emptyDisplay() {
  for (int i = 0; i < 8; i++) {
    display_array[i] = resetArray[i];
    num_array[i] = resetArray[i];
  }
  should_cursor = true;
  cursored = 7;
  selected = 7;
}

bool LastRightState = 0;
bool LastLeftState = 0;
unsigned long firstRightButtonPress = 0;
int rightButtonCounter = 0;
unsigned long lastButtonPressed = 0;

void handleButtonLocked() {
  unsigned long now = millis();
  if ((now - lastButtonPressed > 100) && (!lock_out))//debouncing
  {
    lastButtonPressed = now;
    //Right Button Progress Cursor
    if (!digitalRead(9))
    {
      advanceCursor();
    }
    //Left Button Assert Correct
    if (!digitalRead(8))
    {
      short x = assertCorrect();
      switch (x) {
        case 1:
          //CORRECT OPEN
          mode = 'U';
          special_display(labOpen);
          break;
        case -1:
          //FAIL  Print ERROR
          special_display(error);
          setAlarm(1);
          break;
        case 0:
          //INCORRECT  INCREMENT COUNTER; reset
          fail_attempts++;
          switch (fail_attempts) {
            case 1:
              special_display(badTry1);
              setAlarm(1);
              break;
            case 2:
              special_display(badTry2);
              setAlarm(1);//
              break;
            default:
              special_display(badTry3);
              lock_out = true;
              setAlertAlarm(1);
              break;
          }
          break;
      }
    }

  }

}

void handleButtonUnlocked() {
  unsigned long now = millis();
  if ((now - lastButtonPressed > 150) && (!lock_out))//debouncing
  {
    lastButtonPressed  = now;
    //Left
    if (!digitalRead(8))
    {
      if (digitalRead(A4) && digitalRead(A5)) {
        //Change to CHANGING
        mode = 'C';
        special_display(enter);
        setAlarm(1);
      }
    }
    //Double Right
    if (!digitalRead(A4) && !digitalRead(A5) && (mode == 'U')) {
      if (!digitalRead(9)) { //right button pressed
        rightButtonCounter++;
        if (rightButtonCounter == 1) {
          firstRightButtonPress = now;
        }
        if (rightButtonCounter > 1 && (firstRightButtonPress + DOUBLE_CLICK_TIME) > now) {
          rightButtonCounter = 0;
          mode = 'L';
          special_display(closed);
          setAlarm(1);
        }
        if (rightButtonCounter > 1) {
          rightButtonCounter = 0;
        }
      }
    }
  }
}

void handleButtonChanging() {
  unsigned long now = millis();
  if ((now - lastButtonPressed > 150) && (!lock_out))//debouncing
  {
    lastButtonPressed  = now;
    // Right
    if (!digitalRead(9))
    {
      advanceCursor();
    }
    //Left
    if (!digitalRead(8))
    {
      if (!digitalRead(A4)) {
        persist_input();
        //Change to CONFIRMING
        mode = 'O';
        special_display(reEnter);
        setAlarm(1);
      }
    }
    //Double Right
    if (!digitalRead(A4) && !digitalRead(A5) && (mode == 'U')) {
      if (!digitalRead(9)) { //right button pressed
        rightButtonCounter++;
        if (rightButtonCounter == 1) {
          firstRightButtonPress = now;
        }
        if (rightButtonCounter > 1 && (firstRightButtonPress + DOUBLE_CLICK_TIME) > now) {
          rightButtonCounter = 0;
          mode = 'L';
          special_display(closed);
          setAlarm(1);
        }
        if (rightButtonCounter > 1) {
          rightButtonCounter = 0;
        }
      }
    }
  }
}
void handleButtonConfirming()
{
  //BUTTON CODE
  unsigned long now = millis();
  if ((now - lastButtonPressed > 150) && (!lock_out))//debouncing
  {
    lastButtonPressed  = now;
    // Right is Down and wasn't before
    if (!digitalRead(9))
    {
      advanceCursor();
    }
    // Left
    if (!digitalRead(8))
    {
      if (!digitalRead(A5)) {//right switch is in the left position
        if (attempt_change()) {
          special_display(changed);//Change is Persisted in attempt_change is successful
        } else {
          //FAIL
          special_display(noChange);
        }
        mode = 'U';
      }
    }
  }
}

unsigned long lastKeypadPress = 0;
void handleKeypress()
{
  //KEYPAD CODE
  unsigned long now = millis();
  if ((now - lastKeypadPress > 500u) && (!lock_out))
  {
    lastKeypadPress = now;
    char pressed = cowpi_getKeypress();
    short value;
    switch (pressed) {
      case '#':
        value = 0xE;
        break;
      case '*':
        value = 0xF;
        break;
      case 'A':
        value = 0xA;
        break;
      case 'B':
        value = 0xB;
        break;
      case 'C':
        value = 0xC;
        break;
      case 'D':
        value = 0xD;
        break;
      default:
        value = int(pressed) - '0';
        break;
    }
    if (value == -48)
    {
      return;
    }
    display_array[selected] = sevenSegments[value];
    num_array[selected] = value;
    selected = selectiveDecrement(selected);
    //show_display_array();

  }
}

short selectiveDecrement(short x) {
  x--;
  if ((x == 2) || (x == 5))
  {
    x += 2;
  }
  return x;
}

short signifigent[6] = {0, 1, 3, 4, 6, 7};
short assertCorrect() {
  short value = 2;
  //1 on Correct -- 0 on Incorrect -- -1 on Fail
  for (short x : signifigent) {
    if (display_array[x] == sevenSegments[FLAG_VALUE]) {
      Serial.println("FAIL");
      value = -1;
    }
  }
  if (value == -1) {
    return value;
  }
  for (short x : signifigent) {
    if (num_array[x] != EEPROM_array[x]) {
      Serial.println("Inncorrect");
      value = 0;
    }
  }
  if (value == 0) {
    return value;
  }

  Serial.println("Correct");
  return 1;
}

void read_from_EEPROM() {
  unsigned long vale;
  unsigned long mask = 0b1111;
  EEPROM.get(0, vale);
  short shift_down = 0;
  for (short x : signifigent) {
    EEPROM_array[x] = (vale & mask) >> shift_down;
    mask = mask << 4;
    shift_down += 4;
  }
}

void special_display(uint8_t x[8]) {
  should_cursor = false;
  for (int i = 0; i < 8; i++) {
    
    display_array[i] = x[7 - i];
  }
}

void advanceCursor() {
  //Ensure Current Courser is off
  display_array[cursored] &= ~sevenSegments[CURSOR];
  display_array[cursored - 1] &= ~sevenSegments[CURSOR];

  cursored -= 3;
  if (cursored < 0) {
    cursored = 7;
  }
  selected = cursored;
}
void show_display_array() {
  for (int i = 0; i < 8; i++) {
    cowpi_sendDataToMax7219(i + 1, display_array[i]);
  }
}

void persist_input() {
  for (short x : signifigent) {
    possible_array[x] = num_array[x];
  }
}

long num_array_to_long() {
  unsigned long results = 0;
  short shift = 4;
  for (short x : signifigent) {
    results = (results << shift) | num_array[7-x];
  }
  return results;
}

bool attempt_change() {
  for (short x : signifigent) {
    if (possible_array[x] != num_array[x]) {
      return false;
    }
  }
  long x = num_array_to_long();
  EEPROM.put(0, x);
  Serial.println("Changed");
  Serial.println(x, HEX);
  read_from_EEPROM();
  return true;
}

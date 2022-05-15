void displaySleep() {
  pinMode(SS, OUTPUT);    // pin 10: serial select, aka CS
  pinMode(MOSI, OUTPUT);  // pin 11: MOSI, aka DIN
  pinMode(SCK, OUTPUT);   // pin 13: serial clock, aka CLK
  digitalWrite(SS, LOW);
  shiftOut(MOSI, SCK, MSBFIRST, 0xC);
  shiftOut(MOSI, SCK, MSBFIRST, 0);
  digitalWrite(SS, HIGH);
}

void setup() {
  displaySleep();         // just in case
  Serial.begin(9600);
  // Keypad
  for(int pin = 4; pin < 8; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for(int pin = A0; pin < A4; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
  pinMode(3, INPUT);
  // Switches
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  // Pushbuttons
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(2, INPUT);
}

void loop() {
  Serial.print("KEYPAD COLUMNS: ");
  for(int pin = A0; pin < A4; pin++) {
    Serial.print(digitalRead(pin));
  }
  Serial.print("  ");
  Serial.print("KEYPAD NAND: ");
  Serial.print(digitalRead(3));
  Serial.print("  ");
  Serial.print("LEFT SWITCH: ");
  Serial.print(digitalRead(A4));
  Serial.print("  ");
  Serial.print("RIGHT SWITCH: ");
  Serial.print(digitalRead(A5));
  Serial.print("  ");
  Serial.print("LEFT BUTTON: ");
  Serial.print(digitalRead(8));
  Serial.print("  ");
  Serial.print("RIGHT BUTTON: ");
  Serial.print(digitalRead(9));
  Serial.print("  ");
  Serial.print("BUTTON NAND: ");
  Serial.print(digitalRead(2));
  Serial.println();
  delay(250);     // short enough that we shouldn't miss a button press, but long enough to throttle serial comms
}

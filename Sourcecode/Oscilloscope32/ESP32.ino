/*
  ┌────────────────────────────────────────────────────┐
  │ Oscilloscope32 by TheAmadeus25                                                   ┃
  ├────────────────────────────────────────────────────┤
  │ https://theamadeus25.github.io/Oscilloscope32/                                   ┃
  ├────────────────────────────────────────────────────┤
  │ IMPORTANT! Read the Wiki on GitHub.                                              ┃
  │                                                                                  ┃
  │                                                                                  ┃
  ├────────────────────────────────────────────────────┤
  │ InitESP()  : Configuration of Input/Output                                       ┃
  │ Measuring(): Parsing Information into the Buffer                                 ┃
  │ Mode()     : Check if Button is Pressed                                          ┃
  │                                                                                  ┃
  │                                                                                  ┃
  │                                                                                  ┃
  ├──────────────────────────┬─────────────────────────┤
  │ Version: 0.0.2 - ALPHA                    Date: 25.Sep.2019                      ┃
  ├──────────────────────────┴─────────────────────────┤
  └────────────────────────────────────────────────────┘
*/

void InitESP() {                                    // # Setting Up the ESP32
  pinMode(LED, OUTPUT);                             // Setting Pin to Output
  pinMode(BTN, INPUT);                              // Setting Pin to Input

  return;
}

void LED_Error() {                                              // # Blue LED when Potentiometer reache (close) max value (3.3V)
  if (Multi_X == (BUFFERSIZE / MAX_X) || Offset_Y >= 2040 ) {   // When you Potentiometer reach critical value (16)
    digitalWrite(LED, HIGH);                                    // Built-In Blue LED start shining
  } else {
    digitalWrite(LED, LOW);                                     // Built-In Blue LED stops shining
  }

  return;
}

void Measuring() {                                  // # Obviously, measuring your Input and saving into the Buffer
  for (short i = 0; i <= BUFFERSIZE; i++) {         // Save each value into the buffer
    Buffer[i] = analogRead(SensorPin);              // Read value
  }

  return;
}


void Mode() {                                       // # Check if Button is Pressed
  if (digitalRead(BTN) == LOW) {                    // When pressing the button...
    if (canBtnPress) canBtnPress = false;           // ...change state
  } else if (!canBtnPress) {
    canBtnPress = true;

    ShowOverlay = !ShowOverlay;                     // Change UI
  }

  return;
}

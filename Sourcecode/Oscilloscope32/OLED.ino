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
  │ InitOLED(): Configuration of the OLED Display                                    ┃
  │ Y_Div()   : Stretching Graph                                                     ┃
  │ Plot()    : Show the Graph on Screen                                             ┃
  │ UI()      : Show/Change User-Interface                                           ┃
  │                                                                                  ┃
  │                                                                                  ┃
  ├──────────────────────────┬─────────────────────────┤
  │ Version: 0.0.1 - ALPHA                    Date: 24.Sep.2019                      ┃
  ├──────────────────────────┴─────────────────────────┤
  └────────────────────────────────────────────────────┘
*/

void InitOLED() {                                                                               // # Setting Up OLED Display
  display.init();                                                                               // Initialize OLED Display
  display.flipScreenVertically();                                                               // Flip Screen upsidedown
  // display.invertDisplay();
  display.setFont(ArialMT_Plain_16);                                                            // Setting Font
  display.drawString(6, 6, "Oscilloscope32");                                                   // Print String at 6, 6
  display.setFont(ArialMT_Plain_10);                                                            // Setting Font
  display.drawString(24, 34, "Made with blood by");                                             // Print String at 24, 34
  display.drawString(29, 44, "@TheAmadeus25");                                                  // Print String at 29, 44
  display.display();                                                                            // Show it on the Screen

  delay(1000);                                                                                  // Surreptitious Advertising :P
  display.clear();                                                                              // Clear Display, which show you the old graph

  return;
}

void Y_Div() {                                                                                  // # Stretching Graph Peak-to-Peak (At the moment Center stretch)
  Offset_Y = map( analogRead(25), 0, 4095, 0, 2047 );                                           // Change Range 0-4095 -> 0-2047
  if (Offset_Y >= 2040) {                                                                       // When you Potentiometer reach critical value (2040 of 2048)
    digitalWrite(LED, HIGH);                                                                    // Built-In Blue LED start shining
  } else {
    digitalWrite(LED, LOW);                                                                     // Built-In Blue LED stops shining
  }

  return;
}

void Plot() {                                                                                   // # Plot Graph on Screen
  for (short i = 0; i <= BUFFERSIZE; i++) {                                                     // Print each value from the buffer
    if (Filled) {                                                                               // Change look of the graph
      display.drawLine(i, MAX_Y, i, map(Buffer[i], 0 + Offset_Y, 4095 - Offset_Y, MAX_Y, 0) );  // Draw Line
    } else {
      display.setPixel(i, map(Buffer[i], 0 + Offset_Y, 4095 - Offset_Y, MAX_Y, 0) );            // Draw Dots
    }
  }

  return;
}

void UI() {                                                                                     // # Show/Change User-Interface
  if ( ShowOverlay ) {                                                                          // Show UI?
    display.drawHorizontalLine( 0, 0,  1);  display.drawHorizontalLine( MAX_X - 1, 0,  1);      // Top Row Left and Right Dot
    display.drawHorizontalLine( 0, 16, 1);  display.drawHorizontalLine( MAX_X - 1, 16, 1);      // 2. Row Left and Right Dot
    display.drawHorizontalLine( 0, 31, 3);  display.drawHorizontalLine( MAX_X - 3, 31, 3);      // Middle Left and Right Large Dot
    display.drawHorizontalLine( 0, 48, 1);  display.drawHorizontalLine( MAX_X - 1, 48, 1);      // 4. Row Left and Right Dot
    display.drawHorizontalLine( 0, 63, 1);  display.drawHorizontalLine( MAX_X - 1, 63, 1);      // Botton Row Left and Right Dot

    display.drawVerticalLine( 32,  0, 1);   display.drawVerticalLine( 32,  MAX_Y - 1, 1);       // 2. Top and Botton Dot
    display.drawVerticalLine( 64,  0, 3);   display.drawVerticalLine( 64,  MAX_Y - 3, 3);       // Middle Top and Botton Large Dot
    display.drawVerticalLine( 96,  0, 1);   display.drawVerticalLine( 96,  MAX_Y - 1, 1);       // 4. Row Top and Botton Dot
  } else {
    //display.drawHorizontalLine( 0, 16, MAX_X);
    //display.drawHorizontalLine( 0, 31, MAX_X);
    //display.drawHorizontalLine( 0, 48, MAX_X);

    //display.drawVerticalLine( 32,  0, MAX_Y);
    //display.drawVerticalLine( 64,  0, MAX_Y);
    //display.drawVerticalLine( 96,  0, MAX_Y);
  }

  return;
}

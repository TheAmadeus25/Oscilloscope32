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
  │ X_Div()   : Time Base                                                            ┃
  │ Plot()    : Show the Graph on Screen                                             ┃
  │ UI()      : Show/Change User-Interface                                           ┃
  │ ShowSetting(): Show your setting while changing it                               ┃
  │                                                                                  ┃
  ├──────────────────────────┬─────────────────────────┤
  │ Version: 0.0.2 - ALPHA                    Date: 25.Sep.2019                      ┃
  ├──────────────────────────┴─────────────────────────┤
  │ + X_Div()                                                                        ┃
  │ + Y_Div()                                                                        ┃
  │ + ShowSetting()                                                                  ┃
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
  Offset_Y = map( analogRead(Y_POT), 0, 4095, 0, 2047 );                                        // Change Range 0-4095 -> 0-2047

  if ( (map(Offset_Y, 0, 2047, 1, 10)) != (map(Offset_Y_Last, 0, 2047, 1, 10)) ) {              // Compare now and last value of Offset_Y
    Y.last_refresh = millis();                                                                  // Setting time of last comparing was false
  }

  Offset_Y_Last = Offset_Y;                                                                     // Now -> Last

  return;
}

void X_Div() {                                                                                  // # Changing Timebase range on Display
  Multi_X = map( analogRead(X_POT), 0, 4095, 1, BUFFERSIZE / MAX_X);                            // Calculate streching Timebase on Display

  if (Multi_X != Multi_X_Last) {                                                                // Compare now and last value of Multi_X
    X.last_refresh = millis();                                                                  // Setting time of last comparing was false
  }

  Multi_X_Last = Multi_X;                                                                       // Now -> Last

  return;
}

void Plot() {                                                                                               // # Plot Graph on Screen
  for (short i = 0; i <= BUFFERSIZE / Multi_X; i++) {                                                       // Print each value from the buffer
    if (Filled || Multi_X >= 4) {                                                                           // Change look of the graph if resolution is to high for Display (4 is a good value)
      display.drawLine(i, MAX_Y, i, map(Buffer[i * Multi_X], 0 + Offset_Y, 4095 - Offset_Y, MAX_Y, 0) );    // Draw Line
    } else {
      display.setPixel(i, map(Buffer[i * Multi_X], 0 + Offset_Y, 4095 - Offset_Y, MAX_Y, 0) );              // Draw Dots
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

void ShowSetting() {                                                                            // # Show your setting while changing it
  if ( (Y.last_refresh + 500) > millis() ) {                                                    // If Y-Range has changed,...
    display.drawString(  0, 0, (String)(map(Offset_Y, 0, 2047, 1, 10)) + " Y-Axis" );           // ...show this Information
  } else if (X.last_refresh + 500 > millis() ) {                                                // If X-Range has changed,...
    display.drawString(0, 0, (String)Multi_X + "x Time");                                       // ...show this Information
    if(Filled == false && Multi_X >= 4){                                                        // When dots instead of lines are drawing and resolution is to high for Display,...
      display.drawString(112, 0, "BW");                                                         // ...show Black/White (BW) indicator
    }
    
  }

  return;
}

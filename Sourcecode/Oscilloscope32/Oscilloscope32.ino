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
  │ To be honest, I never planed to do this. I got my ESP32 and saw, that it has     ┃
  │ very nice hardware built-in. After checking the datasheet, I decided to plot the ┃
  │ ADC on a screen, to see how good it works. After attaching a Potentiometer, LDR  ┃
  │ and a microphone, I found the last on was pretty interesting to see. So, what    ┃
  │ now? I release a video about it while playing music, on social media and I got   ┃
  │ Hugh response of it. It seems that there are many people who are interested in   ┃
  │ this. Now it's released on GitHub and hope you are happy with it. Yeah, I know,  ┃
  │ it doesn't have any built-in feature, yet, but maybe I program this, too.        ┃
  ├──────────────────────────┬─────────────────────────┤
  │ Version: 0.0.2 - ALPHA                    Date: 25.Sep.2019                      ┃
  ├──────────────────────────┴─────────────────────────┤
  │ + Change Timebase                                                                ┃
  │ - Not optimizing Var, yet                                                        ┃
  │ - Not compatible for ESP8266 anymore                                             ┃
  └────────────────────────────────────────────────────┘
*/

#include "SH1106.h"                         // Library | Inside of Arduino IDE, search for "OLED SSD1306 - SH1106" (I hope its the right one xD)
#define SDA_PIN 26                          // Screen SDA
#define SCL_PIN 27                          // Screen SCL
#define MAX_X 128                           // Display Hight (not optimized, yet)
#define MAX_Y 64                            // Display Width (not optimized, yet)

SH1106 display(0x3C, SDA_PIN, SCL_PIN);     // Setting Port for Display

#define SensorPin 33                        // Your Input Pin 33
#define LED 2                               // Built-In Blue LED on ESP32
#define BTN 0                               // Built-In Button on ESP32

#define Y_POT 25                            // Attach Potentiometer for Y-Axis
#define X_POT 13                            // Attach Potentiometer for Time Axis

bool canBtnPress = true;                    // Button available
bool ShowOverlay = true;                    // Show Overlay at startup

#define BUFFERSIZE 2048                     // Max Size of Buffer (In this Case MAX_X*8) 128

uint16_t Buffer[BUFFERSIZE + 1] = {0};      // Analog Read Buffer (The ESP32 measure much faster than the Display has Time to show the graph)
uint16_t Offset_Y = 0;                      // On Screen Peak-to-Peak Stretching (Similar to Y-Div setting)
uint16_t Offset_Y_Last = 0;                 // Last Value of Offset_Y

short Multi_X = 0;                          // On Screen how many data will be showen from buffer (Multiplicator)
short Multi_X_Last = 0;                     // Last Value of Multi_X

struct {                                    
  unsigned long last_refresh = 0;           // Two value must be saved for different time
} X, Y;


bool Filled = false;                        // Show Filled graph insted of dots (Better for very small or high changes (Buggy)

//---
void setup() {
  InitESP();                                // Setting Up the ESP32    | @ESP32
  InitOLED();                               // Setting Up OLED Display | @OLED
}
//---
void loop() {
  X_Div();                                  // Changing Timebase range on Display                           | @OLED
  Y_Div();                                  // Stretching Graph Peak-to-Peak (At the moment Center stretch) | @OLED
  LED_Error();                              // Blue LED when Potentiometer reache (close) max value (3.3V)  | @ESP32
  
  Measuring();                              // Obviously, measuring your Input and saving into the Buffer   | @ESP32
  display.clear();                          // Clear Display, which show you the old graph
  Plot();                                   // Plot Graph on Screen                                         | @OLED
  Mode();                                   // Check if Button is Pressed                                   | @ESP32
  UI();                                     // Show/Change User-Interface                                   | @OLED
  ShowSetting();
  display.display();                        // Show it on the Screen
}
//---

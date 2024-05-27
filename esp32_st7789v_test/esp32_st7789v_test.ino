#include <driver/gpio.h>
#include "esp32_ST7789V/esp32_ST7789V.h"

void setup() {
  // Initialize serial communication (optional for debugging)
  Serial.begin(115200);

  Lcd_Init();       // Initialize LCD
  // drawRect(0,0,320,240, BLUE);
}

void loop() {
  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n'); // Read until newline

    // Split the string using the delimiter (replace ',' with your desired delimiter)
    int firstInt = inputString.toInt();
    int delimiterIndex = inputString.indexOf(' ');
    if (delimiterIndex != -1) {
      String secondString = inputString.substring(delimiterIndex + 1);
      int secondInt = secondString.toInt();
      
      // Now you have both integers: firstInt and secondInt
      Serial.print(firstInt);
      Serial.print(" ");
      Serial.println(secondInt);
      drawPixel(firstInt, secondInt, GREEN);
    } else {
      Serial.println("Invalid input format! Please enter two comma-separated integers.");
    }
  }
}
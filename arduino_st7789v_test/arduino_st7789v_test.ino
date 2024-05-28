#include "arduino_ST7789V/arduino_ST7789V.h"

// Function to convert RGB to 16-bit color
uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t r5 = (r >> 3) & 0x1F;  // 5 bits
    uint16_t g6 = (g >> 2) & 0x3F;  // 6 bits
    uint16_t b5 = (b >> 3) & 0x1F;  // 5 bits

    // Combine the bits into a single 16-bit value
    return (r5 << 11) | (g6 << 5) | b5;
}

void setup() {
    Lcd_Init();
    Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n'); // Read until newline
    inputString.trim(); // Remove any leading or trailing whitespace

    int values[7]; // Array to hold pos_x, pos_y, red, green, blue
    int startIndex = 0;
    int spaceIndex;
    char delimeter = ',';

    for (int i = 0; i < 5; i++) {
        spaceIndex = inputString.indexOf(delimeter, startIndex);
        if (spaceIndex == -1 && i < 4) { // If space not found and not all values are parsed
            Serial.println("Invalid input format! Please enter values separated by spaces.");
            return;
        }
        values[i] = inputString.substring(startIndex, spaceIndex).toInt();
        startIndex = spaceIndex + 1;
    }

    uint16_t color = rgb(values[4], values[5], values[6]);

    // Draw the pixel at the specified position with the specified color
    drawRect(values[0], values[1], values[2], values[3], color);
  }
}

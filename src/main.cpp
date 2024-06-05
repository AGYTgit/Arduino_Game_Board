#include <Arduino.h>
#include <VS7789V.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcdtft(0x27, 16, 2);

// #include <block/block.h>

// Function to convert RGB to 16-bit color
uint16_t rgb(int r, int g, int b) {
  uint16_t r5 = (r >> 3) & 0x1F;  // 5 bits
  uint16_t g6 = (g >> 2) & 0x3F;  // 6 bits
  uint16_t b5 = (b >> 3) & 0x1F;  // 5 bits

  // Combine the bits into a single 16-bit value
  return (r5 << 11) | (g6 << 5) | b5;
}

arduino_ST7789V lcd = arduino_ST7789V();

void setup() {
  lcd.Init();
  lcd.fill();

  lcdtft.init();
  lcdtft.backlight();
}

void loop() {
  // if (Serial.available()) {
  //   String inputString = Serial.readStringUntil('\n'); // Read until newline
  //   inputString.trim(); // Remove any leading or trailing whitespace

  //   int values[8]; // Array to hold pos_x, pos_y, red, green, blue
  //   int startIndex = 0;
  //   int spaceIndex;
  //   char delimeter = ',';

  //   for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
  //       spaceIndex = inputString.indexOf(delimeter, startIndex);
  //       if (spaceIndex == -1 && i < 4) { // If space not found and not all values are parsed
  //           Serial.println("Invalid input format! Please enter values separated by spaces.");
  //           return;
  //       }
  //       values[i] = inputString.substring(startIndex, spaceIndex).toInt();
  //       startIndex = spaceIndex + 1;
  //   }

  //   uint16_t color = rgb(values[4], values[5], values[6]);

  //   // Draw the pixel at the specified position with the specified color
  //   draw_frame(values[0], values[1], values[2], values[3], values[7], color);
  // }


  
  for (int i = 0; i < 20; i++) {
    lcd.draw_rect(112, 0 + (i - 1) * 16, 16, 16, rgb(0,0,0));
    lcd.draw_rect(96, 16 + (i - 1) * 16, 16, 16, rgb(0,0,0));
    lcd.draw_rect(128, 16 + (i - 1) * 16, 16, 16, rgb(0,0,0));

    lcd.draw_rect(112, 0 + i * 16, 16, 16, rgb(255,0,255));
    lcd.draw_rect(96, 16 + i * 16, 16, 16, rgb(255,0,255));
    lcd.draw_rect(112, 16 + i * 16, 16, 16, rgb(255,0,255));
    lcd.draw_rect(128, 16 + i * 16, 16, 16, rgb(255,0,255));
    delay(500);

    lcdtft.setCursor(8,0);
    lcdtft.print("        ");
    for (int i = 0, j = 7; i < 8; i++, j--) {
      lcdtft.setCursor(i,0);
      lcdtft.print((((PORTD & B11111100) | (PORTB & B00000011)) >> j) & 0x01);
    }
    lcdtft.setCursor(8,1);
    lcdtft.print("        ");
    for (int i = 0, j = 7; i < 8; i++, j--) {
      lcdtft.setCursor(i,1);
      lcdtft.print((PORTD >> j) & 0x01);
    }
  }
}

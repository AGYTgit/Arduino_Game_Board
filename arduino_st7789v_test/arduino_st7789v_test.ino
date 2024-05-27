#include "ST7789V/ST7789V.h"

void setup() {
  // Setting Pin 2-7 as Output, DDR is PinMode Command, Pin0,1 Untouched
  DDRD = DDRD | B11111100;
  // Setting Pin 8-9 as Output
  DDRB = DDRB | B00000011;
  //Setting Analog Pins A4-A0 as Output
  DDRC = DDRC | B00011111;
  // Setting Analog Pins A4-A0 as HIGH
  PORTC = PORTC | B00011111;
  delay(1000);
  Lcd_Init();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    int data = Serial.parseInt();
    Serial.println(data);
    if (data == 0) {
      drawRect(0,0,320,240, BLACK);
    } else if (data == 1) {
      drawRect(0,0,320,240, BLUE);
    } else if (data == 2) {
      drawRect(0,100,320,40, RED);
    }
    while (Serial.available()) {
        Serial.read();
      }
  }
}

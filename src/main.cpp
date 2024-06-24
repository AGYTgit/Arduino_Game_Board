#include <Arduino.h>
#include <ST7789V.h>

#include <board/board.h>

#include <block_data/block_data.h>

ST7789V lcd = ST7789V();
Board board = Board();

Block block = {0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  lcd.Init();
  lcd.fill();

  // board.add_block(block);

  board.draw(lcd);
}

void loop() {
  board.rotate_block(block, DIRECTION::CW);
  board.draw(lcd);
  delay(500);
}
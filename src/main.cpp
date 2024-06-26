#include <Arduino.h>
#include <ST7789V.h>

#include <board/board.h>

#include <block_data/block_data.h>

ST7789V lcd = ST7789V();
Board board = Board();

Block block = {5, 5, 5, 0};
Block block2 = {5, 3, 10, 0};

void setup() {
  Serial.begin(9600);
  lcd.Init();
  lcd.fill();

  board.add_block(block);
  board.add_block(block2);

  board.draw(lcd);
}

void loop() {
  board.move_block(block, DIRECTION::DOWN);
  board.draw(lcd);
  delay(500);
}

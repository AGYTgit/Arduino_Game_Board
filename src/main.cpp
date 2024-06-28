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

  board.draw(lcd);
}

void loop() {
  if (Serial.available()) {
    int input = Serial.parseInt();

    if (input == 5) {
      board.rotate_block(block, DIRECTION::CCW);
    } else if (input == 6) {
      board.rotate_block(block, DIRECTION::CW);
    } else if (input == 1) {
      board.move_block(block, DIRECTION::UP);
    } else if (input == 2) {
      board.move_block(block, DIRECTION::DOWN);
    } else if (input == 3) {
      board.move_block(block, DIRECTION::LEFT);
    } else if (input == 4) {
      board.move_block(block, DIRECTION::RIGHT);
    } else if (input == 0) {
      while (!Serial.available());
      int8_t input2 = Serial.parseInt();
      block = {input2, 0, 0, 0};
      board.add_block(block);
      while (Serial.available()) {
        Serial.read();
      }
    }

    Serial.print(block.X);
    Serial.print(' ');
    Serial.println(block.Y);
    board.draw(lcd);

    while (Serial.available()) {
      Serial.read();
    }
  }
}

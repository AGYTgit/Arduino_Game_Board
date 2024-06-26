#include <Arduino.h>
#include <ST7789V.h>

#include <board/board.h>

#include <block_data/block_data.h>

ST7789V lcd = ST7789V();
Board board = Board();

Block block = {0, 0, 0, 0};
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
  // board.move_block(block, DIRECTION::DOWN);

  // board.rotate_block(block, DIRECTION::CW);
  // board.draw(lcd);
  // delay(500);

  if (Serial.available()) {
    int input = Serial.parseInt();
    Serial.println(input);

    if (input == 5) {
      board.rotate_block(block, DIRECTION::CW);
    } else if (input == 6) {
      board.rotate_block(block, DIRECTION::CCW);
    } else if (input == 1) {
      board.move_block(block, DIRECTION::UP);
    } else if (input == 2) {
      board.move_block(block, DIRECTION::DOWN);
    } else if (input == 3) {
      board.move_block(block, DIRECTION::LEFT);
    } else if (input == 4) {
      board.move_block(block, DIRECTION::RIGHT);
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

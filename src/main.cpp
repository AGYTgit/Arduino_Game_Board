#include <Arduino.h>
#include <ST7789V.h>

#include <board/board.h>

#include <block_data/block_data.h>

// #include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcdtft(0x27, 16, 2);

ST7789V lcd = ST7789V();
Board board = Board();

Block block = {5, 3, 0, 0};

void setup() {
  lcd.Init();
  lcd.fill();

  // lcdtft.init();
  // lcdtft.backlight();

  DDRB |= B00110000;
  DDRB &= B11110011;
  PORTB |= B00001100;
  PORTB &= B11001111;

  board.add_block(block);
  board.draw(lcd);
}

bool test[2] = {true, true};

void loop() {
  if ((PINB & 1<<2) == 0 && test[0] == true) {
    test[0] = false;
  } else if ((PINB & 1<<2) != 0 && test[0] == false) {
    test[0] = true;
    board.remove_block(block);
    block.ROTATION = (block.ROTATION + 1) % 4;
    board.add_block(block);
    board.draw(lcd);
  }

  if ((PINB & 1<<3) == 0 && test[1] == true) {
    test[1] = false;
  } else if ((PINB & 1<<3) != 0 && test[1] == false) {
    test[1] = true;
    board.remove_block(block);
    block.ROTATION = (block.ROTATION + 3) % 4;
    board.add_block(block);
    board.draw(lcd);
  }
  delayMicroseconds(500);
}

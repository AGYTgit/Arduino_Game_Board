#include <ST7789V.h>

// #include <board/board.h>

// #include <block_data/block_data.h>

// ST7789V lcd = ST7789V();
// Board board = Board();

// Block block = {5, 3, 0, 0};

void setup() {
  Serial.begin(9600);

  // lcd.Init();
  // lcd.fill();

  DDRD |= B11100000; // button input
  PORTD |= B11100000;
  
  DDRB &= B11111000; // button output
  PORTB &= B11111000;

  // board.add_block(block);
  // board.draw(lcd);
}

bool test[3][3] = {
  {true,true,true},
  {true,true,true},
  {true,true,true}
};

void loop() {
  for (int i = 7; i > 4; i--) {
    PORTD = 001<<i;
    for (int j = 0; j < 3; j++) {
      PORTB = ~(001<<j);
      Serial.println(PIND, BIN);
      Serial.println(PINB, BIN);
      Serial.println("  ----  ");
      if (digitalRead(i) == LOW && test[i - 5][j]) {
        test[i - 5][j] = false;
      } else if (digitalRead(i) != LOW && !test[i - 5][j]) {
        test[i - 5][j] = true;
        Serial.print(i);
        Serial.print(' ');
        Serial.println(j);
      }
      delayMicroseconds(50);
    }
  }
}

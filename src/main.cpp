#include <Arduino.h>
// #include <ST7789V.h>

// #include <gui.h>

// #include <Tetris.h>


// ST7789V lcd = ST7789V();

// Board board = Board();

// Block block = {0, 0, 0, 0};

#define BUTTONS_WIDTH 2
#define BUTTONS_HEIGHT 2

#define INPUT_BUTTON_OFFSET 2
#define OUTPUT_BUTTON_OFFSET 4

bool test[BUTTONS_HEIGHT][BUTTONS_WIDTH];

void setup() {
  Serial.begin(115200);

  for (int ib = 0; ib < BUTTONS_WIDTH; ib++) {
    pinMode(ib + INPUT_BUTTON_OFFSET, INPUT_PULLUP);
    for (int jb = 0; jb < BUTTONS_HEIGHT; jb++) {
      pinMode(jb + OUTPUT_BUTTON_OFFSET, OUTPUT);
      test[jb][ib] = true;
    }
  }

  // lcd.Init();
  // lcd.fill();

  // board.draw(lcd);
}

void loop() {
  for (int i = 0; i < BUTTONS_WIDTH; i++) {
    digitalWrite(i + INPUT_BUTTON_OFFSET, HIGH);
    for (int ip = 1; ip < BUTTONS_WIDTH; ip++) {
      digitalWrite(((i + ip) % BUTTONS_WIDTH) + INPUT_BUTTON_OFFSET, LOW);
    }

    for (int j = 0; j < BUTTONS_HEIGHT; j++) {
      digitalWrite(j + OUTPUT_BUTTON_OFFSET, LOW);
      for (int jp = 1; jp < BUTTONS_HEIGHT; jp++) {
        digitalWrite(((j + jp) % BUTTONS_HEIGHT) + OUTPUT_BUTTON_OFFSET, HIGH);
      }
      
      if (digitalRead(i + INPUT_BUTTON_OFFSET) == LOW && test[j][i]) {
        test[j][i] = false;
      } else if (digitalRead(i + INPUT_BUTTON_OFFSET) != LOW && !test[j][i]) {
        test[j][i] = true;
        Serial.print(j);
        Serial.print(' ');
        Serial.println(i);
      }
      delay(10);
    }
  }

  // if (Serial.available()) {
  //   int input = Serial.parseInt();

  //   if (input == 5) {
  //     board.rotate_block(block, DIRECTION::CCW);
  //   } else if (input == 6) {
  //     board.rotate_block(block, DIRECTION::CW);
  //   } else if (input == 1) {
  //     board.move_block(block, DIRECTION::UP);
  //   } else if (input == 2) {
  //     board.move_block(block, DIRECTION::DOWN);
  //   } else if (input == 3) {
  //     board.move_block(block, DIRECTION::LEFT);
  //   } else if (input == 4) {
  //     board.move_block(block, DIRECTION::RIGHT);
  //   } else if (input == 0) {
  //     while (!Serial.available());
  //     int8_t input2 = Serial.parseInt();
  //     block = {input2, 0, 0, 0};
  //     board.add_block(block);
  //   } else if (input == 7) {
  //     Serial.println(board.clear_completed_lines());
  //   }



  //   Serial.print(block.X);
  //   Serial.print(' ');
  //   Serial.println(block.Y);
  //   board.draw(lcd);

  //   while (Serial.available()) {
  //     Serial.read();
  //   }
  // }
}

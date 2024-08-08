// #include <Arduino.h>
// #include <ST7789V.h>

// #include <gui.h>

// #include <Tetris.h>


// ST7789V lcd = ST7789V();

// Board board = Board();

// Block block = {0, 0, 0, 0};

// void setup() {
//   Serial.begin(115200);

//   lcd.Init();
//   lcd.fill();

//   board.draw(lcd);
// }

// void loop() {
//   if (Serial.available()) {
//     int input = Serial.parseInt();

//     if (input == 5) {
//       board.rotate_block(block, DIRECTION::CCW);
//     } else if (input == 6) {
//       board.rotate_block(block, DIRECTION::CW);
//     } else if (input == 1) {
//       board.move_block(block, DIRECTION::UP);
//     } else if (input == 2) {
//       board.move_block(block, DIRECTION::DOWN);
//     } else if (input == 3) {
//       board.move_block(block, DIRECTION::LEFT);
//     } else if (input == 4) {
//       board.move_block(block, DIRECTION::RIGHT);
//     } else if (input == 0) {
//       while (!Serial.available());
//       int8_t input2 = Serial.parseInt();
//       block = {input2, 0, 0, 0};
//       board.add_block(block);
//     } else if (input == 7) {
//       Serial.println(board.clear_completed_lines());
//     }

//     Serial.print(block.X);
//     Serial.print(' ');
//     Serial.println(block.Y);
//     board.draw(lcd);

//     while (Serial.available()) {
//       Serial.read();
//     }
//   }
// }

#include <Arduino.h>

// #include <ST7789V.h>

// #include <board/board.h>

// #include <block_data/block_data.h>

// ST7789V lcd = ST7789V();
// Board board = Board();

// Block block = {5, 3, 0, 0};

void setup() {
  Serial.begin(9600);

  // lcd.Init();
  // lcd.fill();

  pinMode(7, INPUT_PULLUP);
  digitalWrite(7, HIGH);
  
  pinMode(8, OUTPUT);

  // board.add_block(block);
  // board.draw(lcd);
}

// bool test[3][3] = {
//   {true,true,true},
//   {true,true,true},
//   {true,true,true}
// };

void loop() {
  // if (digitalRead(7) != 0 && test[0]) {
  //   test[0][0] = false;
  //   Serial.println(0);
  // } else if (digitalRead(7) == 0 && !test[0]) {
  //   test[0][0] = true;
  //   Serial.println(1);
  // }
  Serial.println(digitalRead(7));
}

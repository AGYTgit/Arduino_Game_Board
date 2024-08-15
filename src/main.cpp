#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>
#include <Tetris.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x24);


uint8_t game_select_menu() {
  Menu m = Menu(lcd, 0x12, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(0, 0, 70, 65, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));
    m.add_button(0, 1, 70, 180, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw();
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        m.move(2);
        break;
      case 0x11:
        m.move(1);
        break;
      case 0x12:
        m.move(0);
        break;
      case 0x13:
        m.move(3);
        break;
      case 0x02:
        switch (m.get_position()) {
          case 0x00:
            m.undraw();
            return 1;
            break;
          case 0x01:
            m.undraw();
            return 5;
            break;
        }
        break;
    }
  }
}

uint8_t tetris_menu() {
  Menu m = Menu(lcd, 0x13, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
    m.add_button(0, 1, 70, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw(); 
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        m.move(2);
        break;
      case 0x11:
        m.move(1);
        break;
      case 0x12:
        m.move(0);
        break;
      case 0x13:
        m.move(3);
        break;
      case 0x02:
        switch (m.get_position()) {
          case 0x00:
            m.undraw();
            return 2;
            break;
          case 0x01:
            lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,0,255));
            break;
          case 0x02:
            m.undraw();
            return 0;
            break;
        }
        break;
    }
  }
}

uint8_t tetris_game() {
  Board board = Board();
  Block block;

  uint16_t time_to_drop = 1000;
  uint16_t time_to_move = 3000;

  unsigned long time_of_last_drop = 0;
  unsigned long time_of_last_move = 0;

  if (true) { // setup
    board.draw(lcd);
    
    board.add_next_block();
    time_of_last_drop = millis();
    time_of_last_move = millis();
    board.draw(lcd);
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        if (board.move_block(DIRECTION::LEFT)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x11:
        if (board.move_block(DIRECTION::DOWN)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x12:
        board.drop();
        board.draw(lcd);
        time_of_last_move = millis() + time_to_move;
        break;
      case 0x13:
        if (board.move_block(DIRECTION::RIGHT)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x00:
        if (board.rotate_block(DIRECTION::CCW)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x03:
        if (board.rotate_block(DIRECTION::CW)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x01:
        // hold block
        break;
      case 0x02:
        return 1;
        break;
    }

    if (millis() - time_of_last_drop > time_to_drop) {
      if (board.move_block(DIRECTION::DOWN)) {
        board.draw(lcd);
        time_of_last_drop = millis();
        time_of_last_move = millis();
      }

      if (millis() - time_of_last_move > time_to_move) {
        if (block.Y <= 0) {
          return 1;
        }

        board.clear_completed_lines();
        board.draw(lcd);
  
        board.add_next_block();
        board.draw(lcd);
        time_of_last_drop = millis();
        time_of_last_move = millis();
      }
    }
  }
}  

uint8_t minesweeper_menu() {
  Menu m = Menu(lcd, 0x13, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 1, 70, 120, 100, 50, lcd.rgb(0,255,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw();
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        m.move(2);
        break;
      case 0x11:
        m.move(1);
        break;
      case 0x12:
        m.move(0);
        break;
      case 0x13:
        m.move(3);
        break;
      case 0x02:
        switch (m.get_position()) {
          case 0x00:
            lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,255));
            break;
          case 0x01:
            lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,255,255));
            break;
          case 0x02:
            m.undraw();
            return 0;
            break;
        }
        break;
    }
  }
}


void setup() {
  // Serial.begin(115200);
  // Serial.println(1);

  lcd.Init();
  lcd.fill();
  
  button_grid.init();
}

uint8_t menu_index = 0;

void loop() {
  // bug with button on 0x10

  // uint8_t scan = button_grid.scan();
  // if (scan != 0xFF) {
  //   Serial.println(scan, HEX);
  // }
  switch (menu_index) {
    case 0:
      menu_index = game_select_menu();
      break;
    case 1:
      menu_index = tetris_menu();
      break;
    case 2:
      menu_index = tetris_game();
      break;
    case 5:
      menu_index = minesweeper_menu();
      break;
    
    default:
      break;
  }
}

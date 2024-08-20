#include <Arduino.h>
#include <gui.h>
#include <ui.h>
#include <Tetris.h>
// #include <Minesweeper.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x24);


void tetris_game() {
  Board board = Board(0, 0, 176, 8);

  uint16_t time_to_drop = 1000;
  uint16_t time_to_move = 3000;

  unsigned long time_of_last_drop = 0;
  unsigned long time_of_last_move = 0;

  bool first_hold_state = true;
  bool hold_allowed_state = true;

  if (true) { // setup
    board.draw(lcd);
    
    board.add_next_block();
    board.draw(lcd);
    time_of_last_drop = millis();
    time_of_last_move = millis();

    board.display_future_blocks(lcd);
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        if (board.move_block(TETRIS_DIRECTION::LEFT)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x11:
        if (board.move_block(TETRIS_DIRECTION::DOWN)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x12:
        board.drop();
        board.draw(lcd);
        time_of_last_drop = 0;
        time_of_last_move = millis() + time_to_move;
        break;
      case 0x13:
        if (board.move_block(TETRIS_DIRECTION::RIGHT)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x00:
        if (board.rotate_block(TETRIS_DIRECTION::CCW)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
      case 0x01:
        if (hold_allowed_state) {
          hold_allowed_state = false;
          board.hold();
          board.draw(lcd);
          if (first_hold_state) {
            first_hold_state = false;
            board.display_future_blocks(lcd);
          }
          board.display_hold_block(lcd);
          time_of_last_drop = millis();
          time_of_last_move = millis();
        }
        break;
      case 0x02:
        return;
        break;
      case 0x03:
        if (board.rotate_block(TETRIS_DIRECTION::CW)) {
          board.draw(lcd);
          time_of_last_move = millis();
        }
        break;
    }

    if (millis() - time_of_last_drop >= time_to_drop) {
      if (board.move_block(TETRIS_DIRECTION::DOWN)) {
        board.draw(lcd);
        time_of_last_drop = millis();
        time_of_last_move = millis();
      }
    }

    if (millis() - time_of_last_move >= time_to_move) {
      if (board.get_block_y() <= 1) {
        return;
      }

      hold_allowed_state = true;

      board.clear_completed_lines();
      board.draw(lcd);

      board.add_next_block();
      board.draw(lcd);
      time_of_last_drop = millis();
      time_of_last_move = millis();

      board.display_future_blocks(lcd);
    }
  }
}

void tetris_menu() {
  Menu m = Menu(0x13, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(lcd, 0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
    m.add_button(lcd, 0, 1, 70, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(lcd, 0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

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
            tetris_game();
            m.draw();
            break;
          case 0x01:
            lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,0,255));
            break;
          case 0x02:
            m.undraw();
            return;
            break;
        }
        break;
    }
  }
}


void minesweeper_game() {


  if (true) { // setup
    lcd.draw_rect(0,0,50,50,lcd.rgb(255,255,255));
  }

  while (true) { // loop
    switch (button_grid.scan()) {
      case 0xFF:
        break;
      case 0x10:
        break;
      case 0x11:
        break;
      case 0x12:
        break;
      case 0x13:
        break;
      case 0x00:
        break;
      case 0x01:
        break;
      case 0x02:
        lcd.draw_rect(0,0,50,50,lcd.rgb(0,0,0));
        return;
        break;
      case 0x03:
        break;
    }
  }
}

void minesweeper_menu() {
  Menu m = Menu(0x13, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(lcd, 0, 0, 70, 50, 100, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(lcd, 0, 1, 70, 120, 100, 50, lcd.rgb(0,255,255), 2, lcd.rgb(255,255,255));
    m.add_button(lcd, 0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

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
            minesweeper_game();
            m.draw();
            break;
          case 0x01:
            lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,255,255));
            break;
          case 0x02:
            m.undraw();
            return;
            break;
        }
        break;
    }
  }
}


void game_select_menu() {
  Menu m = Menu(0x12, lcd.rgb(0,0,0));

  if (true) { // setup
    m.init();

    m.add_button(lcd, 0, 0, 70, 65, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));
    m.add_button(lcd, 0, 1, 70, 180, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

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
            tetris_menu();
            m.draw();
            break;
          case 0x01:
            m.undraw();
            minesweeper_menu();
            m.draw();
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

  game_select_menu();
}

void loop() {
  // minesweeper_game();
}

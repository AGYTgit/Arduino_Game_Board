#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>
#include <Tetris.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x24);


uint8_t game_select_menu() {
  Menu m = Menu(lcd, 0x12, lcd.rgb(0,0,0));

  if (1) { // setup
    m.init();

    m.add_button(0, 0, 70, 65, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));
    m.add_button(0, 1, 70, 180, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw();
  }

  if (1) { // loop
    while (true) {
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
}

uint8_t tetris_menu() {
  Menu m = Menu(lcd, 0x13, lcd.rgb(0,0,0));

  if (1) { // setup
    m.init();

    m.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
    m.add_button(0, 1, 70, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw(); 
  }

  if (1) { // loop
    while (true) {
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
}

uint8_t tetris_game() {
  Board board = Board();
  Block block = {0, 0, 0, 0};

  if (1) { // setup
    board.draw(lcd);
  }

  if (1) { // loop
    while (true) {
      switch (button_grid.scan()) {
        case 0xFF:
          break;
        case 0x10:
          board.move_block(block, DIRECTION::LEFT);
          board.draw(lcd);
          break;
        case 0x11:
          board.move_block(block, DIRECTION::DOWN);
          board.draw(lcd);
          break;
        case 0x12:
          board.move_block(block, DIRECTION::UP);
          board.draw(lcd);
          break;
        case 0x13:
          board.move_block(block, DIRECTION::RIGHT);
          board.draw(lcd);
          break;
        case 0x00:
          board.rotate_block(block, DIRECTION::CCW);
          board.draw(lcd);
          break;
        case 0x03:
          board.rotate_block(block, DIRECTION::CW);
          board.draw(lcd);
          break;
        case 0x01:
          board.clear_completed_lines();
          board.draw(lcd);
          block = {(int8_t)(analogRead(A5) % 7), 0, 0, 0};
          board.add_block(block);
          board.draw(lcd);
          break;
        case 0x02:
          return 0;
          break;
      }
    }
  }
}

uint8_t minesweeper_menu() {
  Menu m = Menu(lcd, 0x13, lcd.rgb(0,0,0));

  if (1) { // setup
    m.init();

    m.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 1, 70, 120, 100, 50, lcd.rgb(0,255,255), 2, lcd.rgb(255,255,255));
    m.add_button(0, 2, 70, 190, 100, 50, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    m.draw();
  }

  if (1) { // loop
    while (true) {
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

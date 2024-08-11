#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x23);


uint8_t game_select_menu() {
  Menu game_select_menu = Menu(lcd, 0x12, lcd.rgb(0,0,0));

  if (1) { // setup
    game_select_menu.init();

    game_select_menu.add_button(0, 0, 70, 65, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));
    game_select_menu.add_button(0, 1, 70, 180, 100, 75, lcd.rgb(255,255,255), 2, lcd.rgb(255,0,255));

    game_select_menu.draw();
  }

  if (1) { // loop
    while (true) {
      uint8_t ui_button_coordinations = button_grid.scan();

      if (ui_button_coordinations == 0xFF) {
        continue;
      }

      uint16_t ui_button_code = (ui_button_coordinations >> 4) + ((ui_button_coordinations & 0x0F) * 2);

      if (ui_button_code == 4) {
        uint8_t gui_button_coordinations = game_select_menu.get_position();

        game_select_menu.undraw();
        if (gui_button_coordinations == 0x00) {
          return 1;
        } else if (gui_button_coordinations == 0x01) {
          return 2;
        }

      } else if (ui_button_code >= 0 && ui_button_code <= 3) {
        if (ui_button_code == 0) {
          game_select_menu.move(1);
        } else if (ui_button_code == 1) {
          game_select_menu.move(0);
        } else if (ui_button_code == 2) {
          game_select_menu.move(2);
        } else if (ui_button_code == 3) {
          game_select_menu.move(3);
        }
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
      uint8_t ui_button_coordinations = button_grid.scan();

      if (ui_button_coordinations == 0xFF) {
        continue;
      }

      uint16_t ui_button_code = (ui_button_coordinations >> 4) + ((ui_button_coordinations & 0x0F) * 2);

      if (ui_button_code == 4) {
        uint8_t gui_button_coordinations = m.get_position();

        if (gui_button_coordinations == 0x00) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,0));
        } else if (gui_button_coordinations == 0x01) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,0,255));
        } else if (gui_button_coordinations == 0x02) {
          m.undraw();
          return 0;
        }

      } else if (ui_button_code >= 0 && ui_button_code <= 3) {
        if (ui_button_code == 0) {
          m.move(1);
        } else if (ui_button_code == 1) {
          m.move(0);
        } else if (ui_button_code == 2) {
          m.move(2);
        } else if (ui_button_code == 3) {
          m.move(3);
        }
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
      uint8_t ui_button_coordinations = button_grid.scan();

      if (ui_button_coordinations == 0xFF) {
        continue;
      }

      uint16_t ui_button_code = (ui_button_coordinations >> 4) + ((ui_button_coordinations & 0x0F) * 2);

      if (ui_button_code == 4) {
        uint8_t gui_button_coordinations = m.get_position();

        if (gui_button_coordinations == 0x00) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,255));
        } else if (gui_button_coordinations == 0x01) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,255,255));
        } else if (gui_button_coordinations == 0x02) {
          m.undraw();
          return 0;
        }

      } else if (ui_button_code >= 0 && ui_button_code <= 3) {
        if (ui_button_code == 0) {
          m.move(1);
        } else if (ui_button_code == 1) {
          m.move(0);
        } else if (ui_button_code == 2) {
          m.move(2);
        } else if (ui_button_code == 3) {
          m.move(3);
        }
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
  switch (menu_index) {
    case 0:
      menu_index = game_select_menu();
      break;
    case 1:
      menu_index = tetris_menu();
      break;
    case 2:
      menu_index = minesweeper_menu();
      break;
    
    default:
      break;
  }
}

#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x23);

Menu main_menu = Menu(lcd, 0x23, lcd.rgb(0,0,0));

void load_main_menu() {
  if (1) { // setup
    main_menu.init();

    main_menu.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
    main_menu.add_button(0, 1, 70, 120, 100, 50, lcd.rgb(0,255,0), 2, lcd.rgb(255,255,255));
    main_menu.add_button(0, 2, 70, 190, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
    main_menu.add_button(1, 1, 190, 120, 20, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));

    main_menu.draw();
  }

  if (1) { // loop
    while (true) {
      uint8_t ui_button_coordinations = button_grid.scan();

      if (ui_button_coordinations == 0xFF) {
        continue;
      }

      uint16_t ui_button_code = (ui_button_coordinations >> 4) + ((ui_button_coordinations & 0x0F) * 2);
      // Serial.println(ui_button_code);

      if (ui_button_code == 4) {
        uint8_t gui_button_coordinations = main_menu.get_position();

        if ((gui_button_coordinations >> 4) == 0 && (gui_button_coordinations & 0x0F) == 0) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,0));
        } else if ((gui_button_coordinations >> 4) == 0 && (gui_button_coordinations & 0x0F) == 1) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,255,0));
        } else if ((gui_button_coordinations >> 4) == 0 && (gui_button_coordinations & 0x0F) == 2) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,0,255));
        } else if ((gui_button_coordinations >> 4) == 1 && (gui_button_coordinations & 0x0F) == 1) {
          lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,255));
        }

      } else if (ui_button_code >= 0 && ui_button_code <= 3) {
        if (ui_button_code == 0) {
          main_menu.move(1);
        } else if (ui_button_code == 1) {
          main_menu.move(0);
        } else if (ui_button_code == 2) {
          main_menu.move(3);
        } else if (ui_button_code == 3) {
          main_menu.move(2);
        }
      }
    }
  }
  
  if (1) { // end
    main_menu.~Menu();
  }
}

void setup() {
  // Serial.begin(115200);
  // Serial.println(1);

  lcd.Init();
  lcd.fill();
  
  button_grid.init();

  load_main_menu();
}

void loop() {
  
}

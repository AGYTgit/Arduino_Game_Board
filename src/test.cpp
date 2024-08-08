#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>


ST7789V lcd = ST7789V();

Menu main_menu = Menu(lcd, 3, 2, lcd.rgb(0,0,0));

Button_Grid button_grid = Button_Grid(8, 10, 2, 3);


void setup() {
  // Serial.begin(115200);

  lcd.Init();
  lcd.fill();
  
  main_menu.init();

  button_grid.init();


  main_menu.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(1, 0, 70, 120, 100, 50, lcd.rgb(0,255,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(2, 0, 70, 190, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
  main_menu.add_button(1, 1, 190, 120, 20, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));
  main_menu.add_button(1, 1, 190, 120, 20, 50, lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));

  main_menu.draw();
}

uint8_t* button_coordinations = nullptr;
uint8_t* position = nullptr;

void loop() {
  if (button_grid.scan(button_coordinations)) {
    uint8_t button_code = button_coordinations[0] * 2 + button_coordinations[1];

    if (button_code == 4) {
      main_menu.get_position(position);

      if (position[0] == 0 && position[1] == 0) {
        lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,0));
      } else if (position[0] == 1 && position[1] == 0) {
        lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,255,0));
      } else if (position[0] == 2 && position[1] == 0) {
        lcd.draw_rect(0, 0, 25, 25, lcd.rgb(0,0,255));
      } else if (position[0] == 1 && position[1] == 1) {
        lcd.draw_rect(0, 0, 25, 25, lcd.rgb(255,0,255));
      }

      delete[] position;

    } else if (button_code >= 0 && button_code <= 3) {
      main_menu.move(button_code);
    }

    delete[] button_coordinations;
  }
}
#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>
#include <ui.h>


ST7789V lcd = ST7789V();

Menu main_menu = Menu(lcd, 3, 1, lcd.rgb(0,0,0));

Button_Grid button_grid = Button_Grid(10, 12, 2, 2);


void setup() {
  Serial.begin(115200);

  lcd.Init();
  lcd.fill();
  
  main_menu.init();

  button_grid.init();


  main_menu.add_button(0, 0, 70, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(1, 0, 70, 120, 100, 50, lcd.rgb(0,255,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(2, 0, 70, 190, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));

  main_menu.draw();
}

uint8_t* button_coordinations = nullptr;

void loop() {
  if (button_grid.scan(button_coordinations)) {
    main_menu.move(button_coordinations[0] * 2 + button_coordinations[1]);
    delete[] button_coordinations;
  }
}
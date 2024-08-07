#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>


ST7789V lcd = ST7789V();

Menu main_menu = Menu(3, lcd.rgb(0,50,0));


void setup() {
  Serial.begin(115200);

  lcd.Init();
  lcd.fill();

  main_menu.add_button(10, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(130, 50, 100, 50, lcd.rgb(0,255,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(130, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
  main_menu.draw(lcd);
}

void loop() {
  delay(500);
  main_menu.highlight(lcd);
}
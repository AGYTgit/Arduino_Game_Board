#ifndef MENU_H
#define MENU_H

#include <ST7789V.h>

#include "button/button.h"

class Menu {
private:
  uint8_t button_count;
  Button* buttons;
  uint16_t bg_color;
  uint8_t highlight_index;

public:
  Menu(uint8_t max_button_count, uint16_t bg_color);
  ~Menu();

  void add_button(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color);
  void draw(ST7789V lcd);
  void highlight(ST7789V lcd, bool direction=0);
};

#endif  // MENU_H
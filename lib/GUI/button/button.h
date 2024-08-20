#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#include "ST7789V/ST7789V.h"

class Button {
private:
  ST7789V* lcd;

  uint16_t pos_x;
  uint16_t pos_y;
  uint16_t width;
  uint16_t height;

  uint16_t color;

  uint8_t border_thickness;
  uint16_t highlight_color;

public:
  Button();
  Button(ST7789V* lcd, uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height, uint16_t _color, uint8_t _border_thickness, uint16_t _highlight_color);
  void draw();
  void draw(uint16_t color);
  void highlight();
  void unhighlight();
};

#endif  // BUTTON_H
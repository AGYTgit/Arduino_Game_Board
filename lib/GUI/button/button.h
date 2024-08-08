#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <ST7789V.h>

class Button {
private:
  uint16_t pos_x;
  uint16_t pos_y;
  uint16_t width;
  uint16_t height;

  uint16_t color;

  uint8_t border_thickness;
  uint16_t highlight_color;

public:
  Button();
  Button(uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height, uint16_t _color, uint8_t _border_thickness, uint16_t _highlight_color);
  void draw(ST7789V lcd);
  void highlight(ST7789V lcd);
  void unhighlight(ST7789V lcd);
};

#endif  // BUTTON_H
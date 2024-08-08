#include "button.h"

Button::Button() {}

Button::Button(uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height, uint16_t _color, uint8_t _border_thickness, uint16_t _highlight_color) :
  pos_x(_pos_x), pos_y(_pos_y), width(_width), height(_height), color(_color), border_thickness(_border_thickness), highlight_color(_highlight_color) {}

void Button::draw(ST7789V lcd) {
  lcd.draw_rect(pos_x, pos_y, width, height, color);
}

void Button::highlight(ST7789V lcd) {
  lcd.draw_frame(pos_x, pos_y, width, height, border_thickness, highlight_color);
}

void Button::unhighlight(ST7789V lcd) {
  lcd.draw_frame(pos_x, pos_y, width, height, border_thickness, color);
}
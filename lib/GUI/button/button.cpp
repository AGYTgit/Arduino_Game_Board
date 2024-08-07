#include "button/button.h"

Button::Button() {}

Button::Button(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color) : pos_x(pos_x), pos_y(pos_y), width(width), height(height), color(color), border_thickness(border_thickness), highlight_color(highlight_color) {}

void Button::draw(ST7789V lcd) {
  lcd.draw_rect(pos_x, pos_y, width, height, color);
}

void Button::highlight(ST7789V lcd) {
  lcd.draw_frame(pos_x, pos_y, width, height, border_thickness, highlight_color);
}

void Button::unhighlight(ST7789V lcd) {
  lcd.draw_frame(pos_x, pos_y, width, height, border_thickness, color);
}
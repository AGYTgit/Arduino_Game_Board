#include "button.h"

Button::Button() : lcd(nullptr), text(nullptr) {}

Button::Button(ST7789V* _lcd, uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height, char* _text, uint16_t _color, uint8_t _border_thickness, uint16_t _highlight_color) :
  lcd(_lcd), pos_x(_pos_x), pos_y(_pos_y), width(_width), height(_height), text(_text), color(_color), border_thickness(_border_thickness), highlight_color(_highlight_color) {}

void Button::draw() {
  this->lcd->draw_rect(this->pos_x, this->pos_y, this->width, this->height, this->color);
  if (this->text != nullptr) {
    this->lcd->draw_text(this->pos_x, this->pos_y, this->width, this->height, 2, 0, this->text, this->highlight_color);
  }
}

void Button::draw(uint16_t color) {
  this->lcd->draw_rect(this->pos_x, this->pos_y, this->width, this->height, color);
  if (this->text != nullptr) {
    this->lcd->draw_text(this->pos_x, this->pos_y, this->width, this->height, 2, 0, this->text, color);
  }
}

void Button::highlight() {
  this->lcd->draw_frame(this->pos_x, this->pos_y, this->width, this->height, this->border_thickness, this->highlight_color);
}

void Button::unhighlight() {
  this->lcd->draw_frame(this->pos_x, this->pos_y, this->width, this->height, this->border_thickness, this->color);
}
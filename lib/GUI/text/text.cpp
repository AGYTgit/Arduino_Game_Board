#include "text.h"

Text::Text(const char* _text, uint16_t _pos_x, uint16_t _pos_y, uint16_t _color)
    : text(_text), pos_x(_pos_x), pos_y(_pos_y), color(_color) {}

void Text::draw(ST7789V lcd) {
    // lcd.text(this->text, this->color);
}
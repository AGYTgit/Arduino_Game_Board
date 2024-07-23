#include "button.h"

Button::Button(int8_t _input_pin, int8_t _output_pin, uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height,
    uint8_t _border_thickness, Text _text, uint16_t _main_color, uint16_t _accent_color)
        : input_pin(_input_pin), output_pin(_output_pin), pos_x(_pos_x), pos_y(_pos_y), width(_width), height(_height),
            border_thickness(_border_thickness), text(_text), main_color(_main_color), accent_color(_accent_color) {
                
            }


void Button::draw(ST7789V lcd) {
    lcd.draw_rect(this->pos_x, this->pos_y, this->width, this->height, this->main_color);
    lcd.draw_frame(this->pos_x, this->pos_y, this->width, this->height, this->border_thickness, this->accent_color);
    text.draw(lcd);
}

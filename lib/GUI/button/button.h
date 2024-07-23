#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#include <ST7789V.h>

#include "text/text.h"

class Button {
public:
    Button(int8_t _input_pin, int8_t _output_pin, uint16_t _pos_x, uint16_t _pos_y, uint16_t _width, uint16_t _height,
        uint8_t _border_thickness, Text _text, uint16_t _main_color, uint16_t _accent_color);
    
    void draw(ST7789V lcd);

    void toggle_highlight();

private:
    int8_t input_pin;
    int8_t output_pin;

    uint16_t pos_x;
    uint16_t pos_y;

    uint16_t width;
    uint16_t height;

    uint8_t border_thickness;

    Text text;

    uint16_t main_color;
    uint16_t accent_color;
};


#endif  // BUTTON_H

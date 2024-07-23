#ifndef TEXT_H
#define TEXT_H

#include <Arduino.h>

#include <ST7789V.h>

class Text {
public:
    Text(const char* _text, uint16_t _pos_x, uint16_t _pos_y, uint16_t _color);

    void draw(ST7789V lcd);

private:
    const char* text;

    uint16_t pos_x;
    uint16_t pos_y;

    uint16_t color;
};

#endif // TEXT_H
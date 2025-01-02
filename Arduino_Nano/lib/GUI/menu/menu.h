#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

#include "ST7789V/ST7789V.h"
#include "button/button.h"

class Menu {
public:
    uint8_t button_grid_dimensions;

    Button** buttons;
    bool** button_active;

    struct Text {
        uint16_t pos_x;
        uint16_t pos_y;
        uint16_t width;
        uint16_t height;
        uint8_t scale;
        int8_t spacing;
        char* text;
        uint16_t color;
    };

    Text* texts;
    uint8_t text_count;
    uint8_t max_text_count;
    
    uint16_t bg_color;

    uint8_t selected_position;

public:
    Menu(uint8_t _button_grid_dimensions, uint16_t _bg_color, uint8_t _max_text_count=0);
    ~Menu();

    void init();

    void add_button(ST7789V& lcd, uint8_t button_grid_pos_x, uint8_t button_grid_pos_y, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, char* text, uint16_t color, uint8_t border_thickness, uint16_t highlight_color);
    void add_text(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint8_t scale, int8_t spacing, const char* text, uint16_t color);
    void draw(ST7789V& lcd);
    void undraw(ST7789V& lcd);
    void move(uint8_t direction);
    uint8_t get_position();
};

#endif  // MENU_H
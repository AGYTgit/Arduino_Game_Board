#ifndef MENU_H
#define MENU_H

#include <ST7789V.h>

#include "button/button.h"

class Menu {
public:
    ST7789V& lcd;

    uint8_t button_grid_height;
    uint8_t button_grid_width;

    Button** buttons;
    bool** button_active;
    
    uint16_t bg_color;

    uint8_t highlight_index_y;
    uint8_t highlight_index_x;

public:
    Menu(ST7789V& _lcd, uint8_t _button_grid_height, uint8_t _button_grid_width, uint16_t _bg_color);
    ~Menu();

    void init();

    void add_button(uint8_t button_grid_pos_y, uint8_t button_grid_pos_x, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color);
    void draw();
    void move(uint8_t direction);
};

#endif  // MENU_H

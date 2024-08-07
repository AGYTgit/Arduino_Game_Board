#include "menu/menu.h"

Menu::Menu(uint8_t _button_grid_height, uint8_t _button_grid_width, uint16_t _bg_color) : button_grid_height(_button_grid_height), button_grid_width(_button_grid_width), bg_color(_bg_color), highlight_index_x(0), highlight_index_y(0) {
    buttons = new Button*[button_grid_height];
    button_used = new bool*[button_grid_height];
    for (uint8_t i = 0; i < button_grid_height; i++) {
        buttons[i] = new Button[button_grid_width];
        button_used[i] = new bool[button_grid_width];
        for (uint8_t j = 0; j < button_grid_width; j++) {
            button_used[i][j] = false;
        }
    }
}

Menu::~Menu() {
    delete[] buttons;
    delete[] button_used;
}

void Menu::add_button(uint8_t button_grid_pos_y, uint8_t button_grid_pos_x, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color) {
    if (button_grid_pos_y < button_grid_height && button_grid_pos_x < button_grid_width) {
        buttons[button_grid_pos_y][button_grid_pos_x] = Button(pos_x, pos_y, width, height, color, border_thickness, highlight_color);
        button_used[button_grid_pos_y][button_grid_pos_x] = true;
    }
}

void Menu::draw(ST7789V lcd) {
    lcd.fill(bg_color);
    for (uint8_t i = 0; i < button_grid_height; i++) {
        for (uint8_t j = 0; j < button_grid_width; j++) {
            buttons[i][j].draw(lcd);
        }
    }
}

void Menu::highlight(ST7789V lcd, uint8_t direction) {
    uint8_t old_highlight_index_y = highlight_index_y;
    uint8_t old_highlight_index_x = highlight_index_x;

    if (direction == 0) {
        highlight_index_y = (highlight_index_y + 1) % button_grid_height;
    } else if (direction == 1) {
        highlight_index_y = (highlight_index_y - 1 + button_grid_height) % button_grid_height;
    } else if (direction == 2) {
        highlight_index_x = (highlight_index_x + 1) % button_grid_width;
    } else if (direction == 3) {
        highlight_index_x = (highlight_index_x - 1 + button_grid_width) % button_grid_width;
    } else {
        highlight_index_x = 0;
        highlight_index_y = 0;
    }

    if (button_used[highlight_index_y][highlight_index_x]) {
        buttons[old_highlight_index_y][old_highlight_index_x].unhighlight(lcd);
        buttons[highlight_index_y][highlight_index_x].highlight(lcd);
        return;
    }

    if (direction == 0) {
        highlight_index_y = (highlight_index_y - 1 + button_grid_height) % button_grid_height;
    } else if (direction == 1) {
        highlight_index_y = (highlight_index_y + 1) % button_grid_height;
    } else if (direction == 2) {
        highlight_index_x = (highlight_index_x - 1 + button_grid_width) % button_grid_width;
    } else if (direction == 3) {
        highlight_index_x = (highlight_index_x + 1) % button_grid_width;
    } else {
        highlight_index_x = 0;
        highlight_index_y = 0;
    }
}
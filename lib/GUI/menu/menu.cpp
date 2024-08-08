#include "menu.h"

Menu::Menu(ST7789V& _lcd, uint8_t _button_grid_height, uint8_t _button_grid_width, uint16_t _bg_color) :
    lcd(_lcd), button_grid_height(_button_grid_height), button_grid_width(_button_grid_width), bg_color(_bg_color), highlight_index_y(0), highlight_index_x(0) {}

Menu::~Menu() {
    delete[] this->buttons;
    delete[] this->button_active;
}

void Menu::init() {
    this->buttons = new Button*[this->button_grid_height];
    this->button_active = new bool*[this->button_grid_height];
    for (uint8_t i = 0; i < this->button_grid_height; i++) {
        this->buttons[i] = new Button[this->button_grid_width];
        this->button_active[i] = new bool[this->button_grid_width];
        for (uint8_t j = 0; j < this->button_grid_width; j++) {
            this->button_active[i][j] = false;
        }
    }
}

void Menu::add_button(uint8_t button_grid_pos_y, uint8_t button_grid_pos_x, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color) {
    if (button_grid_pos_y < this->button_grid_height && button_grid_pos_x < this->button_grid_width) {
        this->buttons[button_grid_pos_y][button_grid_pos_x] = Button(pos_x, pos_y, width, height, color, border_thickness, highlight_color);
        this->button_active[button_grid_pos_y][button_grid_pos_x] = true;
    }
}

void Menu::draw() {
    this->lcd.fill(bg_color);
    for (uint8_t i = 0; i < this->button_grid_height; i++) {
        for (uint8_t j = 0; j < this->button_grid_width; j++) {
            this->buttons[i][j].draw(this->lcd);
        }
    }
    this->move(-1);
}

void Menu::move(uint8_t direction) {
    uint8_t old_highlight_index_y = this->highlight_index_y;
    uint8_t old_highlight_index_x = this->highlight_index_x;

    if (direction == 0) {
        this->highlight_index_y = (this->highlight_index_y + 1) % this->button_grid_height;
    } else if (direction == 1) {
        this->highlight_index_y = (this->highlight_index_y - 1 + this->button_grid_height) % this->button_grid_height;
    } else if (direction == 2) {
        this->highlight_index_x = (this->highlight_index_x + 1) % this->button_grid_width;
    } else if (direction == 3) {
        this->highlight_index_x = (this->highlight_index_x - 1 + this->button_grid_width) % this->button_grid_width;
    }

    if (this->button_active[this->highlight_index_y][this->highlight_index_x]) {
        this->buttons[old_highlight_index_y][old_highlight_index_x].unhighlight(this->lcd);
        this->buttons[this->highlight_index_y][this->highlight_index_x].highlight(this->lcd);
        return;
    }

    this->highlight_index_x = old_highlight_index_x;
    this->highlight_index_y = old_highlight_index_y;
}
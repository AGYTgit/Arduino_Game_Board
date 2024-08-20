#include "menu.h"

Menu::Menu(uint8_t _button_grid_dimensions, uint16_t _bg_color) :
    button_grid_dimensions(_button_grid_dimensions), bg_color(_bg_color), selected_position(0) {}

Menu::~Menu() {
    for (uint8_t i = 0; i < (this->button_grid_dimensions >> 4); i++) {
        delete[] this->buttons[i];
        delete[] this->button_active[i];
    }
    delete[] this->buttons;
    delete[] this->button_active;
}

void Menu::init() {
    this->buttons = new Button*[this->button_grid_dimensions >> 4];
    this->button_active = new bool*[this->button_grid_dimensions >> 4];
    for (uint8_t i = 0; i < (this->button_grid_dimensions >> 4); i++) {
        this->buttons[i] = new Button[this->button_grid_dimensions & 0x0F];
        this->button_active[i] = new bool[this->button_grid_dimensions & 0x0F];
        for (uint8_t j = 0; j < (this->button_grid_dimensions & 0x0F); j++) {
            this->button_active[i][j] = false;
        }
    }
}

void Menu::add_button(ST7789V& lcd, uint8_t button_grid_pos_x, uint8_t button_grid_pos_y, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color) {
    if (button_grid_pos_x < (this->button_grid_dimensions >> 4) && button_grid_pos_y < (this->button_grid_dimensions & 0x0F)) {
        this->buttons[button_grid_pos_x][button_grid_pos_y] = Button(&lcd, pos_x, pos_y, width, height, color, border_thickness, highlight_color);
        this->button_active[button_grid_pos_x][button_grid_pos_y] = true;
    }
}

void Menu::draw() {
    // lcd.fill(this->bg_color);
    for (uint8_t i = 0; i < (this->button_grid_dimensions >> 4); i++) {
        for (uint8_t j = 0; j < (this->button_grid_dimensions & 0x0F); j++) {
            if (this->button_active[i][j]) {
                this->buttons[i][j].draw();
            }
        }
    }
    this->move(-1);
}

void Menu::undraw() {
    for (uint8_t i = 0; i < (this->button_grid_dimensions >> 4); i++) {
        for (uint8_t j = 0; j < (this->button_grid_dimensions & 0x0F); j++) {
            if (this->button_active[i][j]) {
                this->buttons[i][j].draw(this->bg_color);
            }
        }
    }
}

void Menu::move(uint8_t direction) {
    uint8_t row = this->selected_position >> 4;
    uint8_t col = this->selected_position & 0x0F;
    uint8_t old_row = row;
    uint8_t old_col = col;

    if (direction == 0) { // Move up
        col = (col - 1 + (this->button_grid_dimensions & 0x0F)) % (this->button_grid_dimensions & 0x0F);
    } else if (direction == 1) { // Move down
        col = (col + 1) % (this->button_grid_dimensions & 0x0F);
    } else if (direction == 2) { // Move left
        row = (row - 1 + (this->button_grid_dimensions >> 4)) % (this->button_grid_dimensions >> 4);
    } else if (direction == 3) { // Move right
        row = (row + 1) % (this->button_grid_dimensions >> 4);
    }

    if (!this->button_active[row][col]) {
        this->selected_position = (old_row << 4) | old_col;
        return;
    }

    this->buttons[old_row][old_col].unhighlight();
    this->buttons[row][col].highlight();

    this->selected_position = (row << 4) | col;
}


uint8_t Menu::get_position() {
    return this->selected_position;
}

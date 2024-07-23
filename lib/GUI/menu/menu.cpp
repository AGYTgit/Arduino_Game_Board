#include "menu.h"

Menu::Menu() : buttons(nullptr), button_count(0) {}

void Menu::draw(ST7789V lcd) {
    for (int i = 0; i < this->button_count; i++) {
        this->buttons[i].draw(lcd);
    }
}

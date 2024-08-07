#include "menu/menu.h"

Menu::Menu(uint8_t max_button_count, uint16_t bg_color) : button_count(0), bg_color(bg_color), highlight_index(0) {
  buttons = new Button[max_button_count];
}

Menu::~Menu() {
  delete[] buttons;
}

void Menu::add_button(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint16_t color, uint8_t border_thickness, uint16_t highlight_color) {
  buttons[button_count++] = Button(pos_x, pos_y, width, height, color, border_thickness, highlight_color);
}

void Menu::draw(ST7789V lcd) {
  lcd.fill(bg_color);
  for (uint8_t i = 0; i < button_count; i++) {
    buttons[i].draw(lcd);
  }
}

void Menu::highlight(ST7789V lcd, bool direction) {
  buttons[highlight_index].unhighlight(lcd);
  if (!direction) {
    highlight_index = (highlight_index + 1) % button_count;
  } else {
    highlight_index = (highlight_index - 1 + button_count) % button_count;
  }
  buttons[highlight_index].highlight(lcd);
}
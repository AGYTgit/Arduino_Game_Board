#ifndef GUI_H
#define GUI_H

#include <Arduino.h>

#include <ST7789V.h>

#include "button/button.h"
#include "text/text.h"

class Menu {
public:
    Menu();

    void draw(ST7789V lcd);

private:
    Button* buttons;
    int8_t button_count;
};

#endif  // GUI_H

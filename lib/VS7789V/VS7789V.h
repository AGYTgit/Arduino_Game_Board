#ifndef ARDUINO_ST7789V_H
#define ARDUINO_ST7789V_H

#include <Arduino.h>

class arduino_ST7789V {
private:
    void setup_pins();
    void LCD_write(uint8_t d);
    void LCD_command_write(uint8_t command);
    void LCD_data_write(uint8_t data);
    void set_address(int16_t y1, int16_t y2, int16_t x1, int16_t x2);

public:
    arduino_ST7789V();
    void Init();
    void draw_pixel(int16_t x, int16_t y, uint16_t color);
    void draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);
    void draw_frame(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color);
    void fill(uint16_t color = 0);
};

#endif // ARDUINO_ST7789V_H

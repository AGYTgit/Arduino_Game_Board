#ifndef ST7789V_H
#define ST7789V_H

#include <Arduino.h>

#include "pin_magic.h"
#include "dimensions.h"

class ST7789V {
private:
    void setup_pins();

    void LCD_write(uint8_t d);
    void LCD_command_write(uint8_t command);
    void LCD_data_write(uint8_t data);
    void set_address(int16_t y1, int16_t y2, int16_t x1, int16_t x2);

public:
    void Init();
    uint16_t rgb(int r, int g, int b);

    void draw_pixel(int16_t x, int16_t y, uint16_t color);
    void draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);
    void draw_frame(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color);

    void fill(uint16_t color=0);

    void draw_char(int16_t pos_x, int16_t pos_y, uint16_t width, uint16_t height, uint8_t scale, char character, uint16_t color);
    void draw_text(int16_t pos_x, int16_t pos_y, uint16_t width, uint16_t height, uint8_t scale, int8_t spacing, char* text, uint16_t color);

};

constexpr uint8_t SBMFont8x8_size = 8;
constexpr uint8_t SBMFont8x8_length = 93;
constexpr char SBMFont8x8_characters[SBMFont8x8_length + 1] = "0123456789 abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-=[]\\;',./!@#$%^&*()_+{}|:\"<>?";
constexpr uint8_t SBMFont8x8_character_bitmap[SBMFont8x8_length][SBMFont8x8_size] = {
    {0b00111100, 0b01100110, 0b01001010, 0b01001010, 0b01010010, 0b01100110, 0b00111100, 0b00000000}, // 0
    {0b00011000, 0b00111000, 0b01111000, 0b00011000, 0b00011000, 0b00011000, 0b01111110, 0b00000000}, // 1
    {0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00110000, 0b01100110, 0b01111110, 0b00000000}, // 2
    {0b01111110, 0b00000110, 0b00001100, 0b00011100, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // 3
    {0b00001110, 0b00011110, 0b00110110, 0b01100110, 0b01111111, 0b00000110, 0b00001111, 0b00000000}, // 4
    {0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b00000110, 0b01100110, 0b01111100, 0b00000000}, // 5
    {0b00011110, 0b00110000, 0b01100000, 0b01111110, 0b01100011, 0b01100011, 0b00111110, 0b00000000}, // 6
    {0b01111111, 0b01100011, 0b00000110, 0b00001100, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // 7
    {0b00111110, 0b01100011, 0b01100011, 0b00111110, 0b01100011, 0b01100011, 0b00111110, 0b00000000}, // 8
    {0b00111110, 0b01100011, 0b01100011, 0b00111111, 0b00000011, 0b00000110, 0b00111100, 0b00000000}, // 9

    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // ' '

    {0b00000000, 0b00000000, 0b00111100, 0b00000110, 0b00111110, 0b01100110, 0b00111011, 0b00000000}, // a
    {0b01110000, 0b00110000, 0b00111110, 0b00110011, 0b00110011, 0b00110011, 0b01101110, 0b00000000}, // b
    {0b00000000, 0b00000000, 0b00011110, 0b00110011, 0b00110000, 0b00110011, 0b00011110, 0b00000000}, // c
    {0b00000111, 0b00000110, 0b00111110, 0b01100110, 0b01100110, 0b01100110, 0b00111011, 0b00000000}, // d
    {0b00000000, 0b00000000, 0b00111100, 0b01100110, 0b01111110, 0b01100000, 0b00111110, 0b00000000}, // e
    {0b00001110, 0b00011000, 0b00011000, 0b01111110, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // f
    {0b00000000, 0b00000000, 0b00111111, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b01111100}, // g
    {0b01110000, 0b00110000, 0b00111110, 0b00110011, 0b00110011, 0b00110011, 0b01110011, 0b00000000}, // h
    {0b00000000, 0b00001100, 0b00000000, 0b00011100, 0b00001100, 0b00001100, 0b00011110, 0b00000000}, // i
    {0b00000000, 0b00000110, 0b00000000, 0b00000110, 0b00000110, 0b01100110, 0b01100110, 0b00111100}, // j
    {0b01110000, 0b00110000, 0b00110011, 0b00110110, 0b00111100, 0b00110110, 0b01110011, 0b00000000}, // k
    {0b00000000, 0b00011100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00011110, 0b00000000}, // l
    {0b00000000, 0b00000000, 0b01111110, 0b01101011, 0b01101011, 0b01101011, 0b01101011, 0b00000000}, // m
    {0b00000000, 0b00000000, 0b01111110, 0b00110011, 0b00110011, 0b00110011, 0b00110011, 0b00000000}, // n
    {0b00000000, 0b00000000, 0b00011110, 0b00110011, 0b00110011, 0b00110011, 0b00011110, 0b00000000}, // o
    {0b00000000, 0b00000000, 0b01111110, 0b00110011, 0b00110011, 0b00111110, 0b00110000, 0b01111000}, // p
    {0b00000000, 0b00000000, 0b00111111, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b00001111}, // q
    {0b00000000, 0b00000000, 0b01101110, 0b00111011, 0b00110011, 0b00110000, 0b01111000, 0b00000000}, // r
    {0b00000000, 0b00000000, 0b00011111, 0b00110000, 0b00011110, 0b00000011, 0b00111110, 0b00000000}, // s
    {0b00011000, 0b00011000, 0b01111110, 0b00011000, 0b00011000, 0b00011011, 0b00001110, 0b00000000}, // t
    {0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111011, 0b00000000}, // u
    {0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00000000}, // v
    {0b00000000, 0b00000000, 0b01100011, 0b01101011, 0b01101011, 0b01111111, 0b00110110, 0b00000000}, // w
    {0b00000000, 0b00000000, 0b01100011, 0b00110110, 0b00011100, 0b00110110, 0b01100011, 0b00000000}, // x
    {0b00000000, 0b00000000, 0b01100011, 0b01100011, 0b00110110, 0b00011100, 0b00011000, 0b01110000}, // y
    {0b00000000, 0b00000000, 0b00111111, 0b00100110, 0b00001100, 0b00011001, 0b00111111, 0b00000000}, // z

    {0b00011100, 0b00110110, 0b01100011, 0b01100011, 0b01111111, 0b01100011, 0b01100011, 0b00000000}, // A
    {0b01111110, 0b00110011, 0b00110011, 0b00111110, 0b00110011, 0b00110011, 0b01111110, 0b00000000}, // B
    {0b00011110, 0b00110011, 0b01100000, 0b01100000, 0b01100000, 0b00110011, 0b00011110, 0b00000000}, // C
    {0b01111100, 0b00110110, 0b00110011, 0b00110011, 0b00110011, 0b00110110, 0b01111100, 0b00000000}, // D
    {0b01111111, 0b00110001, 0b00110100, 0b00111100, 0b00110100, 0b00110001, 0b01111111, 0b00000000}, // E
    {0b01111111, 0b00110001, 0b00110100, 0b00111100, 0b00110100, 0b00110000, 0b00110000, 0b00000000}, // F
    {0b00011110, 0b00110011, 0b01100000, 0b01100000, 0b01100111, 0b00110011, 0b00011111, 0b00000000}, // G
    {0b01100011, 0b01100011, 0b01100011, 0b01111111, 0b01100011, 0b01100011, 0b01100011, 0b00000000}, // H
    {0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // I
    {0b00011111, 0b00000110, 0b00000110, 0b00000110, 0b01100110, 0b01101110, 0b00111100, 0b00000000}, // J
    {0b01110011, 0b00110011, 0b00110110, 0b00111100, 0b00110110, 0b00110011, 0b01110011, 0b00000000}, // K
    {0b01111000, 0b00110000, 0b00110000, 0b00110000, 0b00110001, 0b00110011, 0b00111111, 0b00000000}, // L
    {0b01100011, 0b01110111, 0b01111111, 0b01101011, 0b01100011, 0b01100011, 0b01100011, 0b00000000}, // M
    {0b01100011, 0b01110011, 0b01111011, 0b01101111, 0b01100111, 0b01100011, 0b01100011, 0b00000000}, // N
    {0b00111110, 0b01100011, 0b01100011, 0b01100011, 0b01100011, 0b01100011, 0b00111110, 0b00000000}, // O
    {0b01111110, 0b00110011, 0b00110011, 0b00111110, 0b00110000, 0b00110000, 0b01111000, 0b00000000}, // P
    {0b00111110, 0b01100011, 0b01100011, 0b01100011, 0b01101011, 0b01101111, 0b00111110, 0b00000000}, // Q
    {0b01111110, 0b00110011, 0b00110011, 0b00111110, 0b00110110, 0b00110011, 0b01110011, 0b00000000}, // R
    {0b00111110, 0b01100011, 0b01100000, 0b00111110, 0b00000011, 0b01100011, 0b00111110, 0b00000000}, // S
    {0b01111110, 0b01011010, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // T
    {0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // U
    {0b01100011, 0b01100011, 0b01100011, 0b01100011, 0b00110110, 0b00111110, 0b00011100, 0b00000000}, // V
    {0b01100011, 0b01100011, 0b01101011, 0b01101011, 0b01101011, 0b01111111, 0b00110110, 0b00000000}, // W
    {0b01100011, 0b00110110, 0b00011100, 0b00011100, 0b00011100, 0b00110110, 0b01100011, 0b00000000}, // X
    {0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // Y
    {0b01111111, 0b01000011, 0b00000110, 0b00001100, 0b00011001, 0b00110011, 0b01111111, 0b00000000}, // Z

    {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // -
    {0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00111100, 0b00000000, 0b00000000, 0b00000000}, // =
    {0b00011000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011000, 0b00000000}, // [
    {0b00011000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00011000, 0b00000000}, // ]
    {0b00110000, 0b00010000, 0b00011000, 0b00001000, 0b00001100, 0b00000100, 0b00000110, 0b00000000}, // '\'
    {0b00000000, 0b00011000, 0b00011000, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00110000}, // ;
    {0b00001000, 0b00011000, 0b00011000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // '
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00110000}, // ,
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000}, // .
    {0b00000110, 0b00000100, 0b00001100, 0b00001000, 0b00011000, 0b00010000, 0b00110000, 0b00000000}, // /
    {0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00011000, 0b00000000}, // !
    {0b00111100, 0b01000010, 0b10011101, 0b10100101, 0b10100101, 0b10011010, 0b01000000, 0b00111110}, // @
    {0b00010010, 0b00010010, 0b01111111, 0b00010010, 0b00100100, 0b01111111, 0b00100100, 0b00100100}, // #
    {0b00111100, 0b01001010, 0b01001000, 0b00111100, 0b00010010, 0b01010010, 0b00111100, 0b00000000}, // $
    {0b01100010, 0b10010100, 0b10010100, 0b01111000, 0b00011110, 0b00101001, 0b00101001, 0b01000110}, // %
    {0b00011000, 0b00100100, 0b01000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // ^
    {0b00011000, 0b00100100, 0b00100100, 0b00011000, 0b00100101, 0b01000010, 0b01000110, 0b00111101}, // &
    {0b00101010, 0b00011100, 0b00011100, 0b00101010, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // *
    {0b00011000, 0b00110000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00110000, 0b00011000}, // (
    {0b00011000, 0b00001100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00001100, 0b00011000}, // )
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111110}, // _
    {0b00000000, 0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000, 0b00000000, 0b00000000}, // +
    {0b00011000, 0b00010000, 0b00010000, 0b00100000, 0b00100000, 0b00010000, 0b00010000, 0b00011000}, // {
    {0b00011000, 0b00001000, 0b00001000, 0b00000100, 0b00000100, 0b00001000, 0b00001000, 0b00011000}, // }
    {0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000}, // |
    {0b00000000, 0b00011000, 0b00011000, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000}, // :
    {0b00100100, 0b00100100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // "
    {0b00000100, 0b00011000, 0b00100000, 0b01000000, 0b01000000, 0b00100000, 0b00011000, 0b00000100}, // <
    {0b00100000, 0b00011000, 0b00000100, 0b00000010, 0b00000010, 0b00000100, 0b00011000, 0b00100000}, // >
    {0b00111000, 0b01000100, 0b00000100, 0b00000100, 0b00011000, 0b00000000, 0b00011000, 0b00011000}, // ?
};

#endif // ST7789V_H
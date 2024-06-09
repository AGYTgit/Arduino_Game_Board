#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

struct data_struct {
    uint16_t COLOR;       // r 5-bit, g 6-bit, b 5-bit
    uint8_t DIMENSIONS;   // width first 4-bits, height second 4-bits
    uint8_t SHAPE;        // top half first 4-bits, bottom half second 4-bits
};

const data_struct BLOCK_DATA[7] = { 
    {0xF800,B00010100,B11110000}, // I
    {0xE000,B00100011,B10011100}, // J
    {0xFF00,B00100011,B00111100}, // L
    {0xFFE0,B00100010,B11110000}, // O
    {0xC600,B00100011,B01111000}, // S
    {0xF81F,B00100011,B01011100}, // T
    {0xC600,B00100011,B11001100}  // Z
};

#endif // BLOCK_DATA_H
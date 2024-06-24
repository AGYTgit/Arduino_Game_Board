#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

struct Block {
    int8_t BLOCK_CODE;
    int16_t X;
    int16_t Y;
    int8_t ROTATION;
};

struct Block_data {
    uint16_t COLOR;       // r 5-bit, g 6-bit, b 5-bit
    uint8_t DIMENSIONS;   // height first 4-bits, width second 4-bits
    uint8_t SHAPE;        // top half first 4-bits, bottom half second 4-bits
};

const Block_data BLOCK_DATA[7] = { 
    {0x07FF,B00010100,B11110000}, // I
    {0x001F,B00100011,B10011100}, // J
    {0xFBE0,B00100011,B00111100}, // L
    {0xFFE0,B00100010,B11110000}, // O
    {0x07E0,B00100011,B01111000}, // S
    {0xF81F,B00100011,B01011100}, // T
    {0xF800,B00100011,B11001100}  // Z
};

#endif // BLOCK_DATA_H
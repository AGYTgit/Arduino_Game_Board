#ifndef TETRIS_DATABASE_H
#define TETRIS_DATABASE_H

struct Block {
    int8_t BLOCK_CODE;
    int16_t X;
    int16_t Y;
    int8_t ROTATION;
};

struct Block_data {
    uint16_t COLOR;       // r 5-bit, g 6-bit, b 5-bit
    uint16_t HIGHLIGHT_COLOR;
    uint8_t DIMENSIONS;   // height first 4-bits, width second 4-bits
    uint8_t SHAPE;        // top half first 4-bits, bottom half second 4-bits
    int8_t OFFSET[4][2];  // block's x/y offset for each rotation
};

const Block_data BLOCK_DATA[7] = { 
    {0x07FF, 0x0659, B00010100, B11110000, {{0, 1},{1, 0},{0, 1},{1, 0}}}, // I
    {0x001F, 0x0019, B00100011, B10011100},                                // J
    {0xFBE0, 0xCB20, B00100011, B00111100},                                // L
    {0xFFE0, 0x6320, B00100010, B11110000, {{1, 0}}},                      // O
    {0x07E0, 0x0640, B00100011, B01111000},                                // S
    {0xF81F, 0xC819, B00100011, B01011100},                                // T
    {0xF800, 0xC800, B00100011, B11001100}                                 // Z
};

// [which piece]  [rotate direction]  [current rotation]  [which offset]  [which axis]
const int8_t WKO[2][2][4][5][2] = {
    // for 0 (I) piece
    {{{{0, 0},{-2, 0},{ 1, 0},{-2, 1},{ 1,-2}},
      {{0, 0},{-1, 0},{ 2, 0},{-1,-2},{ 2, 1}},
      {{0, 0},{ 2, 0},{-1, 0},{ 2,-1},{-1, 2}},
      {{0, 0},{ 1, 0},{-2, 0},{ 1, 2},{-2,-1}}},
      
     {{{0, 0},{-1, 0},{ 2, 0},{-1,-2},{ 2, 1}},
      {{0, 0},{ 2, 0},{-1, 0},{ 2,-1},{-1, 2}},
      {{0, 0},{ 1, 0},{-2, 0},{ 1, 2},{-2 -1}},
      {{0, 0},{-2, 0},{ 1, 0},{-2, 1},{ 1,-2}}}},

    // for all other (except 3 (O)) pieces
    {{{{0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2}},
      {{0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2}},
      {{0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2}},
      {{0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2}}},

     {{{0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2}},
      {{0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2}},
      {{0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2}},
      {{0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2}}}}
};

#endif // TETRIS_DATABASE_H

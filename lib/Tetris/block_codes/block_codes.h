#ifndef BLOCK_CODES_H
#define BLOCK_CODES_H

namespace T {
    char CODE = 'T';
    uint16_t COLOR = 0xF81F;
    byte SIZE = 3;
    bool SHAPE[4][3][3] = {
        {{0,1,0},
         {1,1,1},
         {0,0,0}},

        {{0,1,0},
         {0,1,1},
         {0,1,0}},

        {{0,0,0},
         {1,1,1},
         {0,1,0}},

        {{0,1,0},
         {1,1,0},
         {0,1,0}}
    };
} // namespace T

#endif // BLOCK_CODES_H
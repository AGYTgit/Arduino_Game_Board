#ifndef TETRIS_SETTINGS_H
#define TETRIS_SETTINGS_H


namespace TETRIS_BOARD {
    const int8_t HEIGHT = 20;
    const int8_t WIDTH = 10;
    const int8_t DEPTH = 2;

    const int8_t GRID_SIZE = 16;

    const int8_t BLOCKS_AMOUNT = 7;
    const int8_t FUTURE_BLOCKS_AMOUNT = 5;

    const int8_t RANDOM_VALUE_PIN = 19;

    const int8_t DISPLAY_GRID_SIZE = 12;
} // namespace BOARD

namespace TETRIS_BLOCK_DIMENSIONS {
    const int8_t HEIGHT = 4;
    const int8_t WIDTH = 4;
} // namespace SHAPE

namespace TETRIS_DIRECTION {
    const int8_t UP = 0;
    const int8_t DOWN = 1;
    const int8_t LEFT = 2;
    const int8_t RIGHT = 3;

    const int8_t CW = 0;
    const int8_t CCW = 1;
} // namespace MOVE

#endif // TETRIS_SETTINGS_H
#ifndef BOARD_CONSTANTS_H
#define BOARD_CONSTANTS_H


namespace BOARD {
    const int8_t HEIGHT = 20;
    const int8_t WIDTH = 10;
    const int8_t DEPTH = 2;

    const int8_t GRID_SIZE = 16;

    const int8_t FUTURE_BLOCKS_AMOUNT = 5;

    const int8_t BLOCKS_AMOUNT = 7;

    const int8_t RANDOM_VALUE_PIN = 19;
} // namespace BOARD

namespace SHAPE {
    const int8_t HEIGHT = 4;
    const int8_t WIDTH = 4;
} // namespace SHAPE

namespace DIRECTION {
    const int8_t UP = 0;
    const int8_t DOWN = 1;
    const int8_t LEFT = 2;
    const int8_t RIGHT = 3;

    const int8_t CW = 0;
    const int8_t CCW = 1;
} // namespace MOVE

#endif // BOARD_CONSTANTS_H
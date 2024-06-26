#ifndef BOARD_CONSTANTS_H
#define BOARD_CONSTANTS_H


namespace BOARD {
    const byte HEIGHT = 20;
    const byte WIDTH = 10;
    const byte DEPTH = 2;

    const byte GRID_SIZE = 16;

    const byte FUTURE_BLOCKS = 5;
} // namespace BOARD

namespace SHAPE {
    const byte HEIGHT = 4;
    const byte WIDTH = 4;
} // namespace SHAPE

namespace DIRECTION {
    const byte UP = 0;
    const byte DOWN = 1;
    const byte LEFT = 2;
    const byte RIGHT = 3;

    const byte CW = 0;
    const byte CCW = 1;
} // namespace MOVE

#endif // BOARD_CONSTANTS_H
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
    const byte UP = 1;
    const byte DOWN = 2;
    const byte LEFT = 3;
    const byte RIGHT = 4;

    const byte CW = 1;
    const byte CCW = 2;
} // namespace MOVE

#endif // BOARD_CONSTANTS_H
#ifndef MINESWEEPER_SETTINGS_H
#define MINESWEEPER_SETTINGS_H


namespace MINESWEEPER_BOARD {
    const int8_t HEIGHT = 15;
    const int8_t WIDTH = 15;

    const int8_t GRID_SIZE = 16;

    const uint8_t MINE_COUNT = 50;

    const uint8_t SPAWN_AREA_HEIGHT = 3;
    const uint8_t SPAWN_AREA_WIDTH = 3;

    const uint8_t HP = 3;

    const int8_t RANDOM_VALUE_PIN = 19;
} // namespace MINESWEEPER_BOARD

namespace MINESWEEPER_DIRECTION {
    const int8_t UP = 0;
    const int8_t DOWN = 1;
    const int8_t LEFT = 2;
    const int8_t RIGHT = 3;
} // namespace MINESWEEPER_DIRECTION


#endif // MINESWEEPER_SETTINGS_H
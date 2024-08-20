#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <Arduino.h>

#include <gui.h>

#include "settings-data/minesweeper_settings.h"


class Minesweeper_Board {
public:
    Minesweeper_Board(int16_t _board_pos_x=0, int16_t _board_pos_y=0);
    ~Minesweeper_Board();

    void draw(ST7789V& lcd, bool force_draw=false);

private:
    int16_t board_pos_x;
    int16_t board_pos_y;
    // 3D array to store state of each grid block (present, future), 7th bit - revealed, 6th bit - flagged, 5th bit - exploded, bits 0-3 - 0 = blank, 1-8 = number, 9 = mine
    int8_t board_matrix[MINESWEEPER_BOARD::HEIGHT][MINESWEEPER_BOARD::WIDTH][MINESWEEPER_BOARD::DEPTH];
};

#endif // MINESWEEPER_BOARD_H
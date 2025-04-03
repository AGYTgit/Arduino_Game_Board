#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <Arduino.h>

#include <gui.h>

#include "settings-data/minesweeper_settings.h"

class Minesweeper_Board {
public:
    Minesweeper_Board(int16_t _board_pos_x=0, int16_t _board_pos_y=0);

    void draw(ST7789V& lcd, bool force_draw=false);

    void first_reveal();
    void generate_mines(uint8_t mine_count);

    void reveal();

    void move_selected_pos(uint8_t move_direction);

private:
    int16_t board_pos_x;
    int16_t board_pos_y;
    // 2D array to store state of each square (7th bit - revealed, 6th bit - flagged,
    // 5th bit - exploded, 4th bit - update, bits 0-3 - 0 = blank, 1-8 = number, 9 = mine)
    int8_t board_matrix[MINESWEEPER_BOARD::HEIGHT][MINESWEEPER_BOARD::WIDTH];

    uint8_t selected_pos_x;
    uint8_t selected_pos_y;

    uint8_t life;

    bool game_over;

    void reveal(uint8_t pos_x, uint8_t pos_y);
};

#endif // MINESWEEPER_BOARD_H
#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include <Arduino.h>
#include <EEPROM.h>

#include <gui.h>

#include "settings-data/tetris_database.h"
#include "settings-data/tetris_settings.h"

class Tetris_Board {
public:
    Tetris_Board(int16_t _board_pos_x=0, int16_t _board_pos_y=0, int16_t _display_pos_x=0, int16_t _display_pos_y=0);

    void draw(ST7789V& lcd, bool force_draw=false);

    void add_next_block();

    void add_block();
    void remove_block();
    bool check_collision();

    bool move_block(int8_t move_direction=0);
    bool rotate_block(int8_t rotate_direction=0);

    void drop();

    void hold();

    void clear_completed_lines(ST7789V& lcd);

    void update_score(ST7789V& lcd);

    void draw_score();

    int8_t get_block_y();

    void display_future_blocks(ST7789V& lcd);
    void display_hold_block(ST7789V& lcd);

private:
    uint16_t score;
    uint8_t cleared_lines;

    int16_t board_pos_x;
    int16_t board_pos_y;
    // 3D array to store state of each grid block (present, next update)
    int8_t board_matrix[TETRIS_BOARD::HEIGHT][TETRIS_BOARD::WIDTH][TETRIS_BOARD::DEPTH];

    int8_t block_codes[TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT];
    uint8_t block_code_index;
    Block block;

    int8_t held_block_code;

    int16_t display_pos_x;
    int16_t display_pos_y;

    bool block_util(int8_t update_method);

    bool try_WKO(int8_t rotate_direction);
};

#endif // TETRIS_BOARD_H

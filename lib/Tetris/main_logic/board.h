#ifndef board_h
#define board_h

#include <Arduino.h>

#include <gui.h>

#include "settings-data/block_data.h"
#include "settings-data/tetris_settings.h"

class Board {
public:
    Board(int16_t _board_pos_x=0, int16_t _board_pos_y=0, int16_t _display_pos_x=0, int16_t _display_pos_y=0);
    ~Board();

    void draw(ST7789V lcd, bool force_draw=false);

    void add_next_block();

    void add_block();
    void remove_block();
    bool check_collision();

    bool move_block(int8_t move_direction=0);
    bool rotate_block(int8_t rotate_direction=0);

    void drop();

    void hold();

    uint8_t clear_completed_lines();

    int8_t get_block_y();

    void display_future_blocks(ST7789V lcd);
    void display_hold_block(ST7789V lcd);

private:
    int16_t board_pos_x;
    int16_t board_pos_y;
    int8_t board_matrix[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH]; // 2D array to store board state

    int8_t* block_codes;
    uint8_t block_code_index;
    Block block;

    int8_t held_block_code;

    int16_t display_pos_x;
    int16_t display_pos_y;

    bool block_util(int8_t update_method);

    bool try_WKO(int8_t rotate_direction);
};

#endif // board_h

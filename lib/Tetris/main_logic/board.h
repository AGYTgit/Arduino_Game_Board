#ifndef board_h
#define board_h

#include <Arduino.h>

#include <ST7789V.h>

#include "settings-data/tetris_settings.h"
#include "settings-data/block_data.h"

class Board {
public:
    Board(int board_pos_x=0, int board_pos_y=0);
    ~Board();

    void draw(ST7789V lcd, bool force_draw=false);

    void add_next_block();

    void add_block();
    void remove_block();
    bool check_collision();

    bool move_block(int8_t move_direction=0);
    bool rotate_block(int8_t rotate_direction=0);

    void drop();

    uint8_t clear_completed_lines();

private:
    int board_pos_x;
    int board_pos_y;
    int8_t board_matrix[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH]; // 2D array to store board state

    int8_t* block_codes;
    uint8_t block_code_index;
    Block block;

    bool block_util(int8_t update_method);

    bool try_WKO(int8_t rotate_direction);
};

#endif // board_h

#ifndef board_h
#define board_h

#include <Arduino.h>

#include <ST7789V.h>

#include "settings-data/tetris_settings.h"
#include "settings-data/block_data.h"

class Board {
public:
    Board(int board_pos_x=0, int board_pos_y=0);

    void draw(ST7789V lcd, bool force_draw=false);

    void add_block(Block block);
    void remove_block(Block block);
    bool check_collision(Block& block);

    bool move_block(Block& block, int8_t move_direction=0);
    bool rotate_block(Block& block, int8_t rotate_direction=0);

    void drop(Block& block);

    uint8_t clear_completed_lines();

private:
    int board_pos_x;
    int board_pos_y;
    int8_t board_matrix[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH]; // 2D array to store board state

    bool update_block(Block block, int8_t update_method);

    bool try_WKO(Block& block, int8_t rotate_direction);
};

#endif // board_h

#ifndef board_h
#define board_h

#include <ST7789V.h>

#include <tetris_settings.h>
#include <block_data/block_data.h>

class Board {
public:
    Board(int board_pos_x=0, int board_pos_y=0);

    void draw(ST7789V lcd, bool force_draw = false);

    void add_block(Block block);
    void remove_block(Block block);

    void move_block(Block& block, int move_direction=0);
    void rotate_block(Block& block, int rotate_direction=0);

    // int clear_completed_lines();

private:
    int board_pos_x;
    int board_pos_y;
    int8_t board_matrix[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH]; // 2D array to store board state

    bool check_collision(Block& block);

    // void move_lines(int height);
};

#endif // board_h
#include "board.h"

#include <ST7789V.h>

#include <block_data/block_data.h>

Board::Board(int board_pos_x, int board_pos_y) {
    this->board_pos_x = board_pos_x;
    this->board_pos_y = board_pos_y;

    // Initialize board matrix
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            board_matrix[i][j][0] = -1;
            board_matrix[i][j][1] = 9;
        }
    }
}

void Board::draw(ST7789V lcd, bool force_draw) {
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            if (board_matrix[i][j][0] != board_matrix[i][j][1] || force_draw) {
                if (board_matrix[i][j][1] == 9) {
                    lcd.draw_rect(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, lcd.rgb(0,0,0));
                    lcd.draw_frame(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, 1, lcd.rgb(100,100,100));
                } else {
                    lcd.draw_rect(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, BLOCK_DATA[board_matrix[i][j][1]].COLOR);
                }
                board_matrix[i][j][0] = board_matrix[i][j][1];
            }
        }
    }
}

void Board::add_block(int8_t block_code, uint16_t pos_x, uint16_t pos_y, int8_t rotation) {
    int8_t bit_index = 7;
    for (int y = 0; y < (BLOCK_DATA[block_code].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[block_code].DIMENSIONS & 0x0F); x++) {
            if ((BLOCK_DATA[block_code].SHAPE >> bit_index) & 0x01) {
                if (rotation == 0) {
                    board_matrix[pos_y + y][pos_x + x][1] = block_code;
                } else if (rotation == 1) {
                    board_matrix[pos_y + x][pos_x + (BLOCK_DATA[block_code].DIMENSIONS >> 4) - y][1] = block_code; // mirrored on x
                } else if (rotation == 2) {
                    board_matrix[pos_y + (BLOCK_DATA[block_code].DIMENSIONS >> 4) - y][pos_x + (BLOCK_DATA[block_code].DIMENSIONS & 0x0F) - 1 - x][1] = block_code; // mirrored on x, y
                } else if (rotation == 3) {
                    board_matrix[pos_y + (BLOCK_DATA[block_code].DIMENSIONS & 0x0F) - 1 - x][pos_x + y][1] = block_code; // mirrored on y
                }
            }
            bit_index--;
        }
    }
}

// int Board::clear_completed_lines() {
//     int amount = 0;
//     for (int i = 0; i < BOARD::HEIGHT; i++) {
//         bool is_completed = true;
//         for (int j = 0; j < BOARD::WIDTH; j++) {
//             if (board_matrix[i][j][1] == 'B' || board_matrix[i][j][1] == 'D') {
//                 is_completed = false;
//                 break;
//             }
//         }
//         if (is_completed) {
//             for (int j = 0; j < BOARD::WIDTH; j++) {
//                 board_matrix[i][j][1] = 'B';
//             }
//             move_lines(i);
//             amount++;
//         }
//     }
//     return amount;
// }

// void Board::move_lines(int height) {
//     if (height > 0) {
//         for (int j = 0; j < BOARD::WIDTH; j++) {
//             board_matrix[height][j][1] = board_matrix[height - 1][j][1];
//         }
//         for (int j = 0; j < BOARD::WIDTH; j++) {
//             if (board_matrix[height][j][1] != 'B' && board_matrix[height][j][1] != 'D') {
//                 move_lines(height - 1);
//                 break;
//             }
//         }
//     }
// }
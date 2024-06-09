#include "board.h"

#include <ST7789V.h>

Board::Board(int board_pos_x, int board_pos_y) {
    this->board_pos_x = board_pos_x;
    this->board_pos_y = board_pos_y;

    // Initialize board matrix
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            board_matrix[i][j][0] = 'X';
            board_matrix[i][j][1] = 'B';
        }
    }
}

void Board::draw(ST7789V lcd, bool force_draw) {
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            if (board_matrix[i][j][0] != board_matrix[i][j][1] || force_draw) {
                if (board_matrix[i][j][1] == 'B') {
                    lcd.draw_rect(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, lcd.rgb(0,0,0));
                    lcd.draw_frame(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, 1, lcd.rgb(100,100,100));
                } else {
                    lcd.draw_rect(board_pos_x + j * BOARD::GRID_SIZE, board_pos_y + i * BOARD::GRID_SIZE, BOARD::GRID_SIZE, BOARD::GRID_SIZE, 0xF81F);
                }
                board_matrix[i][j][0] = board_matrix[i][j][1];
            }
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
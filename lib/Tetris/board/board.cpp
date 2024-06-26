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
            board_matrix[i][j][1] = 8;
        }
    }
}

void Board::draw(ST7789V lcd, bool force_draw) {
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            if (board_matrix[i][j][0] != board_matrix[i][j][1] || force_draw) {
                if (board_matrix[i][j][1] == 8) {
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

void Board::add_block(Block block) {
    int8_t bit_index = 7;
    for (int y = 0; y < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F); x++) {
            if (((BLOCK_DATA[block.BLOCK_CODE].SHAPE >> bit_index) & 0x01) == 0) {
                bit_index--;
                continue;
            }
            
            switch (block.ROTATION) {
                case 0:
                    board_matrix[block.Y + y][block.X + x][1] = block.BLOCK_CODE;
                    break;
                case 1:
                    board_matrix[block.Y + x][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y][1] = block.BLOCK_CODE; // mirrored on x, switched x with y
                    break;
                case 2:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x][1] = block.BLOCK_CODE; // mirrored on x, y
                    break;
                case 3:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x][block.X + y][1] = block.BLOCK_CODE; // mirrored on y, switched x with y
                    break;
                default:
                    break;
            }
            bit_index--;
        }
    }
}

void Board::remove_block(Block block) {
    int8_t bit_index = 7;
    for (int y = 0; y < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F); x++) {
            if (((BLOCK_DATA[block.BLOCK_CODE].SHAPE >> bit_index) & 0x01) == 0) {
                bit_index--;
                continue;
            }
            
            switch (block.ROTATION) {
                case 0:
                    board_matrix[block.Y + y][block.X + x][1] = 8;
                    break;
                case 1:
                    board_matrix[block.Y + x][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y][1] = 8; // mirrored on x, switched x with y
                    break;
                case 2:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x][1] = 8; // mirrored on x, y
                    break;
                case 3:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x][block.X + y][1] = 8; // mirrored on y, switched x with y
                    break;
                default:
                    break;
            }
            bit_index--;
        }
    }
}

void Board::move_block(Block& block, byte move_direction) {
    if (move_direction != DIRECTION::UP && move_direction != DIRECTION::DOWN && move_direction != DIRECTION::LEFT && move_direction != DIRECTION::RIGHT) {
        return;
    }

    this->remove_block(block);
    switch (move_direction) {
        case DIRECTION::UP:
            block.Y--;
            if (!check_collision(block)) {
                block.Y++;
            }
            break;
        case DIRECTION::DOWN:
            block.Y++;
            if (!check_collision(block)) {
                block.Y--;
            }
            break;
        case DIRECTION::LEFT:
            block.X--;
            if (!check_collision(block)) {
                block.X++;
            }
            break;
        case DIRECTION::RIGHT:
            block.X++;
            if (!check_collision(block)) {
                block.X--;
            }
            break;
        default:
            break;
    }
    this->add_block(block);
}

void Board::rotate_block(Block& block, byte rotate_direction) {
    if (rotate_direction != DIRECTION::CW && rotate_direction != DIRECTION::CCW) {
        return;
    }

    this->remove_block(block);
    try_WKO(block, rotate_direction);
    this->add_block(block);
}

bool Board::check_collision(Block& block) {
    if (block.X + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0] < 0 ||
        block.Y + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1] < 0 ||
        block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0] >= BOARD::WIDTH ||
        block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1] >= BOARD::HEIGHT) {
        return false;
    }

    int8_t bit_index = 7;
    for (int y = 0; y < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F); x++) {
            if (((BLOCK_DATA[block.BLOCK_CODE].SHAPE >> bit_index) & 0x01) == 0) {
                bit_index--;
                continue;
            }

            switch (block.ROTATION) {
                case 0:
                    if (board_matrix[block.Y + y + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1]][block.X + x + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0]][1] != 8) {
                        return false;
                    }
                    break;
                case 1:
                    if (board_matrix[block.Y + x + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1]][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0]][1] != 8) { // mirrored on x, switched x with y
                        return false;
                    }
                    break;
                case 2:
                    if (board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1]][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0]][1] != 8) { // mirrored on x, y
                        return false;
                    }
                    break;
                case 3:
                    if (board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - x + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1]][block.X + y + BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0]][1] != 8) { // mirrored on y, switched x with y
                        return false;
                    }
                    break;
                default:
                    break;
            }
            bit_index--;
        }
    }
    return true;
}

void Board::try_WKO(Block& block, byte rotate_direction) {
    byte rotate_offset = 0;

    if (rotate_direction == DIRECTION::CW) {
        rotate_offset = 1;
    } else if (rotate_direction == DIRECTION::CCW) {
        rotate_offset = -1;
    }

    bool which_block = 0;

    if (block.BLOCK_CODE == 1 || block.BLOCK_CODE == 2 || block.BLOCK_CODE == 4 || block.BLOCK_CODE == 5 || block.BLOCK_CODE == 6) {
        which_block = 1;
    } else if (block.BLOCK_CODE == 0) {
        which_block = 0;
    } else if (block.BLOCK_CODE == 3) {
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        block.X += WKO[which_block][rotate_direction][block.ROTATION][i][0];
        block.Y += WKO[which_block][rotate_direction][block.ROTATION][i][1];

        block.ROTATION = (block.ROTATION + rotate_offset + 4) % 4;

        if (check_collision(block)) {
            break;
        }

        block.ROTATION = (block.ROTATION - rotate_offset + 4) % 4;

        block.X -= WKO[which_block][rotate_direction][block.ROTATION][i][0];
        block.Y -= WKO[which_block][rotate_direction][block.ROTATION][i][1];
    }
}

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

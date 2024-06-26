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
    int8_t block_offset_x = 0;
    int8_t block_offset_y = 0;

    if (block.BLOCK_CODE == 0) {
        switch (block.ROTATION) {
            case 0:
                block_offset_y = 1;
                break;
            case 1:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_y = 1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
    } else if (block.BLOCK_CODE == 3) {
        switch (block.ROTATION) {
            case 0:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_x = 1;
                block_offset_y = -1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
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
                    board_matrix[block.Y + y + block_offset_y][block.X + x + block_offset_x][1] = block.BLOCK_CODE;
                    break;
                case 1:
                    board_matrix[block.Y + x + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_x][1] = block.BLOCK_CODE; // mirrored on x, switched x with y
                    break;
                case 2:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_x][1] = block.BLOCK_CODE; // mirrored on x, y
                    break;
                case 3:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_y][block.X + y + block_offset_x][1] = block.BLOCK_CODE; // mirrored on y, switched x with y
                    break;
                default:
                    break;
            }
            bit_index--;
        }
    }
}

void Board::remove_block(Block block) {
    int8_t block_offset_x = 0;
    int8_t block_offset_y = 0;

    if (block.BLOCK_CODE == 0) {
        switch (block.ROTATION) {
            case 0:
                block_offset_y = 1;
                break;
            case 1:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_y = 1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
    } else if (block.BLOCK_CODE == 3) {
        switch (block.ROTATION) {
            case 0:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_x = 1;
                block_offset_y = -1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
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
                    board_matrix[block.Y + y + block_offset_y][block.X + x + block_offset_x][1] = 8;
                    break;
                case 1:
                    board_matrix[block.Y + x + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_x][1] = 8; // mirrored on x, switched x with y
                    break;
                case 2:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_x][1] = 8; // mirrored on x, y
                    break;
                case 3:
                    board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_y][block.X + y + block_offset_x][1] = 8; // mirrored on y, switched x with y
                    break;
                default:
                    break;
            }
            bit_index--;
        }
    }
}

void Board::move_block(Block& block, int move_direction) {
    if (move_direction != DIRECTION::UP && move_direction != DIRECTION::DOWN && move_direction != DIRECTION::LEFT && move_direction != DIRECTION::RIGHT) {
        return;
    }

    this->remove_block(block);
    switch (move_direction) {
        case DIRECTION::UP:
            if (block.Y > 0 - (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - 1) {
                block.Y--;
            }
            if (!check_collision(block)) {
                block.Y++;
            }
            break;
        case DIRECTION::DOWN:
            if (block.Y < BOARD::HEIGHT - (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4)) {
                block.Y++;
            }
            if (!check_collision(block)) {
                block.Y--;
            }
            break;
        case DIRECTION::LEFT:
            if (block.X > 0 - (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1) {
                block.X--;
            }
            if (!check_collision(block)) {
                block.X++;
            }
            break;
        case DIRECTION::RIGHT:
            if (block.X < BOARD::WIDTH - (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F)) {
                block.X++;
            }
            if (!check_collision(block)) {
                block.X--;
            }
            break;
        default:
            break;
    }
    this->add_block(block);
}

void Board::rotate_block(Block& block, int rotate_direction) {
    if (rotate_direction != DIRECTION::CW && rotate_direction != DIRECTION::CCW) {
        return;
    }

    this->remove_block(block);
    switch (rotate_direction) {
        case DIRECTION::CW:
            block.ROTATION = (block.ROTATION + 1) % 4;
            if (!check_collision(block)) {
                block.ROTATION = (block.ROTATION - 1 + 4) % 4;
            }
            break;
        case DIRECTION::CCW:
            block.ROTATION = (block.ROTATION - 1 + 4) % 4;
            if (!check_collision(block)) {
                block.ROTATION = (block.ROTATION + 1) % 4;
            }
            break;
        default:
            break;
    }
    this->add_block(block);
}

bool Board::check_collision(Block& block) {
    int8_t block_offset_x = 0;
    int8_t block_offset_y = 0;

    if (block.BLOCK_CODE == 0) {
        switch (block.ROTATION) {
            case 0:
                block_offset_y = 1;
                break;
            case 1:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_y = 1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
    } else if (block.BLOCK_CODE == 3) {
        switch (block.ROTATION) {
            case 0:
                block_offset_x = 1;
                break;
            case 2:
                block_offset_x = 1;
                block_offset_y = -1;
                break;
            case 3:
                block_offset_x = 1;
                break;
            default:
                break;
        }
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
                    if (board_matrix[block.Y + y + block_offset_y][block.X + x + block_offset_x][1] != 8) {
                        return false;
                    }
                    break;
                case 1:
                    if (board_matrix[block.Y + x + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_x][1] != 8) { // mirrored on x, switched x with y
                        return false;
                    }
                    break;
                case 2:
                    if (board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y + block_offset_y][block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_x][1] != 8) { // mirrored on x, y
                        return false;
                    }
                    break;
                case 3:
                    if (board_matrix[block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x + block_offset_y][block.X + y + block_offset_x][1] != 8) { // mirrored on y, switched x with y
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

#include "board.h"

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
    this->update_block(block, 1); // add block
}

void Board::remove_block(Block block) {
    this->update_block(block, 0); // remove block
}

bool Board::check_collision(Block& block) {
    return this->update_block(block, 2); // check for collisions, if there is return false if not return true
}

bool Board::update_block(Block block, int8_t update_method) {
    int8_t board_y = 0;
    int8_t board_x = 0;

    int8_t bit_index = sizeof(BLOCK_DATA) / sizeof(BLOCK_DATA[0]) + 1;
    for (int y = 0; y < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F); x++) {
            if (((BLOCK_DATA[block.BLOCK_CODE].SHAPE >> --bit_index) & 0x01) == 0) {
                continue;
            }
            
            switch (block.ROTATION) {
                case 0:
                    board_y = block.Y + y;
                    board_x = block.X + x;
                    break;
                case 1: // mirrored on x, switched x with y
                    board_y = block.Y + x;
                    board_x = block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y;
                    break;
                case 2: // mirrored on x, y
                    board_y = block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS >> 4) - y;
                    board_x = block.X + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x;
                    break;
                case 3: // mirrored on y, switched x with y
                    board_y = block.Y + (BLOCK_DATA[block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x;
                    board_x = block.X + y ;
                    break;
                default:
                    break;
            }

            if (block.BLOCK_CODE == 0 || block.BLOCK_CODE == 3) {
                board_y += BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][1];
                board_x += BLOCK_DATA[block.BLOCK_CODE].OFFSET[block.ROTATION][0];
            }

            switch (update_method) {
                case 0:
                    board_matrix[board_y][board_x][1] = 8;
                    break;
                case 1:
                    board_matrix[board_y][board_x][1] = block.BLOCK_CODE;
                    break;
                case 2:
                    if (board_y < 0 || board_y >= BOARD::HEIGHT || board_x < 0 || board_x >= BOARD::WIDTH) {
                        return false;
                    } else if (board_matrix[board_y][board_x][1] != 8) {
                        return false;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (update_method == 2) {
        return true;
    }
    return false;
}


bool Board::move_block(Block& block, int8_t move_direction) {
    if (move_direction != DIRECTION::UP && move_direction != DIRECTION::DOWN && move_direction != DIRECTION::LEFT && move_direction != DIRECTION::RIGHT) {
        return false;
    }

    bool state = true;

    this->remove_block(block);
    switch (move_direction) {
        case DIRECTION::UP:
            block.Y--;
            if (!check_collision(block)) {
                block.Y++;
                state = false;
            }
            break;
        case DIRECTION::DOWN:
            block.Y++;
            if (!check_collision(block)) {
                block.Y--;
                state = false;
            }
            break;
        case DIRECTION::LEFT:
            block.X--;
            if (!check_collision(block)) {
                block.X++;
                state = false;
            }
            break;
        case DIRECTION::RIGHT:
            block.X++;
            if (!check_collision(block)) {
                block.X--;
                state = false;
            }
            break;
    }
    this->add_block(block);

    return state;
}

bool Board::rotate_block(Block& block, int8_t rotate_direction) {
    if (rotate_direction != DIRECTION::CW && rotate_direction != DIRECTION::CCW) {
        return false;
    }

    this->remove_block(block);
    bool state = try_WKO(block, rotate_direction);
    this->add_block(block);

    return state;
}

void Board::drop(Block& block) {
    if (this->move_block(block, DIRECTION::DOWN)) {
        this->drop(block);
    }
}


bool Board::try_WKO(Block& block, int8_t rotate_direction) {
    int8_t rotate_offset = 0;
    switch (rotate_direction) {
        case DIRECTION::CW:
            rotate_offset = 1;
            break;
        case DIRECTION::CCW:
            rotate_offset = -1;
            break;
}

    bool which_block = 0;
    switch (block.BLOCK_CODE) {
        case 6:
            which_block = 1;
            break;
        case 0:
            which_block = 0;
            break;
        case 3:
            return false;
    }

    for (int i = 0; i < 5; i++) {
        block.X += WKO[which_block][rotate_direction][block.ROTATION][i][0];
        block.Y += WKO[which_block][rotate_direction][block.ROTATION][i][1];

        block.ROTATION = (block.ROTATION + rotate_offset) & 3;

        if (check_collision(block)) {
            return true;
        }

        block.ROTATION = (block.ROTATION - rotate_offset) & 3;

        block.X -= WKO[which_block][rotate_direction][block.ROTATION][i][0];
        block.Y -= WKO[which_block][rotate_direction][block.ROTATION][i][1];
    }

    return false;
}


int Board::clear_completed_lines() {
    int cleared_lines = 0;
    
    for (int y = BOARD::HEIGHT - 1; y >= 0; y--) {
        bool completed = true;

        for (int x = 0; x < BOARD::WIDTH; x++) {
            if (this->board_matrix[y][x][1] == 8) {
                completed = false;
                break;
            }
        }

        if (!completed) {
            continue;
        }

        for (int y2 = y; y2 > 0; y2--) {
            for (int x2 = 0; x2 < BOARD::WIDTH; x2++) {
                this->board_matrix[y2][x2][1] = this->board_matrix[y2 - 1][x2][1];
            }
        }

        for (int x3 = 0; x3 < BOARD::WIDTH; x3++) {
            this->board_matrix[0][x3][1] = 8;
        }

        cleared_lines++;
        y++;
    }
    return cleared_lines;
}

#include "board.h"

Board::Board(int16_t _board_pos_x, int16_t _board_pos_y, int16_t _display_pos_x, int16_t _display_pos_y) :
    board_pos_x(_board_pos_x), board_pos_y(_board_pos_y), display_pos_x(_display_pos_x), display_pos_y(_display_pos_y) {

    // Initialize board matrix
    for (int i = 0; i < BOARD::HEIGHT; i++) {
        for (int j = 0; j < BOARD::WIDTH; j++) {
            board_matrix[i][j][0] = -1;
            board_matrix[i][j][1] = 8;
        }
    }

    this->block_codes = new int8_t[BOARD::FUTURE_BLOCKS_AMOUNT];
    this->block_code_index = 0;

    for (int8_t i = 0; i < BOARD::FUTURE_BLOCKS_AMOUNT; i++) {
        this->block_codes[i] = analogRead(BOARD::RANDOM_VALUE_PIN) % BOARD::BLOCKS_AMOUNT;
    }
}

Board::~Board() {
    delete[] this->block_codes;
}


void Board::draw(ST7789V lcd, bool force_draw) {
    for (uint8_t i = 0; i < BOARD::HEIGHT; i++) {
        for (uint8_t j = 0; j < BOARD::WIDTH; j++) {
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


void Board::add_next_block() {
    this->block = {this->block_codes[this->block_code_index], (int16_t)(floor((BOARD::WIDTH - (BLOCK_DATA->DIMENSIONS >> 4)) / 2) - 1), 0, 0};
    this->block_codes[this->block_code_index] = analogRead(BOARD::RANDOM_VALUE_PIN) % BOARD::BLOCKS_AMOUNT;
    this->block_code_index = (this->block_code_index + 1) % BOARD::FUTURE_BLOCKS_AMOUNT;

    this->block_util(1); // add block
}

void Board::add_block() {
    this->block_util(1); // add block
}

void Board::remove_block() {
    this->block_util(0); // remove block
}

bool Board::check_collision() {
    return this->block_util(2); // check for collisions, if there is return false if not return true
}

bool Board::block_util(int8_t update_method) {
    int8_t board_y = 0;
    int8_t board_x = 0;

    int8_t bit_index = BOARD::BLOCKS_AMOUNT + 1;
    for (int y = 0; y < (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS >> 4); y++) {
        for (int x = 0; x < (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS & 0x0F); x++) {
            if (((BLOCK_DATA[this->block.BLOCK_CODE].SHAPE >> --bit_index) & 0x01) == 0) {
                continue;
            }
            
            switch (this->block.ROTATION) {
                case 0:
                    board_y = this->block.Y + y;
                    board_x = this->block.X + x;
                    break;
                case 1: // mirrored on x, switched x with y
                    board_y = this->block.Y + x;
                    board_x = this->block.X + (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS >> 4) - y;
                    break;
                case 2: // mirrored on x, y
                    board_y = this->block.Y + (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS >> 4) - y;
                    board_x = this->block.X + (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x;
                    break;
                case 3: // mirrored on y, switched x with y
                    board_y = this->block.Y + (BLOCK_DATA[this->block.BLOCK_CODE].DIMENSIONS & 0x0F) - 1 - x;
                    board_x = this->block.X + y ;
                    break;
                default:
                    break;
            }

            if (this->block.BLOCK_CODE == 0 || this->block.BLOCK_CODE == 3) {
                board_y += BLOCK_DATA[this->block.BLOCK_CODE].OFFSET[this->block.ROTATION][1];
                board_x += BLOCK_DATA[this->block.BLOCK_CODE].OFFSET[this->block.ROTATION][0];
            }

            switch (update_method) {
                case 0:
                    board_matrix[board_y][board_x][1] = 8;
                    break;
                case 1:
                    board_matrix[board_y][board_x][1] = this->block.BLOCK_CODE;
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


bool Board::move_block(int8_t move_direction) {
    if (move_direction != DIRECTION::UP && move_direction != DIRECTION::DOWN && move_direction != DIRECTION::LEFT && move_direction != DIRECTION::RIGHT) {
        return false;
    }

    bool state = true;

    this->remove_block();
    switch (move_direction) {
        case DIRECTION::UP:
            this->block.Y--;
            if (!check_collision()) {
                this->block.Y++;
                state = false;
            }
            break;
        case DIRECTION::DOWN:
            this->block.Y++;
            if (!check_collision()) {
                this->block.Y--;
                state = false;
            }
            break;
        case DIRECTION::LEFT:
            this->block.X--;
            if (!check_collision()) {
                this->block.X++;
                state = false;
            }
            break;
        case DIRECTION::RIGHT:
            this->block.X++;
            if (!check_collision()) {
                this->block.X--;
                state = false;
            }
            break;
    }
    this->add_block();

    return state;
}

bool Board::rotate_block(int8_t rotate_direction) {
    if (rotate_direction != DIRECTION::CW && rotate_direction != DIRECTION::CCW) {
        return false;
    }

    this->remove_block();
    bool state = try_WKO(rotate_direction);
    this->add_block();
    return state;
}

bool Board::try_WKO(int8_t rotate_direction) {
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
    switch (this->block.BLOCK_CODE) {
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
        this->block.X += WKO[which_block][rotate_direction][this->block.ROTATION][i][0];
        this->block.Y += WKO[which_block][rotate_direction][this->block.ROTATION][i][1];

        this->block.ROTATION = (this->block.ROTATION + rotate_offset) & 3;

        if (check_collision()) {
            return true;
        }

        this->block.ROTATION = (this->block.ROTATION - rotate_offset) & 3;

        this->block.X -= WKO[which_block][rotate_direction][this->block.ROTATION][i][0];
        this->block.Y -= WKO[which_block][rotate_direction][this->block.ROTATION][i][1];
    }

    return false;
}


void Board::drop() {
        if (this->move_block(DIRECTION::DOWN)) {
        this->drop();
    }
}

uint8_t Board::clear_completed_lines() {
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

int8_t Board::get_block_y() {
    return this->block.Y;
}


void Board::draw_display(ST7789V lcd) {
    lcd.draw_rect(this->display_pos_x, this->display_pos_y, BOARD::DISPLAY_GRID_SIZE * 4, BOARD::DISPLAY_GRID_SIZE * 4 * BOARD::FUTURE_BLOCKS_AMOUNT, lcd.rgb(50,50,50));

    for (int8_t i = 0; i < BOARD::FUTURE_BLOCKS_AMOUNT; i++) {
        int8_t bit_index = BOARD::BLOCKS_AMOUNT + 1;
        int8_t block_code = this->block_codes[(i + this->block_code_index) % BOARD::FUTURE_BLOCKS_AMOUNT];
        for (int y = 0; y < (BLOCK_DATA[block_code].DIMENSIONS >> 4); y++) {
            for (int x = 0; x < (BLOCK_DATA[block_code].DIMENSIONS & 0x0F); x++) {
                if (((BLOCK_DATA[block_code].SHAPE >> --bit_index) & 0x01) == 0) {
                    continue;
                }

                lcd.draw_rect(this->display_pos_x + (BOARD::DISPLAY_GRID_SIZE * x), this->display_pos_y + (BOARD::DISPLAY_GRID_SIZE * y) + (BOARD::DISPLAY_GRID_SIZE * i * (BOARD::FUTURE_BLOCKS_AMOUNT - 1)), BOARD::DISPLAY_GRID_SIZE, BOARD::DISPLAY_GRID_SIZE, BLOCK_DATA[block_code].COLOR);
            }
        }
    }
}

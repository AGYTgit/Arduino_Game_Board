#include "tetris_board.h"

Tetris_Board::Tetris_Board(int16_t _board_pos_x, int16_t _board_pos_y, int16_t _display_pos_x, int16_t _display_pos_y) :
    board_pos_x(_board_pos_x), board_pos_y(_board_pos_y), display_pos_x(_display_pos_x), display_pos_y(_display_pos_y) {

    // Initialize board matrix
    for (int i = 0; i < TETRIS_BOARD::HEIGHT; i++) {
        for (int j = 0; j < TETRIS_BOARD::WIDTH; j++) {
            board_matrix[i][j][0] = -1;
            board_matrix[i][j][1] = 8;
        }
    }

    this->block_code_index = 0;

    randomSeed(analogRead(TETRIS_BOARD::RANDOM_VALUE_PIN));
    for (int8_t i = 0; i < TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT; i++) {
        this->block_codes[i] = random(0, TETRIS_BOARD::BLOCKS_AMOUNT);
    }

    this->held_block_code = -1;

    this->score = 0;
}

void Tetris_Board::draw(ST7789V& lcd, bool force_draw) {
    for (uint8_t i = 0; i < TETRIS_BOARD::HEIGHT; i++) {
        for (uint8_t j = 0; j < TETRIS_BOARD::WIDTH; j++) {
            if (board_matrix[i][j][0] != board_matrix[i][j][1] || force_draw) {
                if (board_matrix[i][j][1] == 8) {
                    lcd.draw_rect(board_pos_x + j * TETRIS_BOARD::GRID_SIZE, board_pos_y + i * TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, lcd.rgb(0,0,0));
                    lcd.draw_frame(board_pos_x + j * TETRIS_BOARD::GRID_SIZE, board_pos_y + i * TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, 1, lcd.rgb(100,100,100));
                } else {
                    lcd.draw_rect(board_pos_x + j * TETRIS_BOARD::GRID_SIZE, board_pos_y + i * TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, BLOCK_DATA[board_matrix[i][j][1]].COLOR);
                    lcd.draw_frame(board_pos_x + j * TETRIS_BOARD::GRID_SIZE, board_pos_y + i * TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, TETRIS_BOARD::GRID_SIZE, 1, BLOCK_DATA[board_matrix[i][j][1]].HIGHLIGHT_COLOR);
                }
                board_matrix[i][j][0] = board_matrix[i][j][1];
            }
        }
    }
}


void Tetris_Board::add_next_block() {
    this->block = {this->block_codes[this->block_code_index], (int16_t)(floor((TETRIS_BOARD::WIDTH - (BLOCK_DATA->DIMENSIONS >> 4)) / 2) - 1), 0, 0};
    this->block_codes[this->block_code_index] = analogRead(TETRIS_BOARD::RANDOM_VALUE_PIN) % TETRIS_BOARD::BLOCKS_AMOUNT;
    this->block_code_index = (this->block_code_index + 1) % TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT;

    this->block_util(1); // add block
}

void Tetris_Board::add_block() {
    this->block_util(1); // add block
}

void Tetris_Board::remove_block() {
    this->block_util(0); // remove block
}

bool Tetris_Board::check_collision() {
    return this->block_util(2); // check for collisions, if there is return false if not return true
}

bool Tetris_Board::block_util(int8_t update_method) {
    int8_t board_y = 0;
    int8_t board_x = 0;

    int8_t bit_index = TETRIS_BOARD::BLOCKS_AMOUNT + 1;
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
                    if (board_y < 0 || board_y >= TETRIS_BOARD::HEIGHT || board_x < 0 || board_x >= TETRIS_BOARD::WIDTH) {
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


bool Tetris_Board::move_block(int8_t move_direction) {
    if (move_direction != TETRIS_DIRECTION::UP && move_direction != TETRIS_DIRECTION::DOWN && move_direction != TETRIS_DIRECTION::LEFT && move_direction != TETRIS_DIRECTION::RIGHT) {
        return false;
    }

    bool state = true;

    this->remove_block();
    switch (move_direction) {
        case TETRIS_DIRECTION::UP:
            this->block.Y--;
            if (!check_collision()) {
                this->block.Y++;
                state = false;
            }
            break;
        case TETRIS_DIRECTION::DOWN:
            this->block.Y++;
            if (!check_collision()) {
                this->block.Y--;
                state = false;
            }
            break;
        case TETRIS_DIRECTION::LEFT:
            this->block.X--;
            if (!check_collision()) {
                this->block.X++;
                state = false;
            }
            break;
        case TETRIS_DIRECTION::RIGHT:
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

bool Tetris_Board::rotate_block(int8_t rotate_direction) {
    if (rotate_direction != TETRIS_DIRECTION::CW && rotate_direction != TETRIS_DIRECTION::CCW) {
        return false;
    }

    this->remove_block();
    bool state = try_WKO(rotate_direction);
    this->add_block();
    return state;
}

bool Tetris_Board::try_WKO(int8_t rotate_direction) {
    int8_t rotate_offset = 0;
    switch (rotate_direction) {
        case TETRIS_DIRECTION::CW:
            rotate_offset = 1;
            break;
        case TETRIS_DIRECTION::CCW:
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


void Tetris_Board::drop() {
        if (this->move_block(TETRIS_DIRECTION::DOWN)) {
        this->drop();
    }
}

void Tetris_Board::hold() {
    if (this->held_block_code == -1) {
        this->remove_block();
        this->held_block_code = this->block.BLOCK_CODE;
        this->add_next_block();
    } else {
        this->remove_block();
        int8_t temp = this->block.BLOCK_CODE;
        this->block = {this->held_block_code, (int16_t)(floor((TETRIS_BOARD::WIDTH - (BLOCK_DATA->DIMENSIONS >> 4)) / 2) - 1), 0, 0};
        this->held_block_code = temp;
        this->add_block();
    }
}

void Tetris_Board::clear_completed_lines(ST7789V& lcd) {
    int cleared_lines = 0;
    
    for (int y = TETRIS_BOARD::HEIGHT - 1; y >= 0; y--) {
        bool completed = true;

        for (int x = 0; x < TETRIS_BOARD::WIDTH; x++) {
            if (this->board_matrix[y][x][1] == 8) {
                completed = false;
                break;
            }
        }

        if (!completed) {
            continue;
        }

        for (int y2 = y; y2 > 0; y2--) {
            for (int x2 = 0; x2 < TETRIS_BOARD::WIDTH; x2++) {
                this->board_matrix[y2][x2][1] = this->board_matrix[y2 - 1][x2][1];
            }
        }

        for (int x3 = 0; x3 < TETRIS_BOARD::WIDTH; x3++) {
            this->board_matrix[0][x3][1] = 8;
        }

        cleared_lines++;
        y++;
    }

    this->cleared_lines = cleared_lines;
}

int8_t Tetris_Board::get_block_y() {
    return this->block.Y;
}


void Tetris_Board::update_score(ST7789V& lcd) {
    if (this->cleared_lines == 1) {
        this->score += 1;
    } else if (this->cleared_lines == 2) {
        this->score += 3;
    } else if (this->cleared_lines == 3) {
        this->score += 6;
    } else if (this->cleared_lines == 4) {
        this->score += 10;
    }
    this->cleared_lines = 0;

    Serial.println(this->score);
    lcd.draw_rect(180, 0, 60, 20, lcd.rgb(0,0,0));
    char buffer[6];
    sprintf(buffer, "%u", this->score);
    lcd.draw_text(180, 0, 60, 20, 2, 2, buffer, lcd.rgb(255,255,255));
}

void Tetris_Board::save_score() {
    uint16_t sc[5];

    for (uint8_t i = 0; i < 5; i++) {
        sc[i] = EEPROM.get(i * 2, sc[i]);
    }

    for (uint8_t i = 0; i < 5; i++) {
        if (this->score > sc[i]) {
            for (uint8_t j = 4; j > i; j--) {
                EEPROM.put(j * 2, sc[j - 1]);
            }
            EEPROM.put(i * 2, this->score);
            break;
        }
    }
}

void Tetris_Board::display_future_blocks(ST7789V& lcd) {
    lcd.draw_frame(this->display_pos_x, this->display_pos_y, TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5, TETRIS_BOARD::DISPLAY_GRID_SIZE * 2.5 * TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT, 2, lcd.rgb(75,75,75));
    lcd.draw_rect(this->display_pos_x + 2, this->display_pos_y + 2, TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5 - 4, TETRIS_BOARD::DISPLAY_GRID_SIZE * 2.5 * TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT - 4, lcd.rgb(50,50,50));

    for (int8_t i = 0; i < TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT; i++) {
        int8_t block_code = this->block_codes[(i + this->block_code_index) % TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT];

        uint8_t block_width = BLOCK_DATA[block_code].DIMENSIONS & 0x0F;
        uint8_t block_height = BLOCK_DATA[block_code].DIMENSIONS >> 4;

        int8_t I_offset = 0;
        if (block_code == 0) {
            I_offset = 6; // hard coded value
        }

        int8_t bit_index = TETRIS_BOARD::BLOCKS_AMOUNT + 1;
        for (int y = 0; y < block_height; y++) {
            for (int x = 0; x < block_width; x++) {
                if (((BLOCK_DATA[block_code].SHAPE >> --bit_index) & 0x01) == 0) {
                    continue;
                }

                uint16_t pos_x = this->display_pos_x + (TETRIS_BOARD::DISPLAY_GRID_SIZE * x) + (TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5 - TETRIS_BOARD::DISPLAY_GRID_SIZE * block_width) / 2;
                uint16_t pos_y = this->display_pos_y + (TETRIS_BOARD::DISPLAY_GRID_SIZE * y) + (TETRIS_BOARD::DISPLAY_GRID_SIZE * i * 2.5) + TETRIS_BOARD::DISPLAY_GRID_SIZE / 4 + I_offset;

                lcd.draw_rect(pos_x, pos_y, TETRIS_BOARD::DISPLAY_GRID_SIZE, TETRIS_BOARD::DISPLAY_GRID_SIZE, BLOCK_DATA[block_code].COLOR);
                lcd.draw_frame(pos_x, pos_y, TETRIS_BOARD::DISPLAY_GRID_SIZE, TETRIS_BOARD::DISPLAY_GRID_SIZE, 1, BLOCK_DATA[block_code].HIGHLIGHT_COLOR);
            }
        }
    }
}

void Tetris_Board::display_hold_block(ST7789V& lcd) {
    uint16_t y_offset = this->display_pos_y + TETRIS_BOARD::DISPLAY_GRID_SIZE * 2.5 * TETRIS_BOARD::FUTURE_BLOCKS_AMOUNT + 4;
    lcd.draw_frame(this->display_pos_x, y_offset, TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5, TETRIS_BOARD::DISPLAY_GRID_SIZE * 2.5, 2, lcd.rgb(75,75,75));
    lcd.draw_rect(this->display_pos_x + 2, y_offset + 2, TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5 - 4, TETRIS_BOARD::DISPLAY_GRID_SIZE * 2.5 - 4, lcd.rgb(50,50,50));
    if (this->held_block_code == -1) {
        return;
    }

    uint8_t block_width = BLOCK_DATA[this->held_block_code].DIMENSIONS & 0x0F;
    uint8_t block_height = BLOCK_DATA[this->held_block_code].DIMENSIONS >> 4;

    int8_t I_offset = 0;
    if (this->held_block_code == 0) {
        I_offset = 6; // hard coded value
    }

    int8_t bit_index = TETRIS_BOARD::BLOCKS_AMOUNT + 1;
    for (int y = 0; y < block_height; y++) {
        for (int x = 0; x < block_width; x++) {
            if (((BLOCK_DATA[this->held_block_code].SHAPE >> --bit_index) & 0x01) == 0) {
                continue;
            }

            uint16_t pos_x = this->display_pos_x + (TETRIS_BOARD::DISPLAY_GRID_SIZE * x) + (TETRIS_BOARD::DISPLAY_GRID_SIZE * 4.5 - TETRIS_BOARD::DISPLAY_GRID_SIZE * block_width) / 2;
            uint16_t pos_y = y_offset + (TETRIS_BOARD::DISPLAY_GRID_SIZE * y) + TETRIS_BOARD::DISPLAY_GRID_SIZE / 4 + I_offset;

            lcd.draw_rect(pos_x, pos_y, TETRIS_BOARD::DISPLAY_GRID_SIZE, TETRIS_BOARD::DISPLAY_GRID_SIZE, BLOCK_DATA[this->held_block_code].COLOR);
            lcd.draw_frame(pos_x, pos_y, TETRIS_BOARD::DISPLAY_GRID_SIZE, TETRIS_BOARD::DISPLAY_GRID_SIZE, 1, BLOCK_DATA[this->held_block_code].HIGHLIGHT_COLOR);
        }
    }
}

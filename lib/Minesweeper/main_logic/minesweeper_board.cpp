#include "minesweeper_board.h"

Minesweeper_Board::Minesweeper_Board(int16_t _board_pos_x, int16_t _board_pos_y) :
    board_pos_x(_board_pos_x), board_pos_y(_board_pos_y), life(MINESWEEPER_BOARD::HP), game_over(false) {

    // Initialize board matrix
    for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
        for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {
            this->board_matrix[y][x] = 0b00010000;
        }
    }

    selected_pos_x = MINESWEEPER_BOARD::WIDTH / 2;
    selected_pos_y = MINESWEEPER_BOARD::HEIGHT / 2;
}


void Minesweeper_Board::draw(ST7789V& lcd, bool force_draw) {
    for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
        for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {

            // bypass update with force_draw
            if (!force_draw) {
                // skip if update is false
                if ((((board_matrix[y][x] >> 4) & 1) == 0)) {
                    continue;
                }
            }

            uint8_t pos_x = board_pos_x + x * MINESWEEPER_BOARD::GRID_SIZE;
            uint8_t pos_y = board_pos_y + y * MINESWEEPER_BOARD::GRID_SIZE;
            uint8_t grid_size = MINESWEEPER_BOARD::GRID_SIZE;

            if (((board_matrix[y][x] >> 7) & 1) == 1) { // draw revealed squares
                if ((board_matrix[y][x] & 0b1111) == 0) { // draw blank
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(100,100,100));
                    lcd.draw_frame(pos_x, pos_y, grid_size, grid_size, 1, lcd.rgb(75,75,75));
                } else if ((board_matrix[y][x] & 0b1111) == 0b1001) { // draw mine
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(0,0,0));
                } else { // draw number
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(100,100,100));
                    lcd.draw_frame(pos_x, pos_y, grid_size, grid_size, 1, lcd.rgb(75,0,75));
                    lcd.draw_char(pos_x, pos_y, grid_size, grid_size, 1, lcd.SBMFont8x8_characters[board_matrix[y][x] & 0b1111], lcd.rgb(0,150,255)); // sometimes crashes arduino
                }
            } else if (((board_matrix[y][x] >> 7) & 1) == 0) { // draw unrevealed squares
                if (((board_matrix[y][x] >> 5) & 1) == 0) { // draw unrevealed/unflagged/unexploded
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(150,150,150));
                } else if (((board_matrix[y][x] >> 6) & 1) == 1) { // draw flagged squares
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(255,0,0));
                } else if (((board_matrix[y][x] >> 5) & 1) == 1) { // draw exploded squares
                    lcd.draw_rect(pos_x, pos_y, grid_size, grid_size, lcd.rgb(255,0,255));
                }
            }


            board_matrix[y][x] &= 0b11101111;
        }
    }

    // highlight new selected square
    lcd.draw_frame(board_pos_x + this->selected_pos_x * MINESWEEPER_BOARD::GRID_SIZE, board_pos_y + this->selected_pos_y * MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, 1, lcd.rgb(255,0,255));

}


void Minesweeper_Board::first_reveal() {

    // generate mines
    generate_mines(MINESWEEPER_BOARD::MINE_COUNT);

    // generate number/blank
    for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
        for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {

            // skip mines
            if ((this->board_matrix[y][x] & 0b1111) == 0b1001) {
                continue;
            }

            uint8_t number = 0;
            
            for (int8_t y2 = y - 1; y2 <= (y + 1); y2++) {
                for (int8_t x2 = x - 1; x2 <= (x + 1); x2++) {

                    // skip out of bounds squares
                    if (y2 < 0 || y2 > (MINESWEEPER_BOARD::HEIGHT - 1) || x2 < 0 || x2 > (MINESWEEPER_BOARD::WIDTH - 1)) {
                        continue;
                    }

                    // add found mine to count
                    if ((this->board_matrix[y2][x2] & 0b1111) == 0b1001) {
                        number++;
                    }
                }
            }

            this->board_matrix[y][x] |= number;
        }
    }

    // start reveal from selected_pos
    this->reveal();
}

void Minesweeper_Board::generate_mines(uint8_t mine_count) {
    int8_t spawn_y_l;
    int8_t spawn_y_r;
    if ((MINESWEEPER_BOARD::SPAWN_AREA_HEIGHT % 2) == 1) {
        spawn_y_l = (MINESWEEPER_BOARD::SPAWN_AREA_HEIGHT - 1) / 2;
        spawn_y_r = spawn_y_l;
    } else {
        spawn_y_l = (MINESWEEPER_BOARD::SPAWN_AREA_HEIGHT) / 2;
        spawn_y_r = spawn_y_l - 1;
    }

    int8_t spawn_x_l;
    int8_t spawn_x_r;
    if ((MINESWEEPER_BOARD::SPAWN_AREA_WIDTH % 2) == 1) {
        spawn_x_l = (MINESWEEPER_BOARD::SPAWN_AREA_WIDTH - 1) / 2;
        spawn_x_r = spawn_x_l;
    } else {
        spawn_x_l = (MINESWEEPER_BOARD::SPAWN_AREA_WIDTH) / 2;
        spawn_x_r = spawn_x_l - 1;
    }

    // generate mines
    uint8_t current_mine_count = mine_count;
    for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
        for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {

            // break if mine_count limit is reached
            if (current_mine_count <= 0) {
                return;
            }

            // skip spawn area
            if ((y + spawn_y_l >= selected_pos_y && y - spawn_y_r <= selected_pos_y) && (x + spawn_x_l >= selected_pos_x && x - spawn_x_r <= selected_pos_x)) {
                continue;
            }

            // skip non-mine squares
            randomSeed(analogRead(MINESWEEPER_BOARD::RANDOM_VALUE_PIN));
            if (!(((random(MINESWEEPER_BOARD::HEIGHT * MINESWEEPER_BOARD::WIDTH - MINESWEEPER_BOARD::SPAWN_AREA_HEIGHT * MINESWEEPER_BOARD::SPAWN_AREA_WIDTH) - 1) < MINESWEEPER_BOARD::MINE_COUNT) ? 1 : 0)) {
                continue;
            }

            current_mine_count--;

            this->board_matrix[y][x] |= 0b1001;
        }
    }

    if (current_mine_count > 0) {
        generate_mines(current_mine_count);
    }
}


void Minesweeper_Board::reveal() {
    if (game_over) {
        return;
    }

    this->reveal(this->selected_pos_y, this->selected_pos_x);
}

void Minesweeper_Board::reveal(uint8_t pos_y, uint8_t pos_x) {

    // break recursion if the square is already revealed
    if (((this->board_matrix[pos_y][pos_x] >> 7) & 1) == 1) {
        return;
    }

    // mark current square as revealed
    this->board_matrix[pos_y][pos_x] |= (1 << 7); // revealed bit
    this->board_matrix[pos_y][pos_x] |= (1 << 4); // update bit

    // if mine, mark as exploded and subtract life
    if ((this->board_matrix[pos_y][pos_x] & 0b1111) == 0b1001) {
        if (this->life > 1) {
            this->life--;
            this->board_matrix[pos_y][pos_x] |= (1<<4);
        } else {
            this->game_over = true;
        }
    }

    // break recursion if the square is not a blank
    if ((this->board_matrix[pos_y][pos_x] & 0b1111) != 0) {
        return;
    }


    for (int8_t y = (pos_y - 1); y < (pos_y + 2); y++) {
        for (int8_t x = (pos_x - 1); x < (pos_x + 2); x++) {

            // skip current square
            if (x == pos_x && y == pos_y) {
                continue;
            }

            // skip out of bounds squares
            if (y < 0 || y > (MINESWEEPER_BOARD::HEIGHT - 1) || x < 0 || x > (MINESWEEPER_BOARD::WIDTH - 1)) {
                continue;
            }

            reveal(y, x);
        }
    }
}

void Minesweeper_Board::move_selected_pos(uint8_t move_direction) {
    if (game_over) {
        return;
    }

    if (move_direction != MINESWEEPER_DIRECTION::UP && move_direction != MINESWEEPER_DIRECTION::DOWN && move_direction != MINESWEEPER_DIRECTION::LEFT && move_direction != MINESWEEPER_DIRECTION::RIGHT) {
        return;
    }

    // set selected square for update to remove highlight
    this->board_matrix[this->selected_pos_y][this->selected_pos_x] |= (1 << 4);

    uint8_t old_selected_pos_y = this->selected_pos_y;
    uint8_t old_selected_pos_x = this->selected_pos_x;

    switch (move_direction) {
        case MINESWEEPER_DIRECTION::UP:
            this->selected_pos_y--;
            break;
        case MINESWEEPER_DIRECTION::DOWN:
            this->selected_pos_y++;
            break;
        case MINESWEEPER_DIRECTION::LEFT:
            this->selected_pos_x--;
            break;
        case MINESWEEPER_DIRECTION::RIGHT:
            this->selected_pos_x++;
            break;
    }

    // prevent moving out of bounds
    if (this->selected_pos_y < 0 || this->selected_pos_y > (MINESWEEPER_BOARD::HEIGHT - 1) || this->selected_pos_x < 0 || this->selected_pos_x > (MINESWEEPER_BOARD::WIDTH - 1)) {
        this->selected_pos_y = old_selected_pos_y;
        this->selected_pos_x = old_selected_pos_x;
    }
}

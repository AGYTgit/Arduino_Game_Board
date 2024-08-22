#include "minesweeper_board.h"

Minesweeper_Board::Minesweeper_Board(int16_t _board_pos_x, int16_t _board_pos_y) :
    board_pos_x(_board_pos_x), board_pos_y(_board_pos_y) {

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
            if (((board_matrix[y][x] & 0b00010000) >> 4) == 1) {
                if ((board_matrix[y][x] & 0b1111) == 0) {
                    lcd.draw_rect(board_pos_x + x * MINESWEEPER_BOARD::GRID_SIZE, board_pos_y + y * MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, lcd.rgb(100,100,100));
                    lcd.draw_frame(board_pos_x + x * MINESWEEPER_BOARD::GRID_SIZE, board_pos_y + y * MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, 1, lcd.rgb(75,75,75));
                } else if ((board_matrix[y][x] & 0b1111) == 0b1001) {
                    lcd.draw_rect(board_pos_x + x * MINESWEEPER_BOARD::GRID_SIZE, board_pos_y + y * MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, lcd.rgb(255,0,0));
                } else {
                    lcd.draw_rect(board_pos_x + x * MINESWEEPER_BOARD::GRID_SIZE, board_pos_y + y * MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, MINESWEEPER_BOARD::GRID_SIZE, lcd.rgb(0,255 - ((board_matrix[y][x] & 0b1111) * 28),255 - ((board_matrix[y][x] & 0b1111) * 28)));
                }
                board_matrix[y][x] &= 0b11101111;
            }
        }
    }
}


void Minesweeper_Board::first_reveal() {

    // generate mines
    generate_mines(MINESWEEPER_BOARD::MINE_COUNT);

    // generate number/blank
    for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
        for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {

            if ((this->board_matrix[y][x] & 0b1111) == 0b1001) {
                continue;
            }

            uint8_t number = 0;
            
            for (int8_t y2 = y - 1; y2 <= (y + 1); y2++) {
                for (int8_t x2 = x - 1; x2 <= (x + 1); x2++) {
                    if (y2 < 0 || y2 > (MINESWEEPER_BOARD::HEIGHT - 1) || x2 < 0 || x2 > (MINESWEEPER_BOARD::WIDTH - 1)) {
                        continue;
                    }

                    if ((this->board_matrix[y2][x2] & 0b1111) == 0b1001) {
                        number++;
                    }
                }
            }

            this->board_matrix[y][x] |= number;
        }
    }

    // print board state on Serial
    // for (uint8_t y = 0; y < MINESWEEPER_BOARD::HEIGHT; y++) {
    //     for (uint8_t x = 0; x < MINESWEEPER_BOARD::WIDTH; x++) {

    //         if ((this->board_matrix[y][x] & 0b1111) == 0b1001) {
    //             Serial.print('X');
    //             Serial.print(' ');
    //         } else if ((this->board_matrix[y][x] & 0b1111) == 0) {
    //             Serial.print(' ');
    //             Serial.print(' ');
    //         } else  {
    //             Serial.print(this->board_matrix[y][x] & 0b1111);
    //             Serial.print(' ');
    //         }
    //     }
    //     Serial.println();
    // }
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

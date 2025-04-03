#include <Arduino.h>
#include <EEPROM.h>
#include <gui.h>
#include <ui.h>
#include <Tetris.h>


ST7789V lcd = ST7789V();

Button_Grid button_grid = Button_Grid(8, 10, 0x24);


uint16_t read(int address) {
    uint16_t output;
    EEPROM.get(address, output);
    return output;
}

void write(int address, uint16_t input) {
    EEPROM.put(address, input);
}


uint8_t tetris_pause() {
    Menu m = Menu(0x12, lcd.rgb(0,0,0));

    if (true) { // setup
        m.init();

        m.add_button(lcd, 0, 0, 60, 100, 120, 50, (char*)"RESUME", lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
        m.add_button(lcd, 0, 1, 70, 170, 100, 50, (char*)"EXIT", lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));

        m.draw(lcd); 
    }

    while (true) { // loop
        switch (button_grid.scan()) {
        case 0xFF:
            break;
        case 0x10:
            m.move(2);
            break;
        case 0x11:
            m.move(1);
            break;
        case 0x01:
            m.move(0);                                                      
            break;
        case 0x00:
            m.move(3);
            break;
        case 0x02:
            switch (m.get_position()) {
            case 0x00:
                m.undraw(lcd);
                return 0;
            case 0x01:
                m.undraw(lcd);
                return 1;
            }
            break;
        }
    }
}


void tetris_game() {
    Tetris_Board board = Tetris_Board(0, 0, 170, 136);

    uint16_t time_to_drop = 1000;
    uint16_t time_to_move = 3000;

    float speed_modifier = 1;

    unsigned long time_of_last_drop = 0;
    unsigned long time_of_last_move = 0;

    bool first_hold_state = true;
    bool hold_allowed_state = true;

    if (true) { // setup
        board.draw(lcd);
        
        board.add_next_block();
        board.draw(lcd);
        time_of_last_drop = millis();
        time_of_last_move = millis();

        board.display_future_blocks(lcd);
        board.display_hold_block(lcd);
        board.update_score(lcd);
    }

    while (true) { // loop
        uint8_t scan_output = button_grid.scan();

        switch (scan_output) {
        case 0xFF:
            break;
        case 0x10:
            if (board.move_block(TETRIS_DIRECTION::LEFT)) {
                board.draw(lcd);
                time_of_last_move = millis();
            }
            break;
        case 0x11:
            if (board.move_block(TETRIS_DIRECTION::DOWN)) {
                board.draw(lcd);
                time_of_last_move = millis();
            }
            break;
        case 0x01:
            board.drop();
            board.draw(lcd);
            time_of_last_drop = 0;
            time_of_last_move = millis() + time_to_move;
            break;
        case 0x00:
            if (board.move_block(TETRIS_DIRECTION::RIGHT)) {
                board.draw(lcd);
                time_of_last_move = millis();
            }
            break;
        case 0x13:
            if (board.rotate_block(TETRIS_DIRECTION::CCW)) {
                board.draw(lcd);
                time_of_last_move = millis();
            }
            break;
        case 0x02:
            if (hold_allowed_state) {
                hold_allowed_state = false;
                board.hold();
                board.draw(lcd);
                if (first_hold_state) {
                    first_hold_state = false;
                    board.display_future_blocks(lcd);
                }
                board.display_hold_block(lcd);
                time_of_last_drop = millis();
                time_of_last_move = millis();
            }
            break;
        case 0x12:
            if (tetris_pause() == 0) {
                board.draw(lcd, true);
                board.display_future_blocks(lcd);
            } else {
                board.save_score();
                lcd.fill();
                return;
            }
            break;
        case 0x03:
            if (board.rotate_block(TETRIS_DIRECTION::CW)) {
                board.draw(lcd);
                time_of_last_move = millis();
            }
            break;
        }


        if (millis() - time_of_last_drop >= time_to_drop / speed_modifier) {
            if (board.move_block(TETRIS_DIRECTION::DOWN)) {
                board.draw(lcd);
                time_of_last_drop = millis();
                time_of_last_move = millis();
            }
        }

        if (millis() - time_of_last_move >= time_to_move) {
            if (board.get_block_y() <= 1) {
                board.save_score();
                return;
            }

            hold_allowed_state = true;

            board.clear_completed_lines(lcd);

            board.add_next_block();
            board.draw(lcd);
            time_of_last_drop = millis();
            time_of_last_move = millis();

            board.display_future_blocks(lcd);

            board.update_score(lcd);

            board.draw(lcd);

            
            speed_modifier = speed_modifier * 1.02f;
        }
    }
}


bool tetris_records() {
    Menu m = Menu(0x20, lcd.rgb(0,0,0), 10);

    uint16_t* scores = new uint16_t[5];
    uint8_t score_len = 0;

    for (uint8_t i = 0; i < 5; ++i) {
        scores[i] = read(i * 2);
    }

    for (uint8_t i = 0; i < 5; ++i) {
        if (scores[i] == 0) {
            break;
        }
        score_len++;
    }

    m.button_grid_dimensions += score_len + 1;

    if (true) { // setup
        m.init();

        m.add_button(lcd, 0, 0, 10, 10, 220, 50, (char*)"BACK", lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));

        for (uint8_t i = 0; i < score_len; ++i) {
            // char buffer2[6];
            // snprintf(buffer2, sizeof(buffer2), "%d:", i + 1);
            // m.add_text(20, 80 + 30 * i, 32, 24, strdup(buffer2));

            char buffer[8];
            sprintf(buffer, "%u", scores[i]);
            m.add_text(50, 80 + 30 * i, 100, 24, strdup(buffer));

            m.add_button(lcd, 0, 1 + i, 160, 80 + 30 * i, 60, 24, (char*)"DEL", lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
        }

        m.draw(lcd);
    }

    while (true) { // loop
        switch (button_grid.scan()) {
        case 0xFF:
            break;
        case 0x10:
            m.move(2);
            break;
        case 0x11:
            m.move(1);
            break;
        case 0x01:
            m.move(0);                                                      
            break;
        case 0x00:
            m.move(3);
            break;
        case 0x02:
            if (m.get_position() == 0x00) {
                m.undraw(lcd);
                return 0;
            } else if (m.get_position() > 0) {
                int8_t delete_index = (m.get_position() & 0x0F) - 1;
                for (uint8_t i = delete_index; i < score_len - 1; i++) {
                    scores[i] = scores[i + 1];
                }

                scores[score_len - 1] = 0;

                for (int8_t i = 0; i < score_len; i++) {
                    write(i * 2, scores[i]);
                }
                
                m.undraw(lcd);
                return 1;
            }
            break;
        }
    }
    return 0;
}


void tetris_menu() {
    Menu m = Menu(0x12, lcd.rgb(0,0,0));

    if (true) { // setup
        m.init();

        m.add_button(lcd, 0, 0, 70, 85, 100, 50, (char*)"PLAY", lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
        m.add_button(lcd, 0, 1, 55, 155, 130, 50, (char*)"RECORDS", lcd.rgb(255,0,255), 2, lcd.rgb(255,255,255));

        m.draw(lcd); 
    }

    while (true) { // loop
        switch (button_grid.scan()) {
        case 0xFF:
            break;
        case 0x10:
            m.move(2);
            break;
        case 0x11:
            m.move(1);
            break;
        case 0x01:
            m.move(0);                                                      
            break;
        case 0x00:
            m.move(3);
            break;
        case 0x02:
            switch (m.get_position()) {
            case 0x00:
                m.undraw(lcd);
                tetris_game();
                m.draw(lcd);
                break;
            case 0x01:
                m.undraw(lcd);
                while (tetris_records() == 1);
                m.draw(lcd);
                break;
            }
            break;
        }
    }
}


int main(void) {
    init();

    Serial.begin(115200);
    Serial.println(1);
    Serial.println(read(0));

    lcd.Init();
    lcd.fill();

    button_grid.init();

    tetris_menu();
        
    return 0;
}
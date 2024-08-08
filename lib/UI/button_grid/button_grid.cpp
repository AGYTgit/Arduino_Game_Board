#include "button_grid.h"

Button_Grid::Button_Grid(uint8_t _input_pin_offset, uint8_t _output_pin_offset, uint8_t _grid_width, uint8_t _grid_heigh) :
    input_pin_offset(_input_pin_offset), output_pin_offset(_output_pin_offset), grid_width(_grid_width), grid_height(_grid_heigh) {}

Button_Grid::~Button_Grid() {
    delete[] this->button_pressed;
}

void Button_Grid::init() {
    this->button_pressed = new bool*[this->grid_height];
    for (uint8_t i = 0; i < this->grid_height; i++) {
        pinMode(i + this->input_pin_offset, INPUT_PULLUP);
        pinMode(i + this->output_pin_offset, OUTPUT);
        this->button_pressed[i] = new bool[this->grid_width];
        for (uint8_t j = 0; j < this->grid_width; j++) {
            this->button_pressed[i][j] = false;
        }
    }

    for (uint8_t i = 0; i < this->grid_height * this->grid_width; i++) {
        uint8_t* _ = new uint8_t[2];
        this->scan(_, 0);
    }
}

bool Button_Grid::scan(uint8_t*& button_coordinations, uint16_t scan_delay) {
    for (int i = 0; i < this->grid_width; i++) {
        digitalWrite(i + this->input_pin_offset, HIGH);
        for (int ip = 1; ip < this->grid_width; ip++) {
            digitalWrite(((i + ip) % this->grid_width) + this->input_pin_offset, LOW);
        }

        for (int j = 0; j < this->grid_height; j++) {
            digitalWrite(j + this->output_pin_offset, LOW);
            for (int jp = 1; jp < this->grid_height; jp++) {
                digitalWrite(((j + jp) % this->grid_height) + this->output_pin_offset, HIGH);
            }

            if (digitalRead(i + this->input_pin_offset) == LOW && this->button_pressed[j][i]) {
                this->button_pressed[j][i] = false;
            } else if (digitalRead(i + this->input_pin_offset) != LOW && !this->button_pressed[j][i]) {
                this->button_pressed[j][i] = true;
                
                button_coordinations = new uint8_t[2];
                button_coordinations[0] = j;
                button_coordinations[1] = i;

                return true;
            }
            delay(scan_delay);
        }
    }
    return false;
}

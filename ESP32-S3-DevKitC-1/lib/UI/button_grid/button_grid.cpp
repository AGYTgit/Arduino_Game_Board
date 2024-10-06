#include "button_grid.h"

Button_Grid::Button_Grid(uint8_t _input_pin_offset, uint8_t _output_pin_offset, uint8_t _button_grid_dimensions) :
    input_pin_offset(_input_pin_offset), output_pin_offset(_output_pin_offset), button_grid_dimensions(_button_grid_dimensions) {}

Button_Grid::~Button_Grid() {
    for (uint8_t i = 0; i < (this->button_grid_dimensions >> 4); i++) {
        delete[] this->button_pressed[i];
    }
    delete[] this->button_pressed;
}

void Button_Grid::init() {
    this->button_pressed = new bool*[button_grid_dimensions & 0x0F];
    for (uint8_t i = 0; i < (button_grid_dimensions & 0x0F); i++) {
        this->button_pressed[i] = new bool[button_grid_dimensions >> 4];
        for (uint8_t j = 0; j < (button_grid_dimensions >> 4); j++) {
            this->button_pressed[i][j] = false;
        }
    }

    for (uint8_t i = 0; i < (button_grid_dimensions >> 4); i++) {
        pinMode(i + this->input_pin_offset, INPUT_PULLUP);
    }
    for (uint8_t i = 0; i < (button_grid_dimensions & 0x0F); i++) {
        pinMode(i + this->output_pin_offset, OUTPUT);
    }

    for (uint8_t i = 0; i < (button_grid_dimensions & 0x0F) * (button_grid_dimensions >> 4); i++) {
        this->scan(0);
    }
}

uint8_t Button_Grid::scan(uint8_t scan_delay) {
    for (uint8_t i = 0; i < (button_grid_dimensions >> 4); i++) {
        digitalWrite(i + this->input_pin_offset, HIGH);
        Serial.println(i + this->input_pin_offset); //
        for (uint8_t ip = 1; ip < (button_grid_dimensions >> 4); ip++) {
            digitalWrite(((i + ip) % (button_grid_dimensions >> 4)) + this->input_pin_offset, LOW);
            Serial.println(((i + ip) % (button_grid_dimensions >> 4)) + this->input_pin_offset); //
        }

        for (uint8_t j = 0; j < (button_grid_dimensions & 0x0F); j++) {
            digitalWrite(j + this->output_pin_offset, LOW);
            Serial.println(j + this->output_pin_offset); //
            for (uint8_t jp = 1; jp < (button_grid_dimensions & 0x0F); jp++) {
                digitalWrite(((j + jp) % (button_grid_dimensions & 0x0F)) + this->output_pin_offset, HIGH);
                Serial.println(((j + jp) % (button_grid_dimensions & 0x0F)) + this->output_pin_offset); //
            }

            if (digitalRead(i + this->input_pin_offset) == LOW && this->button_pressed[j][i]) {
                this->button_pressed[j][i] = false;
            } else if (digitalRead(i + this->input_pin_offset) != LOW && !this->button_pressed[j][i]) {
                this->button_pressed[j][i] = true;

                return (i << 4) | j;
            }
            delay(scan_delay);
        }
    }
    return 0xFF;
}

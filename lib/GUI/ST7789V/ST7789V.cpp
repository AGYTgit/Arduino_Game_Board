#include "ST7789V.h"

void ST7789V::LCD_write(uint8_t d) {
    digitalWrite(LCD_WR, LOW);

    // PORTD = (PORTD & 0b00000011) | ((d) & 0b11111100);
    // PORTB = (PORTB & 0b11111100) | ((d) & 0b00000011);

    PORTD = d;
    
    digitalWrite(LCD_WR, HIGH);
}

void ST7789V::LCD_command_write(uint8_t command) {
    digitalWrite(LCD_RS, LOW);
    LCD_write(command);
}

void ST7789V::LCD_data_write(uint8_t data) {
    digitalWrite(LCD_RS, HIGH);
    LCD_write(data);
}


void ST7789V::Init() {
    // DDRD = DDRD | B11111100;
    // DDRB = DDRB | B00000011;
    // DDRC = DDRC | B00011111;
    // PORTC = PORTC | B00011111;

    DDRD = B11111111;
    DDRC = DDRC | B00011111;
    PORTC = PORTC | B00011111;


    digitalWrite(LCD_RST, HIGH);
    delay(5);
    digitalWrite(LCD_RST, LOW);
    delay(15);
    digitalWrite(LCD_RST, HIGH);
    delay(15);

    digitalWrite(LCD_CS, HIGH);
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_RD, HIGH);
    digitalWrite(LCD_CS, LOW);

    LCD_command_write(0xC5);
    LCD_data_write(0x54);
    LCD_data_write(0x00);
    
    LCD_command_write(0x36);
    LCD_data_write(0b0000000);

    LCD_command_write(0x3A);
    LCD_data_write(0x55);

    LCD_command_write(0x11);
    delay(10);

    LCD_command_write(0x29);
    LCD_command_write(0x2c);
}


void ST7789V::set_address(int16_t y1, int16_t y2, int16_t x1, int16_t x2) {
    LCD_command_write(0x2a);
    LCD_data_write(y1 >> 8);
    LCD_data_write(y1);
    LCD_data_write(y2 >> 8);
    LCD_data_write(y2);

    LCD_command_write(0x2b);
    LCD_data_write(x1 >> 8);
    LCD_data_write(x1);
    LCD_data_write(x2 >> 8);
    LCD_data_write(x2);

    LCD_command_write(0x2c);
}


uint16_t ST7789V::rgb(int r, int g, int b) {
  uint16_t r5 = (r >> 3) & 0x1F;  // 5 bits
  uint16_t g6 = (g >> 2) & 0x3F;  // 6 bits
  uint16_t b5 = (b >> 3) & 0x1F;  // 5 bits

  // Combine the bits into a single 16-bit value
  return (r5 << 11) | (g6 << 5) | b5;
}


void ST7789V::draw_pixel(int16_t x, int16_t y, uint16_t color) {
    set_address(x, x, y, y);
    LCD_data_write(color >> 8);
    LCD_data_write(color);
}

void ST7789V::draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color) {
    set_address(x, x + width, y, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < height + 1; i++) {
        for (uint16_t j = 0; j < width + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }
}

void ST7789V::draw_frame(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color) {
    set_address(x, x + width, y, y + thickness);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < thickness + 1; i++) {
        for (uint16_t j = 0; j < width + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }

    set_address(x, x + width, y + height - thickness, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < thickness + 1; i++) {
        for (uint16_t j = 0; j < width + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }

    set_address(x, x + thickness, y + thickness, y + height - thickness);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < height + 1; i++) {
        for (uint16_t j = 0; j < thickness + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }

    set_address(x + width - thickness, x + width, y + thickness, y + height - thickness);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < height + 1; i++) {
        for (uint16_t j = 0; j < thickness + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }
}

void ST7789V::fill(uint16_t color) {
    set_address(0, LCD_WIDTH, 0, LCD_HEIGHT);
    digitalWrite(LCD_RS, HIGH);
    for (uint16_t i = 0; i < LCD_WIDTH + 1; i++) {
        for (uint16_t j = 0; j < LCD_HEIGHT + 1; j++) {
            LCD_write(color >> 8);
            LCD_write(color);
        }
    }
}


void ST7789V::draw_char(int16_t pos_x, int16_t pos_y, uint16_t width, uint16_t height, uint8_t scale, uint8_t character, uint16_t color) {
    if (scale == 0) {
        return;
    }

    uint8_t char_index;
    for (uint8_t i = 0; i < this->SBMFont8x8_length; i++) {
        if (this->SBMFont8x8_characters[i] == character) {
            char_index = i;
            break;
        }
    }

    int8_t offset_x = (width - (8 * scale)) / 2;
    int8_t offset_y = (height - (8 * scale)) / 2;

    for (uint8_t y = 0; y < this->SBMFont8x8_size; y++) {
        for (uint8_t x = 0; x < this->SBMFont8x8_size; x++) {
            if (((this->SBMFont8x8_character_bitmap[char_index][y] >> ((this->SBMFont8x8_size - 1) - x)) & 1) == 0) {
                continue;
            }

            draw_rect(pos_x + (x * scale) + offset_x, pos_y + (y * scale) + offset_y, scale, scale, color);
        }
    }
}

void ST7789V::draw_text(int16_t pos_x, int16_t pos_y, uint16_t width, uint16_t height, uint8_t scale, int8_t spacing, const uint8_t* text, uint16_t color) {
    for (const char* c = text; *c != '\0'; c++) {
        draw_char(pos_x, pos_y, width, height, scale, *c, color);
        pos_x += 8 * scale + spacing;
    }
}

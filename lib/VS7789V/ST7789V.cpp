#include <ST7789V.h>
#include <pin_magic.h>
#include <dimensions.h>

ST7789V::ST7789V() {
    setup_pins();
}

void ST7789V::setup_pins() {
    DDRD = DDRD | B11111100;
    DDRB = DDRB | B00000011;
    DDRC = DDRC | B00011111;
    PORTC = PORTC | B00011111;

    // DDRD = B11111111;
    // DDRC = DDRC | B00011111;
    // PORTC = PORTC | B00011111;
}

void ST7789V::LCD_write(uint8_t d) {
    digitalWrite(LCD_WR, LOW);
    PORTD = (PORTD & B00000011) | ((d) & B11111100);
    PORTB = (PORTB & B11111100) | ((d) & B00000011);

    // PORTD = d;
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
    setup_pins();
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
    LCD_data_write(B0000000);

    LCD_command_write(0x3A);
    LCD_data_write(0x55);

    LCD_command_write(0x11);
    delay(10);

    LCD_command_write(0x29);
    LCD_command_write(0x2c);
}

uint16_t ST7789V::rgb(int r, int g, int b) {
  uint16_t r5 = (r >> 3) & 0x1F;  // 5 bits
  uint16_t g6 = (g >> 2) & 0x3F;  // 6 bits
  uint16_t b5 = (b >> 3) & 0x1F;  // 5 bits

  // Combine the bits into a single 16-bit value
  return (r5 << 11) | (g6 << 5) | b5;
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

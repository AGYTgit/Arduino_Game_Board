class arduino_ST7789V {
  int8_t LCD_RD; //Serves as read signal/MCU read data at the rising edge. Pg11 Datasheet
  int8_t LCD_WR; //Serves as write signal/command at the rising edge
  int8_t LCD_RS; //D/CX (0=Command/1=Data)       
  int8_t LCD_CS; //Chip Select Pin : Active Low
  int8_t LCD_RST; //Shield Reset

  uint16_t LCD_WIDTH;
  uint16_t LCD_HEIGHT;

public:
  arduino_ST7789V(int8_t RD=A0, int8_t WR=A1, int8_t RS=A2, int8_t CS=A3, int8_t RST=A4, int16_t width=240, int16_t height=320) {
    LCD_RD = RD;
    LCD_WR = WR;
    LCD_RS = RS;
    LCD_CS = CS;
    LCD_RST = RST;

    LCD_WIDTH = width;
    LCD_HEIGHT = height;
  }

  void setup_pins() {
    DDRD = DDRD | B11111100;
    // Setting Pin 8-9 as Output
    DDRB = DDRB | B00000011;
    //Setting Analog Pins A4-A0 as Output
    DDRC = DDRC | B00011111;
    // Setting Analog Pins A4-A0 as HIGH
    PORTC = PORTC | B00011111;
  }

  void LCD_write(uint8_t d) {
    // Serves as write signal/command at the rising edge
    digitalWrite(LCD_WR, LOW); // WR 0

    // LCD Pins |7|6|5|4|3|2|1|0|
    // Uno Pins |7|6|5|4|3|2|9|8|

    //Arduino Port D : Pin 0 - 7, but Pin 0,1 not used in the LCD Shield
    //Arduino Port B : Pin 8 - 13,but Pin, 10,11,12,13 not used in the LCD Shield
    //Arduino Port C : Analog Pins

    PORTD = (PORTD & B00000011) | ((d) & B11111100);
    PORTB = (PORTB & B11111100) | ((d) & B00000011);

    digitalWrite(LCD_WR, HIGH); // WR 1
  }

  void LCD_command_write(uint8_t command) {
    // LCD_RS = 0, A2=0, D/CX (0=Command/1=Data) | DataSheet Page 11
    digitalWrite(LCD_RS, LOW);
    LCD_write(command);
  }

  void LCD_data_write(uint8_t data) {
    // LCD_RS = 1, A2=1, D/CX (0=Command/1=Data) | DataSheet Page 11
    digitalWrite(LCD_RS, HIGH);
    LCD_write(data);
  }

  void Lcd_Init(void) {
    setup_pins();
    //void does not return any value
    //void only execute instruction within it
    //similar to void setup and loop
    //This function will have LCD initialization measures
    //Only the necessary Commands are covered
    //Eventho there are so many more in DataSheet

    //Reset Signal is Active LOW
    digitalWrite(LCD_RST, HIGH);
    delay(5);
    digitalWrite(LCD_RST, LOW); //Actual Reset Done Here
    delay(15);
    digitalWrite(LCD_RST, HIGH);
    delay(15);

    //The below is just preparation for Write Cycle Seq
    digitalWrite(LCD_CS, HIGH); //Chip-Select Active Low Signal
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_RD, HIGH);
    digitalWrite(LCD_CS, LOW);  //Chip-Select Active Low Signal

    LCD_command_write(0xC5);    //Test this Out | VCOM Control 1 : Colour Contrast Maybe
    LCD_data_write(0x54);       //VCOM H 1111111 0x7F
    LCD_data_write(0x00);       //VCOM L 0000000
    //LCD_data_write(B1010011);
    
    LCD_command_write(0x36);    //Memory Access Control | DataSheet Page 127
    ///LCD_data_write(0x48);       //Adjust this value to get right color and starting point of x and y
    LCD_data_write(B0000000);     //Example

    LCD_command_write(0x3A);    //COLMOD: Pixel Format Set | DataSheet Page 134
    LCD_data_write(0x55);       //16 Bit RGB and MCU

    LCD_command_write(0x11);    //Sleep Out | DataSheet Page 245
    delay(10);                  //Necessary to wait 5msec before sending next command

    LCD_command_write(0x29);    //Display on.

    LCD_command_write(0x2c);    //Memory Write | DataSheet Page 245
  }

  void set_address(int16_t y1, int16_t y2, int16_t x1, int16_t x2) {
    LCD_command_write(0x2a);  //Column Address Set | DataSheet Page 110
    LCD_data_write(y1 >> 8);  //8 Bit Shift Right of x1
    LCD_data_write(y1);       //Value of x1
    LCD_data_write(y2 >> 8);  //8 Bit Shift Right of x2
    LCD_data_write(y2);       //Value of x2

    LCD_command_write(0x2b);  //Page Address Set | DataSheet Page 110
    LCD_data_write(x1 >> 8);  //8 Bit Shift Right of y1
    LCD_data_write(x1);       //Value of y1
    LCD_data_write(x2 >> 8);  //8 Bit Shift Right of y2
    LCD_data_write(x2);       //Value of y2

    LCD_command_write(0x2c); // REG 2Ch = Memory Write
  }

  void draw_pixel(int16_t x, int16_t y, uint16_t color) {
    set_address(x, x, y, y);
    LCD_data_write(color >> 8);
    LCD_data_write(color);
  }

  void draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color) {
    set_address(x, x + width, y, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < height + 1; i++) {
      for (int j = 0; j < width + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }
  }

  void draw_frame(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color) {
    set_address(x, x + width, y, y + thickness);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < thickness + 1; i++) {
      for (int j = 0; j < width + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }

    set_address(x, x + width, y + height - thickness, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < thickness + 1; i++) {
      for (int j = 0; j < width + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }

    set_address(x, x + thickness, y, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < height + 1; i++) {
      for (int j = 0; j < thickness + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }

    set_address(x + width - thickness, x + width, y, y + height);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < height + 1; i++) {
      for (int j = 0; j < thickness + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }
  }

  void fill(uint16_t color=0) {
    set_address(0, LCD_WIDTH, 0, LCD_HEIGHT);
    digitalWrite(LCD_RS, HIGH);
    for (int i = 0; i < LCD_WIDTH + 1; i++) {
      for (int j = 0; j < LCD_HEIGHT + 1; j++) {
        LCD_write(color >> 8);
        LCD_write(color);
      }
    }
  }
};

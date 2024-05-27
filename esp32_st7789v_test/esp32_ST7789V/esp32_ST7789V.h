#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define RED2    0x4000
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREEN2  0x2FA4
#define CYAN2   0x07FF

const int LCD_RD = 22; // Read (unused in this example)
const int LCD_WR = 21; // Write
const int LCD_RS = 18; // D/CX (0=Command/1=Data)
const int LCD_CS = 19; // Chip Select Pin : Active Low
const int LCD_RST = 23; // Shield Reset
const int LCD_DATA_PINS[] = {2, 4, 15, 13, 12, 14, 26, 5}; // Data pins (assuming 8 data pins)

void check_for_error(esp_err_t err) {
  if (err != ESP_OK) {
    Serial.print("Error: ");
    Serial.println(err);
  }
}

void setup_lcd_pins() {
  // Set LCD control pins as outputs (except RD, which is unused)
  pinMode(LCD_RD, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RST, HIGH);

  // Set LCD data pins as outputs
  for (int pin : LCD_DATA_PINS) {
    pinMode(pin, OUTPUT);
  }
}

void LCD_write(uint8_t data) {
  // Serves as write signal/command at the rising edge
  gpio_set_level((gpio_num_t)LCD_WR, LOW); // WR 0
  // Serial.print(0);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_WR));
 
  for (int i = 0; i < 8; i++) {
    check_for_error(gpio_set_level((gpio_num_t)LCD_DATA_PINS[i], (data >> i) & 0x01));
  }

  // for (int i = 7; i > -1; i--) {
  //   Serial.print(gpio_get_level((gpio_num_t)LCD_DATA_PINS[i]));
  //   Serial.print(" ");
  // }
  // Serial.println();

  gpio_set_level((gpio_num_t)LCD_WR, HIGH); // WR 1
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_WR));
}

void LCD_command_write(uint8_t command) {
  // LCD_RS = 0, A2=0, D/CX (0=Command/1=Data) | DataSheet Page 11
  gpio_set_level((gpio_num_t)LCD_RS, LOW);
  // Serial.print(0);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RS));
  LCD_write(command);
}

void LCD_data_write(uint8_t data) {
  // LCD_RS = 1, A2=1, D/CX (0=Command/1=Data) | DataSheet Page 11
  gpio_set_level((gpio_num_t)LCD_RS, HIGH);
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RS));
  LCD_write(data);
}

void Lcd_Init(void) {
  setup_lcd_pins();  // Configure LCD pins
  delay(10);

  //void does not return any value
  //void only execute instruction within it
  //similar to void setup and loop
  //This function will have LCD initialization measures
  //Only the necessary Commands are covered
  //Eventho there are so many more in DataSheet

  //Reset Signal is Active LOW
  gpio_set_level((gpio_num_t)LCD_RST, HIGH);
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RST));
  delay(15);
  gpio_set_level((gpio_num_t)LCD_RST, LOW); //Actual Reset Done Here
  // Serial.print(0);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RST));
  delay(15);
  gpio_set_level((gpio_num_t)LCD_RST, HIGH);
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RST));
  delay(15);

  LCD_command_write(0x01); // software reset
  delay(10);

  //The below is just preparation for Write Cycle Seq
  gpio_set_level((gpio_num_t)LCD_CS, HIGH); //Chip-Select Active Low Signal
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_CS));
  gpio_set_level((gpio_num_t)LCD_WR, HIGH);
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_WR));
  gpio_set_level((gpio_num_t)LCD_RD, HIGH);
  // Serial.print(1);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_RD));
  gpio_set_level((gpio_num_t)LCD_CS, LOW);  //Chip-Select Active Low Signal
  // Serial.print(0);
  // Serial.print(" ");
  // Serial.println(gpio_get_level((gpio_num_t)LCD_CS));

  LCD_command_write(0xC5);    //Test this Out | VCOM Control 1 : Colour Contrast Maybe
  LCD_data_write(0x54);       //VCOM H 1111111 0x7F
  LCD_data_write(0x00);       //VCOM L 0000000
  //LCD_data_write(B1010011);
  
  LCD_command_write(0x36);    //Memory Access Control | DataSheet Page 127
  ///LCD_data_write(0x48);       //Adjust this value to get right color and starting point of x and y
  LCD_data_write(B00000000);     //Example

  LCD_command_write(0x3A);    //COLMOD: Pixel Format Set | DataSheet Page 134
  LCD_data_write(0x55);       //16 Bit RGB and MCU

  LCD_command_write(0x11);    //Sleep Out | DataSheet Page 245
  delay(10);                  //Necessary to wait 5msec before sending next command

  LCD_command_write(0x29);    //Display on.

  LCD_command_write(0x2c);    //Memory Write | DataSheet Page 245
}

void Address_set(int16_t y1, int16_t y2, int16_t x1, int16_t x2) {
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

void drawPixel(int16_t x, int16_t y, uint16_t color) {
  Address_set(y, y, x, x);
  LCD_data_write(color >> 8);
  LCD_data_write(color);
}

void drawRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color) {
  Address_set(y, y + height, x, x + width);
  gpio_set_level((gpio_num_t)LCD_RS, HIGH); // set writing to data
  for (int i = 0; i < height + 1; i++) {
    for (int j = 0; j < width + 1; j++) {
      // gpio_set_level((gpio_num_t)LCD_WR, LOW); // WR 0
      // PORTD = (PORTD & B00000011) | ((color >> 8) & B11111100);
      // PORTB = (PORTB & B11111100) | ((color >> 8) & B00000011);
      // gpio_set_level((gpio_num_t)LCD_WR, HIGH); // WR 1
      // gpio_set_level((gpio_num_t)LCD_WR, LOW); // WR 0
      // PORTD = (PORTD & B00000011) | ((color) & B11111100);
      // PORTB = (PORTB & B11111100) | ((color) & B00000011);
      // gpio_set_level((gpio_num_t)LCD_WR, HIGH); // WR 1
      LCD_data_write(color >> 8);
      LCD_data_write(color);
    }
  }
}

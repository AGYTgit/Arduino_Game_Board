#ifndef PIN_MAGIC_H
#define PIN_MAGIC_H

#define LCD_RD  A0  // Serves as read signal/MCU read data at the rising edge
#define LCD_WR  A1  // Serves as write signal/command at the rising edge
#define LCD_RS  A2  // D/CX (0=Command/1=Data)
#define LCD_CS  A3  // Chip Select Pin : Active Low
#define LCD_RST -1  // Shield Reset

#endif // PIN_MAGIC_H
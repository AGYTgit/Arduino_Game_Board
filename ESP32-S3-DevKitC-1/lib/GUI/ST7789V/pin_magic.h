#ifndef PIN_MAGIC_H
#define PIN_MAGIC_H

#define COMMAND_GPIO_PINS {GPIO_NUM_2, GPIO_NUM_42, GPIO_NUM_41, GPIO_NUM_40, GPIO_NUM_39}
#define LCD_RD  4  // Serves as read signal/MCU read data at the rising edge
#define LCD_WR  3  // Serves as write signal/command at the rising edge
#define LCD_RS  2  // D/CX (0=Command/1=Data)
#define LCD_CS  1  // Chip Select Pin : Active Low
#define LCD_RST 0  // Shield Reset

#define DATA_GPIO_PINS {GPIO_NUM_38, GPIO_NUM_37, GPIO_NUM_36, GPIO_NUM_35, GPIO_NUM_0, GPIO_NUM_45, GPIO_NUM_46, GPIO_NUM_47}

#endif // PIN_MAGIC_H
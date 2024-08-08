#ifndef BUTTON_GRID
#define BUTTON_GRID

#include <Arduino.h>

class Button_Grid {
private:
  uint8_t input_pin_offset;
  uint8_t output_pin_offset;
  
  uint8_t grid_width;
  uint8_t grid_height;

  bool** button_pressed;

public:
  Button_Grid(uint8_t input_pin_offset, uint8_t output_pin_offset, uint8_t grid_width, uint8_t grid_heigh);
  ~Button_Grid();

  void init();

  bool scan(uint8_t*& button_coordinations, uint16_t scan_delay=10);
};

#endif // BUTTON_GRID

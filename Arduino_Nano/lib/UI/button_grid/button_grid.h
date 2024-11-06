#ifndef BUTTON_GRID
#define BUTTON_GRID

#include <Arduino.h>

class Button_Grid {
private:
  uint8_t input_pin_offset;
  uint8_t output_pin_offset;
  
  uint8_t button_grid_dimensions;

  bool** button_pressed;

  uint8_t* scan_return_value;

public:
  Button_Grid(uint8_t _input_pin_offset, uint8_t _output_pin_offset, uint8_t _button_grid_dimensions);
  ~Button_Grid();

  void init();

  uint8_t* scan(uint8_t scan_delay=10);
};

#endif // BUTTON_GRID

// #include <Arduino.h>
// #include <ui.h>


// Button_Grid button_grid = Button_Grid(8, 10, 0x23);


// void load_main_menu() {
//   if (1) { // setup
    
//   }

//   if (1) { // loop
//     while (true) {
//       uint8_t ui_button_coordinations = button_grid.scan();

//       if (ui_button_coordinations == 0xFF) {
//         continue;
//       }

//       Serial.print(ui_button_coordinations >> 4);
//       Serial.print(' ');
//       Serial.print(ui_button_coordinations & 0x0F);
//       Serial.print(' ');
//       Serial.println((ui_button_coordinations >> 4) + ((ui_button_coordinations & 0x0F) * 2));
//     }
//   }
  
//   if (1) { // end
    
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   Serial.println(1);
  
//   button_grid.init();

//   load_main_menu();
// }

// void loop() {
  
// }
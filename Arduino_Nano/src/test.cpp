// #include <Arduino.h>
// #include <ui.h>

// Button_Grid button_grid = Button_Grid(8, 10, 0x24);

// int main(void) {
// 	init();
	
// 	Serial.begin(115200);
//     button_grid.init();

// 	while (true) {
//         uint8_t* scan_output = button_grid.scan();
// 		for (int i = 1; i < scan_output[0]; i++) {
//             switch (scan_output[i]) {
//             case 0xFF:
//                 break;
//             default:
//                 Serial.println(scan_output[i], HEX);
//             }
//         }
//         // button_grid.scan();
//     }
        
// 	return 0;
// }
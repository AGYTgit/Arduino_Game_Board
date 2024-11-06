#include <Arduino.h>
#include <ui.h>

Button_Grid button_grid = Button_Grid(8, 10, 0x24);

int main(void) {
	init();
	
	Serial.begin(115200);
    button_grid.init();

	while (true) {
        // uint8_t* scan_output = button_grid.scan();
		// for (int i = 1; i < scan_output[0]; i++) {
        //     Serial.println(scan_output[i]);
        // }
        button_grid.scan();
    }
        
	return 0;
}
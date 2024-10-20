#include <Arduino.h>
#include <ui.h>


Button_Grid button_grid = Button_Grid(34, 25, 0x23);

void setup() {
    Serial.begin(115200);
    Serial.println(1);

    // gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT);
    // gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);

    // gpio_set_level(GPIO_NUM_32, HIGH);
}

// bool pressed = false;
void loop() {
    // if (gpio_get_level(GPIO_NUM_34) != LOW && !pressed) {
    //     pressed = true;
    //     Serial.println("On");
    // } else if (gpio_get_level(GPIO_NUM_34) == LOW && pressed) {
    //     pressed = false;
    //     Serial.println("Off");
    // }
    // Serial.println(gpio_get_level(GPIO_NUM_34));
    Serial.println(button_grid.scan(), HEX);
    delay(200);
}

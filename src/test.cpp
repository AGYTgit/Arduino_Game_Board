#include <Arduino.h>
#include <ST7789V.h>
#include <gui.h>


ST7789V lcd = ST7789V();

Menu main_menu = Menu(1, 2, lcd.rgb(0,50,0));


#define BUTTONS_WIDTH 2
#define BUTTONS_HEIGHT 2

#define INPUT_BUTTON_OFFSET 10
#define OUTPUT_BUTTON_OFFSET 12

bool test[BUTTONS_HEIGHT][BUTTONS_WIDTH];


void setup() {
  Serial.begin(115200);

  lcd.Init();
  lcd.fill();

  for (int ib = 0; ib < BUTTONS_WIDTH; ib++) {
    pinMode(ib + INPUT_BUTTON_OFFSET, INPUT_PULLUP);
    for (int jb = 0; jb < BUTTONS_HEIGHT; jb++) {
      pinMode(jb + OUTPUT_BUTTON_OFFSET, OUTPUT);
      test[jb][ib] = true;
    }
  }

  main_menu.add_button(0, 0, 10, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
  main_menu.add_button(0, 1, 130, 50, 100, 50, lcd.rgb(255,0,0), 2, lcd.rgb(255,255,255));
  // main_menu.add_button(1, 0, 10, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));
  // main_menu.add_button(1, 1, 130, 120, 100, 50, lcd.rgb(0,0,255), 2, lcd.rgb(255,255,255));

  main_menu.draw(lcd);
  main_menu.highlight(lcd, -1);
}

void loop() {
  for (int i = 0; i < BUTTONS_WIDTH; i++) {
    digitalWrite(i + INPUT_BUTTON_OFFSET, HIGH);
    for (int ip = 1; ip < BUTTONS_WIDTH; ip++) {
      digitalWrite(((i + ip) % BUTTONS_WIDTH) + INPUT_BUTTON_OFFSET, LOW);
    }

    for (int j = 0; j < BUTTONS_HEIGHT; j++) {
      digitalWrite(j + OUTPUT_BUTTON_OFFSET, LOW);
      for (int jp = 1; jp < BUTTONS_HEIGHT; jp++) {
        digitalWrite(((j + jp) % BUTTONS_HEIGHT) + OUTPUT_BUTTON_OFFSET, HIGH);
      }

      if (digitalRead(i + INPUT_BUTTON_OFFSET) == LOW && test[j][i]) {
        test[j][i] = false;
      } else if (digitalRead(i + INPUT_BUTTON_OFFSET) != LOW && !test[j][i]) {
        test[j][i] = true;
        Serial.print(j);
        Serial.print(' ');
        Serial.println(i);
        main_menu.highlight(lcd, 2*j + i);
      }
      delay(10);
    }
  }
}
#include <Arduino.h>

#define led 2

void setup() {
  Serial.begin(115200);
  
  pinMode(led, OUTPUT);
}

void loop() {
  Serial.println(1);

  digitalWrite(led, HIGH);
  delay(1000);  // Wait for a second

  digitalWrite(led, LOW);
  delay(1000);  // Wait for a second
}


#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.requestFrom(8, 6);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
}

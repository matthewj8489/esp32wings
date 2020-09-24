#include <Wire.h>
#include <SparkFunMLX90614.h>

IRTherm therm;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  delay(100);
}

void requestEvent() {
  Wire.write("hello ");
}

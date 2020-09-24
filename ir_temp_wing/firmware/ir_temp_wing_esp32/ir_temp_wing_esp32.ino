#include <Wire.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  SerialBT.begin("ESP32Temp");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  Wire.requestFrom(8, 38);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
    SerialBT.write(c);
  }

  delay(2000);
}

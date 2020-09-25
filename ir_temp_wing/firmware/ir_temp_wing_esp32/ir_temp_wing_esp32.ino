#include <Wire.h>
#include "BluetoothSerial.h"
#include "IR_Temp_Wing.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TMP_DEV 8

BluetoothSerial SerialBT;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  SerialBT.begin("ESP32Temp");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  Wire.requestFrom(TMP_DEV, 38);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
    SerialBT.write(c);
  }

  delay(2000);
}

int getTemperaturesRecordedCount() {
  int tmp_cnt = 0;

  // request the number of temperatures recorded, since last read
  Wire.beginTransmission(TMP_DEV);
  Wire.write("c");
  Wire.endTransmission();
  
  Wire.requestFrom(TMP_DEV, 1);
  if (Wire.available() == 1) {
    tmp_cnt = Wire.read();
  }

  return tmp_cnt;
}

void beginTemperatureRead() {
  // request the temperatures
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_TEMP_READ);
  Wire.endTransmission();
}

String getTemperatureString() {
  String temp;  

  Wire.requestFrom(TMP_DEV, 14); // 14 chars = ##.##,##.##,## --> obj_tmp,amb_tmp,min
  while (Wire.available()) {
    temp += Wire.read();
  }

  return temp;
}

void endTemperatureRead() {
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_TEMP_READ_END);
  Wire.endTransmission();
}

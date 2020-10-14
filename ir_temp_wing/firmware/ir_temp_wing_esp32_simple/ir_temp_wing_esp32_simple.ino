#include <Wire.h>

#define TMP_DEV 8


void setup() {
  Wire.begin();
  Serial.begin(115200);

  Serial.println("ESP32 Temp Reader Ready!");
}

void loop() {
  String temp;
  
  temp = requestTemperature();
  Serial.println(temp);
  delay(1000);

}

String requestTemperature() {
  String temp;
  char c;
  int recvd = 0;

  Serial.println("Askin' for some temp");
  Wire.requestFrom(TMP_DEV, 11); // 11 chars = ##.##,##.## --> obj_tmp,amb_tmp
  Serial.println("Got'em!");
//  while (Wire.available()) {
//    c = Wire.read();
//    temp += c;
//  }
  while (recvd < 11) {
    if (Wire.available()){
      c = Wire.read();
      temp += c;
      recvd++;
    }
    else {
      delay(10);
    }
  }

  return temp;
}

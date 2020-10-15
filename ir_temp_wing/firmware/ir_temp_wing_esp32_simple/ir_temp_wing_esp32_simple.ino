#include <Wire.h>

#define TMP_DEV 8

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 600;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  initWifi();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  Serial.println("ESP32 Temp Reader Ready!");

  delay(100);
}

void loop() {
  String temp;
  String amb;
  String obj;
  
  temp = requestTemperature();
  //Serial.println(temp);
  amb = temp.substring(0, 5);
  obj = temp.substring(6, 11);
  makeIFTTTRequest(amb, obj);
  
  goToSleep();

}

String requestTemperature() {
  String temp;
  char c;
  int recvd = 0;
  int timeout = 0;

  Serial.println("Askin' for some temp");
  Wire.requestFrom(TMP_DEV, 11); // 11 chars = ##.##,##.## --> obj_tmp,amb_tmp
//  while (Wire.available()) {
//    c = Wire.read();
//    temp += c;
//  }
  while (recvd < 11 && timeout < 10000) {
    if (Wire.available()){
      c = Wire.read();
      Serial.print(c);
      temp += c;
      recvd++;
    }
    else {
      delay(1);
      timeout++;
    }
  }
  Serial.println("");
  return temp;
}

void goToSleep() {
  esp_deep_sleep_start();
  
  //delay(1000);
}


#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP_S 2

RTC_DATA_ATTR int bootCount = 0;

void initializeDeepSleep() {
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_S) + " Seconds");  
}

void goToSleep() {
//  Serial.println("Going to sleep now");
//  Serial.flush();
//  esp_deep_sleep_start();
}

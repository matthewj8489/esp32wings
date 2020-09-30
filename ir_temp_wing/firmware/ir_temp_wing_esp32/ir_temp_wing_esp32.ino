#include <Wire.h>
#include "BluetoothSerial.h"
#include "IR_Temp_Wing.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TEMP_STR_BUFF_SZ  20

#define TMP_DEV 8
//#define TMP_WAKE_UP_PIN 4

#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP_S 2

BluetoothSerial SerialBT;

int loops = 0;
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  SerialBT.begin("ESP32Temp");
  //pinMode(TMP_WAKE_UP_PIN, OUTPUT);
  //digitalWrite(TMP_WAKE_UP_PIN, HIGH);
  Serial.println("The device started, now you can pair it with bluetooth!");
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_S) + " Seconds");
}

//void loop() {
//  Wire.requestFrom(TMP_DEV, 38);
//
//  while (Wire.available()) {
//    char c = Wire.read();
//    Serial.print(c);
//    SerialBT.write(c);
//  }
//
//  delay(2000);
//}

void loop() {
  int temp_cnt = 0;
  String temp_str[TEMP_STR_BUFF_SZ];
  int temp_str_cntr = 0;

  // wake up the temp sensor
  //digitalWrite(TMP_WAKE_UP_PIN, LOW);

  temp_cnt = getTemperaturesRecordedCount();
  Serial.print("Temp count: ");
  Serial.println(temp_cnt);
  delay(100);

//  if (temp_cnt > 0) {
//    beginTemperatureRead();
//    delay(100);
//    temp_str = getTemperatureString();
//    Serial.println(temp_str);
//    delay(100);
//    endTemperatureRead();
//  }


  if (temp_cnt > 0 && temp_str_cntr < TEMP_STR_BUFF_SZ) 
  {
    beginTemperatureRead();
    delay(100);
    while (temp_cnt > 0) {  
      temp_str[temp_str_cntr] = getTemperatureString();
      Serial.println(temp_str[temp_str_cntr]);
      temp_str_cntr++;
      delay(100);
      //temp_cnt = getTemperaturesRecordedCount();
      //Serial.print("Temperature count: ");
      //Serial.println(temp_cnt);
      //delay(100);
      temp_cnt--;
    }
    delay(100);
    endTemperatureRead();
  }

  //Serial.print("Loops: ");
  //Serial.println(loops);
  //loops++;

  // reset temp sensor interrupt
  //digitalWrite(TMP_WAKE_UP_PIN, HIGH);

  //delay(2000);
  if (waitForBluetoothConnection())
  {
    while (temp_str_cntr > 0)
    {
      sendBluetoothData(temp_str[temp_str_cntr]);
      temp_str_cntr--;
    }
  }
  else
  {
    // need temp_str[] in RTC memory to survive deep sleeps
  }

  // go to sleep
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
}

int getTemperaturesRecordedCount() {
  int tmp_cnt = 0;

  // request the number of temperatures recorded, since last read
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_RECORD_COUNT);
  Serial.print("Mode: ");
  Serial.println(MLX_MODE_RECORD_COUNT);
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
  Serial.print("Mode: ");
  Serial.println(MLX_MODE_TEMP_READ);
  Wire.endTransmission();
}

String getTemperatureString() {
  String temp;
  char c;  

  Wire.requestFrom(TMP_DEV, 14); // 14 chars = ##.##,##.##,## --> obj_tmp,amb_tmp,min
  while (Wire.available()) {
    c = Wire.read();
    temp += c;
  }

  return temp;
}

void endTemperatureRead() {
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_TEMP_READ_END);
  Serial.print("Mode: ");
  Serial.println(MLX_MODE_TEMP_READ_END);
  Wire.endTransmission();
}

bool waitForBluetoothConnection()
{
  delay(5000);
  return true;
}

void sendBluetoothData(String dat)
{
  SerialBT.println(dat);
}

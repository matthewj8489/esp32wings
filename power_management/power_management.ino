#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BAT_VOLT_PIN  A13

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 Power Manager");

  Serial.println("Here we go again...");
}

void loop() {
  unsigned int bat_volt = 0;
  char volt_str[10];
  int index = 0;
  int str_len = 0;
  float actual_volt = 0;

  bat_volt = analogRead(BAT_VOLT_PIN);
  Serial.print("Ok, what the sidewinder car ticker am I reading?? ");
  Serial.println(bat_volt);
  bat_volt = bat_volt * 2;
  actual_volt = (bat_volt / 4095) * 3.3;
  Serial.print("ACTUAL VOLTAGE: ");
  Serial.println(actual_volt);

  str_len = sprintf(volt_str, "%d\r\n", bat_volt);

  Serial.print("Here be string: ");
  Serial.print(volt_str);
  Serial.print("Chars away..... ");
  for (index = 0; index < str_len; index++)
  {
    Serial.print(volt_str[index]);
    SerialBT.write(volt_str[index]);
  }
  
  delay(1000);
}

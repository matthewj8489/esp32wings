#include <Wire.h>
#include "IR_Temp_Wing.h"

#define TMP_DEV 8
#define TEMP_STR_BUFF_SZ  20

String temp_str[TEMP_STR_BUFF_SZ];
int temp_str_cntr = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  initializeBluetoothLE();
 
  initializeDeepSleep();
}

void loop() {
  int temp_cnt = 0;

  temp_cnt = getTemperaturesRecordedCount();
  //Serial.print("Temp count: ");
  //Serial.println(temp_cnt);
  delay(100);


  if (temp_cnt > 0 && temp_str_cntr < TEMP_STR_BUFF_SZ) 
  {
    beginTemperatureRead();
    delay(100);
    while (temp_cnt > 0) {  
      temp_str[temp_str_cntr] = getTemperatureString();
      //Serial.println(temp_str[temp_str_cntr]);
      temp_str_cntr++;
      delay(100);
      temp_cnt--;
    }
    delay(100);
    endTemperatureRead();
  }

  //delay(2000);
  handleBluetoothLE();
  temp_str_cntr = 0;
  
  // go to sleep
  goToSleep();
}

#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "LowPower.h"
#include "IR_Temp_Wing.h"

#define WAKE_UP_PIN 2

#define SLEEP_10MIN_COUNTER 1 //75 // sleep for 10 minutes (10 * 60 / 8)
#define TIME_MIN_INCREMENT 10
#define TEMP_CHAR_STR_LEN 15
#define TEMP_BUFF_SZ  20

byte mode = MLX_MODE_NOTHING;
byte standby_mode = 0;
char temps[TEMP_BUFF_SZ][TEMP_CHAR_STR_LEN];
byte temps_total_cntr = 0;
byte temps_cntr = 0;
int time_min = 0;

IRTherm therm;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  therm.begin();
  therm.setUnit(TEMP_F);

  pinMode(WAKE_UP_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(WAKE_UP_PIN), wakeUp, LOW);

  Serial.begin(115200);
  Serial.println("IR Temp Wing Ready");
}

void loop() {
  if (!standby_mode)
  {
    measureTemperature();
    //delay(500);
    goToSleep();
  }
}

void receiveEvent(int howMany)
{
  char c;
  if (howMany == 1)
    c = Wire.read();

  mode = c;

  //Serial.print("Recvd Mode: ");
  //Serial.println(int(mode));

  if (mode == MLX_MODE_TEMP_READ_END) {
    Serial.println("Cleared buffers");
    temps_cntr = 0;
    temps_total_cntr = 0;
    time_min = 0;
    standby_mode = 0;
  }
}

void requestEvent()
{
  Serial.print("Mode: ");
  Serial.println(int(mode));
  
  switch(mode) {
    case MLX_MODE_RECORD_COUNT:
      Wire.write(temps_total_cntr);
      Serial.print("Total Temperatures: ");
      Serial.println(temps_total_cntr);
      break;
    case MLX_MODE_TEMP_READ:
      if (temps_cntr < TEMP_BUFF_SZ) {
        Wire.write(temps[temps_cntr]);
        Serial.print("Sent: ");
        Serial.println(temps[temps_cntr]);
        temps_cntr++;
      }
      else {
        Wire.write("ERROR");
        Serial.println("!!!!!!!!ERRROR!!!!!!!!");
        temps_cntr = 0;
        temps_total_cntr = 0;
        time_min = 0;
      }
      break;
//    case MLX_MODE_TEMP_READ_END:
//      temps_cntr = 0;
//      temps_total_cntr = 0;
//      time_min = 0;
//      Serial.println("MODE 3");
//      break;
    default:
      break;
  }
}

void wakeUp()
{
  // handler for wake up pin interrupt
  standby_mode = 1;
}

void goToSleep()
{
  int counter = 0;

  while (counter < SLEEP_10MIN_COUNTER) { 
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    counter++;
  }

  time_min += TIME_MIN_INCREMENT;
}

void measureTemperature()
{
  char obj_s[6];
  char amb_s[6];
  float obj;
  float amb;

  if (therm.read())
  {
    obj = therm.object();
    amb = therm.ambient();
    
    dtostrf(obj, 2, 2, obj_s);
    dtostrf(amb, 2, 2, amb_s);
  
    if (temps_total_cntr < TEMP_BUFF_SZ) {
      sprintf(temps[temps_total_cntr], "%s,%s,%02d", obj_s, amb_s, time_min);
      //Serial.println(temps[temps_total_cntr]);
      temps_total_cntr += 1;
      //Serial.print("Measured a temperature: ");
      //Serial.println(temps_total_cntr);
    }
  }
}

#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "LowPower.h"
#include "IR_Temp_Wing.h"

#define SLEEP_10MIN_COUNTER 75 // sleep for 10 minutes (10 * 60 / 8)
#define TIME_MIN_INCREMENT 10
#define TEMP_CHAR_STR_LEN 15
#define TEMP_BUFF_SZ  20

byte mode = MLX_MODE_NOTHING;
char temps[TEMP_BUFF_SZ][TEMP_CHAR_STR_LEN];
byte temps_total_cntr = 0;
byte temps_cntr = 0;
int time_min = 0;

IRTherm therm;

//float obj;
//float amb;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  therm.begin();
  therm.setUnit(TEMP_F);

  Serial.begin(115200);
  Serial.println("IR Temp Wing Ready");
}

void loop() {
  measureTemperature();
  delay(500);
}

//void loop() {
//
//  if (therm.read()) {
//    obj = therm.object();
//    amb = therm.ambient();  
//  }
//  
//  delay(1000);
//}

//void requestEvent() {
//  //String msg;
//  //char char_msg[60];
//  char c_obj[10];
//  char c_amb[10];
//
//  //msg = "Object: " + String(therm.object(), 2) + "F, Ambient: " + String(therm.ambient(), 2) + "F || ";
//  //msg.toCharArray(char_msg, 59);
//
//  //sprintf(char_msg, "Object: %1.0f \r\nAmbient: %1.0f\r\n", obj, amb);
//
//  //Wire.write(char_msg);
//  
//  //String(therm.object(), 2).toCharArray(obj, 9);
//  //String(therm.ambient(), 2).toCharArray(amb, 9);
//
//  String(obj, 2).toCharArray(c_obj, 9);
//  String(amb, 2).toCharArray(c_amb, 9);
//  
//  Wire.write("Object: ");
//  Wire.write(c_obj);
//  Wire.write("F\r\n");
//  
//  Wire.write("Ambient: ");
//  Wire.write(c_amb);
//  Wire.write("F\r\n");
//}


void receiveEvent(int howMany)
{
  char c;
  if (howMany == 1)
    c = Wire.read();

  mode = c;

  //Serial.print("how many: ");
  //Serial.println(howMany);
  //Serial.print(int(c));
}

void requestEvent()
{
  Serial.print("Mode: ");
  Serial.println(int(mode));
  
  switch(mode) {
    case MLX_MODE_RECORD_COUNT:
      Wire.write(temps_total_cntr);
      Serial.print("Total Temps: ");
      Serial.println(temps_total_cntr);
      break;
    case MLX_MODE_TEMP_READ:
      Wire.write(temps[temps_cntr]);
      Serial.print("<> ");
      Serial.print(temps[temps_cntr]);
      Serial.println("<> ");
      temps_cntr++;
      break;
    case MLX_MODE_TEMP_READ_END:
      temps_cntr = 0;
      temps_total_cntr = 0;
      time_min = 0;
      break;
    default:
      break;
  }
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
    }
  }
}

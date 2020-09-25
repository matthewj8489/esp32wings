#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "LowPower.h"

#define SLEEP_10MIN_COUNTER 75 // sleep for 10 minutes (10 * 60 / 8)

IRTherm therm;

float obj;
float amb;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  therm.begin();
  therm.setUnit(TEMP_F);
}

void loop() {

  if (therm.read()) {
    obj = therm.object();
    amb = therm.ambient();  
  }
  
  delay(1000);
}

void requestEvent() {
  //String msg;
  //char char_msg[60];
  char c_obj[10];
  char c_amb[10];

  //msg = "Object: " + String(therm.object(), 2) + "F, Ambient: " + String(therm.ambient(), 2) + "F || ";
  //msg.toCharArray(char_msg, 59);

  //sprintf(char_msg, "Object: %1.0f \r\nAmbient: %1.0f\r\n", obj, amb);

  //Wire.write(char_msg);
  
  //String(therm.object(), 2).toCharArray(obj, 9);
  //String(therm.ambient(), 2).toCharArray(amb, 9);

  String(obj, 2).toCharArray(c_obj, 9);
  String(amb, 2).toCharArray(c_amb, 9);
  
  Wire.write("Object: ");
  Wire.write(c_obj);
  Wire.write("F\r\n");
  
  Wire.write("Ambient: ");
  Wire.write(c_amb);
  Wire.write("F\r\n");
}

char mode = "n";
void receiveEvent(int howMany)
{
  char c;
  if (howMany == 1)
    c = Wire.read();

  mode = c;
}

void requestEvent()
{
  switch(mode) {
    case "c":
    case "t":
    
  }
}

void goToSleep()
{
  int counter = 0;

  while (counter < SLEEP_10MIN_COUNTER) { 
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    counter++;
  }
}

void measureTemperature()
{

  
}

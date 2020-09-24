#include <Wire.h>
#include <SparkFunMLX90614.h>

IRTherm therm;

float obj;
float amb;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);

  therm.begin();
  therm.setUnit(TEMP_F);
}

void loop() {

  if (therm.read()) {
    obj = therm.object();
    amb = therm.ambient();  
  }
  
  delay(100);
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

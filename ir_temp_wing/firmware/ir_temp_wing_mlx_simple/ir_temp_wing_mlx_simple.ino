#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "LowPower.h"

#define LED_PIN 8

#define TEMP_CHAR_STR_LEN 15

IRTherm therm;
char temp_str[TEMP_CHAR_STR_LEN];
bool measurement_requested = false;

void requestEvent() {
  //measureTemperature();
  Wire.write(temp_str);
  //measurement_requested = true;
}

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);

  therm.begin();
  therm.setUnit(TEMP_F);

  pinMode(LED_PIN, OUTPUT);
  setLED(LOW);

  Serial.begin(115200);
  Serial.println("IR Temp Wing Ready");
  delay(75);
}

void loop() {
  measureTemperature();
  Serial.println(temp_str);
  Serial.flush();
  //if (measurement_requested) {
  //  //Serial.println("HERE U GO!!!!!");
  //  measureTemperature();
  //  Serial.println(temp_str);
  //  Wire.write(temp_str);
  //  measurement_requested = false;
  //}
  goToSleep();  
}

void goToSleep() {
  //delay(500);
  LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_ON);
}

void measureTemperature() {
  char obj_s[6];
  char amb_s[6];
  float obj;
  float amb;

  Serial.println("We be measurin'");
  if (therm.read())
  {
    obj = therm.object();
    amb = therm.ambient();
    
    dtostrf(obj, 2, 2, obj_s);
    dtostrf(amb, 2, 2, amb_s);
  
    sprintf(temp_str, "%s,%s", obj_s, amb_s);
    //Serial.println(temp_str);
  }
  else
  {
    sprintf(temp_str, "");
    //Serial.println("No temp recorded!");
  }
}

void setLED(bool on)
{
  if (on)
    digitalWrite(LED_PIN, LOW);
  else
    digitalWrite(LED_PIN, HIGH);
}

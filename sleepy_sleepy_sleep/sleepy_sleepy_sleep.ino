// Sleepy-sleepy sleep

#include "LowPower.h"

#define LED_PIN 8

volatile byte isInterrupted = 0;

void setup() {
  // put your setup code here, to run once: 
  pinMode(LED_PIN, OUTPUT);
  setLED(LOW);
  
  Serial.begin(115200);
  Serial.println("Sleepy-sleepy sleep awake and ready!");
}

void loop() {
  // put your main code here, to run repeatedly:
  doSomething();  // 0.011A = 11mA
  goToSleep();
}

void doSomething()
{
  if (isInterrupted) {
    setLEDTimed(100);
  } else {
    setLEDTimed(500);
  }
}

void goToSleep()
{
  Serial.println("Going to sleep...wake me in 4s...");
  delay(75);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // 0.07A = 7mA
  //LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
  //              SPI_OFF, USART0_OFF, TWI_ON);
  delay(75);
  Serial.println("Ahhhh...there's the time alarm!");
}

void setLED(bool on)
{
  if (on)
    digitalWrite(LED_PIN, LOW);
  else
    digitalWrite(LED_PIN, HIGH);
}

void setLEDTimed(int time_ms)
{
  int cnt = 4;

  while (cnt > 0)
  {
    setLED(HIGH);
    delay(time_ms);
    setLED(LOW);
    delay(time_ms);
    cnt--;
  }
}

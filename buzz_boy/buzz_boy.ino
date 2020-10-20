#include <LowPower.h>

#define BUZZER_PIN  4

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  Serial.println("Buzz Boy Ready!");
}

void loop() {

  digitalWrite(BUZZER_PIN, HIGH);
  goToSleep(1);
  digitalWrite(BUZZER_PIN, LOW);

  goToSleep(5);
}

void goToSleep(int time_s)
{
  int elapsed = 0;

  while (elapsed < time_s)
  {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
    elapsed += 1;
  }
}

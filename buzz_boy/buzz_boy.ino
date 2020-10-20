#include <LowPower.h>

#define BUZZER_PIN  4
#define BTN1_PIN    2
#define BTN2_PIN    3

volatile int btn1State = LOW;
volatile int btn2State = LOW;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN1_PIN), btn1_pressed, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN2_PIN), btn2_pressed, RISING);

  Serial.begin(115200);
  Serial.println("Buzz Boy Ready!");
}

void loop() {

  if (btn1State) {
    Serial.println("BUTTON 1 PRESSED");
    btn1State = LOW;
  }

  if (btn2State) {
    Serial.println("BUTTON 2 PRESSED");
    btn2State = LOW;
  }

  delay(250);






  //ringBuzzer();

  //goToSleep(5);
}

void btn1_pressed()
{
  btn1State = digitalRead(BTN1_PIN);
}

void btn2_pressed()
{
  btn2State = digitalRead(BTN2_PIN);
}

void ringBuzzer()
{
  digitalWrite(BUZZER_PIN, HIGH);
  goToSleep(1);
  digitalWrite(BUZZER_PIN, LOW);
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

/*
 *    Arduino_Sample/STEP4/STEP4.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#define LED_PIN D13
#define BUZZER_PIN D2
#define SW1_PIN D7
#define SW2_PIN D8

int count = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(D12, OUTPUT);
  digitalWrite(D12, LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(SW1_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
    count++;
    delay(30);
    while (digitalRead(SW1_PIN) == LOW){};
    delay(30);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  if (digitalRead(SW2_PIN) == LOW) {
    for (int i = 0; i < count; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);
    }
    count = 0;
  }
}

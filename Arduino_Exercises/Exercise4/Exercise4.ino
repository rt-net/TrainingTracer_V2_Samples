/*
 *    Arduino_Exercises/Exercise4/Exercise4.ino
 *    
 *    Copyright(C) 2024 RT Corporation <support@rt-net.jp>
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
  if ((digitalRead(SW1_PIN) == LOW) && (digitalRead(SW2_PIN) == LOW)) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  if (digitalRead(SW2_PIN) == LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

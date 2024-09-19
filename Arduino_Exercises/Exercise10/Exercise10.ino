/*
 *    Arduino_Exercises/Exercise10/Exercise10.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#define LINE_L2_PIN A5
#define LINE_L1_PIN A4
#define LINE_R1_PIN A3
#define LINE_R2_PIN A2
#define LED_PIN D13
#define SW1_PIN D7
#define SW2_PIN D8

int flag = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(D12, OUTPUT);
  digitalWrite(D12, LOW);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(SW1_PIN) == LOW) {
    flag = 1;
    delay(30);
    while (digitalRead(SW1_PIN) == LOW) continue;
    delay(30);
  }
  if (digitalRead(SW2_PIN) == LOW) {
    flag = 0;
    delay(30);
    while (digitalRead(SW2_PIN) == LOW) continue;
    delay(30);
  }
  if (flag == 1) {
    Serial.printf(
      "\n\r LL2=%d LL1=%d LR1=%d LR2=%d", analogRead(LINE_L2_PIN), analogRead(LINE_L1_PIN),
      analogRead(LINE_R1_PIN), analogRead(LINE_R2_PIN));
  }
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}

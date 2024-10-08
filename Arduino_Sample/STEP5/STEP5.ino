/*
 *    Arduino_Sample/STEP5/STEP5.ino
 *    
 *    Copyright(C) 2024 RT Corporation <support@rt-net.jp>
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

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  pinMode(D12, OUTPUT);
  digitalWrite(D12, LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.printf(
    "\n\r LL2=%d LL1=%d LR1=%d LR2=%d", analogRead(LINE_L2_PIN), analogRead(LINE_L1_PIN),
    analogRead(LINE_R1_PIN), analogRead(LINE_R2_PIN));
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}

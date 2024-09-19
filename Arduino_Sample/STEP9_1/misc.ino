/*
 *    Arduino_Sample/STEP9_1/misc.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

void ledDrive(char times, int high_ms, int low_ms)
{
  for (char i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(high_ms);
    digitalWrite(LED_PIN, LOW);
    delay(low_ms);
  }
}

void buzzerDrive(char times, int high_ms, int low_ms)
{
  for (char i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);  // ブザー鳴らす
    delay(100);                      // 指定時間待ち
    digitalWrite(BUZZER_PIN, LOW);   // ブザー止める
    delay(100);                      // 指定時間待ち
  }
}

void runStop(void)
{
  digitalWrite(LED_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, 0);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  line_state = 0;
  while (1) {
    if (digitalRead(SW1_PIN) == LOW) {
      //      digitalWrite(LED_PIN, HIGH);
      break;
    }
  }
}

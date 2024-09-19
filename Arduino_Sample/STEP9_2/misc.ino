/*
 *    Arduino_Sample/STEP9_2/misc.ino
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

void adDebug()
{
  Serial.printf(
    "\n\r %d ML=%d,L2=%d L1=%d R1=%d,R2=%d,MR=%d", line_state, ml_value, l2_value, l1_value,
    r1_value, r2_value, mr_value);
}

void adGet(void)
{
  r2_value = analogRead(LINE_R2_PIN);
  r1_value = analogRead(LINE_R1_PIN);
  l1_value = analogRead(LINE_L1_PIN);
  l2_value = analogRead(LINE_L2_PIN) * 2;
  ml_value = adc_read_value(PB_1, 10);
  mr_value = adc_read_value(PB_0, 10);

  //床の反射率が一定ではないため、特定の箇所で調査し、平均化したオフセットを算出
  outside_offset = r2_value * 0.1281 - 17.245;
  inside_offset = r2_value * 0.1379 + 26.743;
}

int markerCheck(void)
{
  //      adDebug();
  if (
    (mr_value > (r2_value * 0.169 + 2.7488 + 30)) && (mr_value > 90)) {  //マーカーセンサアクティブ
    if (line_state == 0) {
      line_state = 1;
      return 1;
    } else if (line_state == 1) {
      line_state = 2;
      return 2;
    } else if (line_state == 2) {
      return 2;
    } else if (line_state == 3) {
      line_state = 7;
      return 7;
    } else if (line_state == 4) {
      line_state = 6;
      return 6;
    } else if (line_state == 5) {
      line_state = 6;
      return 6;
    } else if (line_state == 6) {
      line_state = 6;
      return 6;
    }
  }

  if (line_state == 1) {
    line_state = 3;
    return 3;
  }

  if (line_state == 2) {
    line_state = 3;
    return 3;
  }

  if (line_state == 6) {
    line_state = 3;
    return 3;
  }

  if (line_state == 3) {
    if ((r2_value + r1_value + l1_value + l2_value) > 2500) {  //クロスライン
      line_state = 4;                                          //クロスラインを検出
      digitalWrite(LED_PIN, HIGH);
      return 4;
    }
  }
  if (line_state == 4) {
    if ((r2_value + r1_value + l1_value + l2_value) < 2000) {
      line_state = 5;
      digitalWrite(LED_PIN, LOW);
      return 5;
    }
  }

  return line_state;
}

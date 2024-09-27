/*
 *    Arduino_Sample/STEP9_1/STEP9_1.ino
 *    
 *    Copyright(C) 2024 RT Corporation <support@rt-net.jp>
 *    Copyright(C) 2020 M.Hirai
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

//ピンの設定
#define DIR_R_PIN D10
#define DIR_L_PIN D0
#define PWM_R_PIN D11
#define PWM_L_PIN D12

#define SW1_PIN D7
#define SW2_PIN D8

#define POWER_PIN A6
#define LINE_L2_PIN A5
#define LINE_L1_PIN A4
#define LINE_R1_PIN A3
#define LINE_R2_PIN A2

#define LED_PIN D13
#define BUZZER_PIN D2

#define CW_R LOW
#define CCW_R HIGH
#define CW_L HIGH
#define CCW_L LOW

// 変数宣言
long line_control;
int inside_offset = 0;
int outside_offset = 0;
int l2_value = 0;
int l1_value = 0;
int r1_value = 0;
int r2_value = 0;
int ml_value = 0;
int mr_value = 0;

int pwm_r_value = 0;
int pwm_l_value = 0;

//ラインが白の場合:1、ラインが黒の場合-1
int line_signed = 1;

//0:マーカー検出、クロス検出がされていない
//1:Startマーカーを検出
//2:Startマーカーを通過中
//3:マーカー検出、クロス検出されてない
//4:クロスライン通過中
//5:マーカー検出、クロス検出されてない
//6:ダミーマーカー通過中
//7:ゴールマーカー検出
int line_state = 0;

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

void adDebug()
{
  Serial.printf(
    "\n\r %d ML=%d,L2=%d L1=%d R1=%d,R2=%d,MR=%d", line_state, ml_value, l2_value, l1_value,
    r1_value, r2_value, mr_value);
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

void setup()
{
  // put your setup code here, to run once:
  //IOポート設定
  pinMode(D12, OUTPUT);
  digitalWrite(D12, 0);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(DIR_R_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);

  ledDrive(2, 100, 100);
  Serial.begin(115200);

  while (1) {
    if (digitalRead(SW2_PIN) == LOW) {
      buzzerDrive(1, 100, 100);
      while (1) {
        adGet();
        Serial.printf(
          "\n\r VDD=%dmV LL2=%d LL1=%d LR1=%d LR2=%d inside_offset=%d outside_offset=%d ML=%d "
          "MR=%d xline=%d  marker_check=%d CTRL=%d",
          analogRead(POWER_PIN) * 9677 / 1000, l2_value, l1_value, r1_value, r2_value,
          (l1_value - r1_value), (l2_value - r2_value), adc_read_value(PB_1, 10),
          adc_read_value(PB_0, 10), l2_value + r1_value + l1_value + r2_value, markerCheck(),
          l1_value - r1_value - inside_offset + 2 * (l2_value - r2_value - outside_offset));
        delay(100);
        if (digitalRead(SW2_PIN) == LOW) {
          delay(200);
          break;
        }
      }
    }

    //左のスイッチを押したら、走行開始
    if (digitalRead(SW1_PIN) == LOW) {
      //      digitalWrite(LED_PIN, HIGH);
      adGet();
      buzzerDrive(2, 70, 70);
      break;
    }
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  adGet();
  //マーカーを検出?
  if (line_signed == 1) {
    markerCheck();
    if (line_state == 1) {
      buzzerDrive(1, 50, 50);
    } else if (line_state == 7) {
      buzzerDrive(2, 50, 50);
      runStop();
    }
  } else {
    if (adc_read_value(PB_0, 10) < 30) {
      runStop();
    }
  }
  //ラインセンサの値から制御量を算出する
  line_control = (l1_value - r1_value - inside_offset) + 2 * (l2_value - r2_value - outside_offset);

  pwm_l_value = 80 - line_signed * line_control / 10;
  pwm_r_value = 80 + line_signed * line_control / 10;
  //左モータPWM出力
  if (pwm_l_value < 0) {
    digitalWrite(DIR_L_PIN, CCW_L);  //モータ後進設定
  } else {
    digitalWrite(DIR_L_PIN, CW_L);  //モータ前進設定
  }
  pwm_l_value = abs(pwm_l_value);
  if (pwm_l_value > 255) {
    pwm_l_value = 255;  //モータ制御値上限ガード処理
  }
  if (pwm_l_value <= 0) {
    pwm_l_value = 0;  //モータ制御値下限ガード処理
  }
  analogWrite(PWM_L_PIN, pwm_l_value);

  //右モータPWM出力
  if (pwm_r_value < 0) {
    digitalWrite(DIR_R_PIN, CCW_R);  //モータ後進設定
  } else {
    digitalWrite(DIR_R_PIN, CW_R);  //モータ前進設定
  }
  pwm_r_value = abs(pwm_r_value);
  if (pwm_r_value > 255) {
    pwm_r_value = 255;  //モータ制御値上限ガード処理
  }
  if (pwm_r_value <= 0) {
    pwm_r_value = 0;  //モータ制御値下限ガード処理
  }
  analogWrite(PWM_R_PIN, pwm_r_value);

  delay(1);
}

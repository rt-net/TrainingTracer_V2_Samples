/*
 *    Arduino_Sample/STEP9_2/STEP9_2.ino
 *    
 *    Copyright(C) 2024 RT Corporation <support@rt-net.jp>
 *    Copyright(C) 2020 M.Hirai
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#include <HardwareTimer.h>
#include <Wire.h>

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

#define ENC_A_R_PIN A0
#define ENC_B_R_PIN A1
#define ENC_A_L_PIN D1
#define ENC_B_L_PIN D9

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

int time_tmp = 0;
int time_cnt = 0;
int sub_time;

//0:マーカー検出、クロス検出がされていない
//1:Startマーカーを検出
//2:Startマーカーを通過中
//3:マーカー検出、クロス検出されてない
//4:クロスライン通過中
//5:マーカー検出、クロス検出されてない
//6:ダミーマーカー通過中
//7:ゴールマーカー検出
int line_state = 0;

int encoder_r_cnt = 0;
int encoder_l_cnt = 0;

// センサの値を保存するグローバル関数

float theta = 0;

extern float zGyro;

void encoderA_R()
{
  if (digitalRead(ENC_A_R_PIN) == HIGH) {
    if (digitalRead(ENC_B_R_PIN) == LOW) {
      encoder_r_cnt++;
    } else {
      encoder_r_cnt--;
    }
  } else {
    if (digitalRead(ENC_B_R_PIN) == HIGH) {
      encoder_r_cnt++;
    } else {
      encoder_r_cnt--;
    }
  }
}

void encoderB_R()
{
  if (digitalRead(ENC_B_R_PIN) == HIGH) {
    if (digitalRead(ENC_A_R_PIN) == HIGH) {
      encoder_r_cnt++;
    } else {
      encoder_r_cnt--;
    }
  } else {
    if (digitalRead(ENC_A_R_PIN) == LOW) {
      encoder_r_cnt++;
    } else {
      encoder_r_cnt--;
    }
  }
}

void encoderA_L()
{
  if (digitalRead(ENC_A_L_PIN) == HIGH) {
    if (digitalRead(ENC_B_L_PIN) == LOW) {
      encoder_l_cnt++;
    } else {
      encoder_l_cnt--;
    }
  } else {
    if (digitalRead(ENC_B_L_PIN) == HIGH) {
      encoder_l_cnt++;
    } else {
      encoder_l_cnt--;
    }
  }
}

void encoderB_L()
{
  if (digitalRead(ENC_B_L_PIN) == HIGH) {
    if (digitalRead(ENC_A_L_PIN) == HIGH) {
      encoder_l_cnt++;
    } else {
      encoder_l_cnt--;
    }
  } else {
    if (digitalRead(ENC_A_L_PIN) == LOW) {
      encoder_l_cnt++;
    } else {
      encoder_l_cnt--;
    }
  }
}

void timerCnt(void) { time_cnt++; }

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

  pinMode(ENC_A_R_PIN, INPUT);
  pinMode(ENC_B_R_PIN, INPUT);
  pinMode(ENC_A_L_PIN, INPUT);
  pinMode(ENC_B_L_PIN, INPUT);

  attachInterrupt(ENC_A_R_PIN, encoderA_R, CHANGE);
  attachInterrupt(ENC_B_R_PIN, encoderB_R, CHANGE);
  attachInterrupt(ENC_A_L_PIN, encoderA_L, CHANGE);
  attachInterrupt(ENC_B_L_PIN, encoderB_L, CHANGE);

  Wire.begin();
  BMX055_Init();
  delay(300);

  TIM_TypeDef * Instance = TIM2;
  HardwareTimer * Timer2 = new HardwareTimer(Instance);
  Timer2->setMode(1, TIMER_OUTPUT_COMPARE);
  Timer2->setOverflow(100, MICROSEC_FORMAT);
  Timer2->attachInterrupt(timerCnt);
  Timer2->resume();

  ledDrive(2, 100, 100);
  Serial.begin(115200);

  while (1) {
    if (digitalRead(SW2_PIN) == LOW) {
      buzzerDrive(1, 100, 100);
      time_tmp = time_cnt;
      while (1) {
        adGet();
        BMX055_Gyro();
        sub_time = time_cnt - time_tmp;
        theta = theta + zGyro * (sub_time + 1000) / 10000;

        Serial.print(sub_time);
        Serial.print(" ");
        Serial.print(theta);
        Serial.print(" ");
        Serial.println(zGyro);
        /*        
        Serial.printf("\n\r VDD=%dmV LL2=%d LL1=%d LR1=%d LR2=%d inside_offset=%d outside_offset=%d ML=%d MR=%d xline=%d  marker_check=%d CTRL=%d ENC_R=%d ENC_L=%d theta=%d zGyro=%d time_cnt=%d",
                      analogRead(POWER_PIN) * 9677 / 1000,
                      l2_value, l1_value, r1_value, r2_value,
                      (l1_value - r1_value), (l2_value - r2_value),
                      adc_read_value(PB_1, 10), adc_read_value(PB_0, 10),
                      l2_value + r1_value + l1_value + r2_value,
                      markerCheck(),
                      l1_value - r1_value - inside_offset + 2 * (l2_value - r2_value - outside_offset),
                      encoder_r_cnt,
                      encoder_l_cnt,
                      (int)(theta*10),
                      (int)(zGyro*10),
                      time_cnt
                     );
*/
        delay(100);
        time_tmp = time_cnt;
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

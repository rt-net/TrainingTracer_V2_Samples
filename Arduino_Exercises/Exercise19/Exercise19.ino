/*
 *    Arduino_Exercises/Exercise19/Exercise19.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
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

#define LINE_L2_PIN A5
#define LINE_L1_PIN A4
#define LINE_R1_PIN A3
#define LINE_R2_PIN A2

#define LED_PIN D13
#define BUZZER_PIN D2

#define CW_R  0
#define CCW_R 1
#define CW_L 1
#define CCW_L 0

// 変数宣言
long line_control;
int inside_offset = -34;
int outside_offset = -146;

int pwm_r_value = 0;
int pwm_l_value = 0;

//ラインが白の場合:1、ラインが黒の場合-1
int line_signed = -1;

int counter = 0;

void setup() {
  // put your setup code here, to run once:
  //IOポート設定
  pinMode(D12,OUTPUT);
  digitalWrite(D12,LOW);
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(DIR_R_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);

  Serial.begin(115200);

  while (1) {

    if (digitalRead(SW2_PIN) == LOW) {
      digitalWrite(BUZZER_PIN, HIGH);   // ブザー鳴らす
      delay(100);               // 指定時間待ち
      digitalWrite(BUZZER_PIN, LOW);    // ブザー止める
      delay(100);              // 指定時間待ち
      while (1) {
        Serial.printf("\n\r LL2=%d LL1=%d LR1=%d LR2=%d inside_offset=%d outside_offset=%d ML=%d MR=%d",
                      analogRead(LINE_L2_PIN), analogRead(LINE_L1_PIN), analogRead(LINE_R1_PIN), analogRead(LINE_R2_PIN),
                      (analogRead(LINE_L1_PIN) - analogRead(LINE_R1_PIN)), (analogRead(LINE_L2_PIN) - analogRead(LINE_R2_PIN)),
                      adc_read_value(PB_1, 10), adc_read_value(PB_0, 10));
        delay(100);
        if (digitalRead(SW2_PIN) == LOW) {
          delay(200);
          break;
        }
      }
    }

    //左のスイッチを押したら、走行開始
    if (digitalRead(SW1_PIN) == LOW) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);   // ブザー鳴らす
      delay(70);               // 指定時間待ち
      digitalWrite(BUZZER_PIN, LOW);    // ブザー止める
      delay(70);              // 指定時間待ち
      digitalWrite(BUZZER_PIN, HIGH);   // ブザー鳴らす
      delay(70);               // 指定時間待ち
      digitalWrite(BUZZER_PIN, LOW);    // ブザー止める
      delay(70);              // 指定時間待ち
      break;
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:

  //マーカーを検出?
  if (line_signed == 1 ) {
    if (adc_read_value(PB_0, 10) > 80) {
      while(adc_read_value(PB_0, 10) > 60);
      counter++;
      if (counter > 1){
        digitalWrite(LED_PIN, LOW);
        analogWrite(PWM_L_PIN, 0);
        analogWrite(PWM_R_PIN, 0);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        while (1) {
          if (digitalRead(SW1_PIN) == LOW) {
            digitalWrite(LED_PIN, HIGH);
            break;
          }
        }
        counter = 0;
      }
      
    }
  } else {
    if (adc_read_value(PB_0, 10) < 30) {
      while(adc_read_value(PB_0, 10) < 45);
      counter++;
      if (counter > 1){
        digitalWrite(LED_PIN, LOW);
        analogWrite(PWM_L_PIN, 0);
        analogWrite(PWM_R_PIN, 0);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        while (1) {
          if (digitalRead(SW1_PIN) == LOW) {
            digitalWrite(LED_PIN, HIGH);
            break;
          }
        }
        counter = 0;
      }
    }
  }
  //ラインセンサの値から制御量を算出する
  line_control = (analogRead(LINE_L1_PIN) - analogRead(LINE_R1_PIN) - inside_offset)
                  + 2 * (analogRead(LINE_L2_PIN) - analogRead(LINE_R2_PIN) - outside_offset);

  pwm_l_value = 80 - line_signed * line_control / 10;
  pwm_r_value = 80 + line_signed * line_control / 10;
  //左モータPWM出力
  if (pwm_l_value < 0) {
    digitalWrite(DIR_L_PIN, CCW_L);//モータ後進設定
  } else {
    digitalWrite(DIR_L_PIN, CW_L);//モータ前進設定
  }
  pwm_l_value = abs(pwm_l_value);
  if (pwm_l_value > 255) {
    pwm_l_value = 255; //モータ制御値上限ガード処理
  }
  if (pwm_l_value <= 0) {
    pwm_l_value = 0; //モータ制御値下限ガード処理
  }
  analogWrite(PWM_L_PIN, pwm_l_value);

  //右モータPWM出力
  if (pwm_r_value < 0) {
    digitalWrite(DIR_R_PIN, CCW_R);//モータ後進設定
  } else {
    digitalWrite(DIR_R_PIN, CW_R);//モータ前進設定
  }
  pwm_r_value = abs(pwm_r_value);
  if (pwm_r_value > 255) {
    pwm_r_value = 255; //モータ制御値上限ガード処理
  }
  if (pwm_r_value <= 0) {
    pwm_r_value = 0; //モータ制御値下限ガード処理
  }
  analogWrite(PWM_R_PIN, pwm_r_value);

  delay(1);
  //PWM、コントロールの値確認用
  //  Serial.printf("\n\r PWM_R=%d PWM_L=%d CONT=%d",pwm_r_value,pwm_l_value,line_control);
  //  delay(100);
}

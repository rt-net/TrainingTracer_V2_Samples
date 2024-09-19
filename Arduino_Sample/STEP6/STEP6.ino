/*
 *    Arduino_Sample/STEP6/STEP6.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#define DIR_R_PIN D10
#define DIR_L_PIN D0
#define SW1_PIN D7
#define SW2_PIN D8
#define PWM_R_PIN D11
#define PWM_L_PIN D12
#define CW_R 0
#define CCW_R 1
#define CW_L 1
#define CCW_L 0

int pwm = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(D12, OUTPUT);
  digitalWrite(D12, 0);

  pinMode(DIR_R_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);

  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);

  digitalWrite(DIR_R_PIN, CW_R);
  digitalWrite(DIR_L_PIN, CW_L);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(SW1_PIN) == LOW) {
    delay(200);
    pwm += 10;
    if (pwm > 255) {
      pwm = 255;
    }
    analogWrite(PWM_R_PIN, pwm);
    analogWrite(PWM_L_PIN, pwm);
  }
  if (digitalRead(SW2_PIN) == LOW) {
    delay(200);
    pwm -= 10;
    if (pwm < 0) {
      pwm = 0;
    }
    analogWrite(PWM_R_PIN, pwm);
    analogWrite(PWM_L_PIN, pwm);
  }
}

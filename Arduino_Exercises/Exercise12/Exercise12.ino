/*
 *    Arduino_Exercises/Exercise12/Exercise12.ino
 *    
 *    Copyright(C) 2024 RT Corporation <support@rt-net.jp>
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
#define CW_R LOW
#define CCW_R HIGH
#define CW_L HIGH
#define CCW_L LOW
#define BUZZER_PIN D2
#define LED_PIN D13

int pwm = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(D12, OUTPUT);
  digitalWrite(D12, LOW);

  pinMode(DIR_R_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);

  digitalWrite(DIR_R_PIN, CW_R);
  digitalWrite(DIR_L_PIN, CW_L);

  digitalWrite(LED_PIN, HIGH);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(SW1_PIN) == LOW) {
    delay(200);
    pwm += 10;
    if (pwm > 255) {
      pwm = 255;
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    analogWrite(PWM_R_PIN, pwm);
    analogWrite(PWM_L_PIN, pwm);
  }
  if (digitalRead(SW2_PIN) == LOW) {
    delay(200);
    pwm -= 10;
    if (pwm < 0) {
      pwm = 0;
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }
    analogWrite(PWM_R_PIN, pwm);
    analogWrite(PWM_L_PIN, pwm);
  }
}

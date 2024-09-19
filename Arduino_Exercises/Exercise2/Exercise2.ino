/*
 *    Arduino_Exercises/Exercise2/Exercise2.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#define LED_PIN D13
#define BUZZER_PIN D2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(D12,OUTPUT);
  digitalWrite(D12,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN,LOW);//LEDを消灯
  digitalWrite(BUZZER_PIN,HIGH);
  delay(1000);                //1秒待つ
  digitalWrite(LED_PIN,HIGH);//LED点灯
  digitalWrite(BUZZER_PIN,LOW);
  delay(1000);                //1秒待つ
}

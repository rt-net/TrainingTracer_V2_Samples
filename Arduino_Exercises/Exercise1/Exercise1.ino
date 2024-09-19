/*
 *    Arduino_Exercises/Exercise1/Exercise1.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#define LED_PIN D13

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT);
  pinMode(D12,OUTPUT);
  digitalWrite(D12,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN,HIGH); //LEDを点灯
  delay(500);                //0.5秒待つ
  digitalWrite(LED_PIN,LOW);  //LED消灯
  delay(500);                //0.5秒待つ
}

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;
#define SDA_PIN 21
#define SCL_PIN 22

void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void spinL(int L, int R);
void spinR(int L, int R);
void stop(void);

///////////////////set pinMode/////////////////////////
const int MOTOR_L1 = 13;
const int MOTOR_L2 = 27;
const int MOTOR_R1 = 25;
const int MOTOR_R2 = 26;
///////////////////////////////////////////////////////

void setup() {
  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!ads1.begin(0x48))  //讀取左邊ADC
  {
    Serial.println("Failed to initialize ADS1.");
    while (1);
  }
  if (!ads2.begin(0x49))  //讀取右邊ADC
  {
    Serial.println("Failed to initialize ADS2.");
    while (1);
  }
  delay(2500);
} 

void loop() {
  if (ads2.readADC_SingleEnded(0) >= 500 || ads1.readADC_SingleEnded(1) >= 500 || ads1.readADC_SingleEnded(2) >= 500) {
    front(255);
  }  
  else if (ads2.readADC_SingleEnded(2) >= 100 || ads2.readADC_SingleEnded(3) >= 100) {
    stop();
    back(240);
    delay(400);
    spinL(150, 150);
    delay(200);
  }
  /*else if (ads1.readADC_SingleEnded(1) >= 500) {
    front(225);
  }
  else if (ads1.readADC_SingleEnded(2) >= 500) {
    front(225);
  }*/
  else if (ads1.readADC_SingleEnded(0) >= 500) {
    spinR(175, 175);
  }
  else if (ads1.readADC_SingleEnded(3) >= 500) {
    spinL(175, 175);
  }
  else {
    front(150);
  }
}

void front(int spd){
  analogWrite(MOTOR_R1,spd);
  analogWrite(MOTOR_R2,0);
  analogWrite(MOTOR_L1,spd);
  analogWrite(MOTOR_L2,0);
}
void back(int spd){
  analogWrite(MOTOR_R1,0);
  analogWrite(MOTOR_R2,spd);
  analogWrite(MOTOR_L1,0);
  analogWrite(MOTOR_L2,spd);
}
void stop(){
  analogWrite(MOTOR_R1,255);
  analogWrite(MOTOR_R2,255);
  analogWrite(MOTOR_L1,255);
  analogWrite(MOTOR_L2,255);
}
void turnLR(int L, int R){
  analogWrite(MOTOR_R1,R);
  analogWrite(MOTOR_R2,0);
  analogWrite(MOTOR_L1,L);
  analogWrite(MOTOR_L2,0);
}
void spinR(int L, int R){
  analogWrite(MOTOR_R1,0);
  analogWrite(MOTOR_R2,R);
  analogWrite(MOTOR_L1,L);
  analogWrite(MOTOR_L2,0);
}
void spinL(int L, int R){
  analogWrite(MOTOR_R1,R);
  analogWrite(MOTOR_R2,0);
  analogWrite(MOTOR_L1,0);
  analogWrite(MOTOR_L2,L);
}
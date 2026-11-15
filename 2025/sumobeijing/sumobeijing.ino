#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads;
#define SDA_PIN 21
#define SCL_PIN 22

void frontFR(int F, int R);
void backLR(int L, int R);
void turnLR(int L, int R);
void spinL(int L, int R);
void spinR(int L, int R);
void stop(void);

int16_t adcL, adcR;

/////////////////////定義腳位///////////////////////////
const int MOTOR_AL1 = 13;
const int MOTOR_AL2 = 27;
const int MOTOR_AR1 = 25;
const int MOTOR_AR2 = 26;
const int MOTOR_BL1 = 18;
const int MOTOR_BL2 = 4;
const int MOTOR_BR1 = 19;
const int MOTOR_BR2 = 23;
const int IR_ML = 34;
const int IR_MM = 39;
const int IR_MR = 36;
const int IR_L  = 35;
const int IR_R  = 32;
const int COLOR_L = 16;
const int COLOR_R = 17;
///////////////////////////////////////////////////////

void setup() {
  pinMode(MOTOR_AL1, OUTPUT);
  pinMode(MOTOR_AL2, OUTPUT);
  pinMode(MOTOR_AR1, OUTPUT);
  pinMode(MOTOR_AR2, OUTPUT);
  pinMode(MOTOR_BL1, OUTPUT);
  pinMode(MOTOR_BL2, OUTPUT);
  pinMode(MOTOR_BR1, OUTPUT);
  pinMode(MOTOR_BR2, OUTPUT);
  pinMode(IR_ML,INPUT);
  pinMode(IR_MM,INPUT);
  pinMode(IR_MR,INPUT);
  pinMode(IR_L,INPUT);
  pinMode(IR_R,INPUT);
  pinMode(COLOR_L,INPUT);
  pinMode(COLOR_R,INPUT);
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  spinL(255,255);
  delay(650);
} 

void loop() {
  if (ads.readADC_SingleEnded(0) >= 650 || ads.readADC_SingleEnded(1) >= 650){
    stop();
    back(150,175);
    delay(450);
    spinL(150,150);
    delay(250);
  }
  else if  (analogRead(IR_MM)>=1500){
    //Serial.print("IR_MM = ");
    //Serial.println(analogRead(IR_MM));
    frontFR(255,255);
  }
  
  else if (analogRead(IR_MR)>=700){
    //Serial.print("IR_MR = ");
    //Serial.println(analogRead(IR_MR));
    turnLR(175,0);
  }
  else if (analogRead(IR_ML)>=700){
    //Serial.print("IR_ML = ");
    //Serial.println(analogRead(IR_ML));
    turnLR(0,175);
  }
  else if (analogRead(IR_R)>=1000){
    //Serial.print("IR_R = ");
    //Serial.println(analogRead(IR_R));
    turnLR(175,0);
  }
  else if (analogRead(IR_L)>=1000){
    //Serial.print("IR_L = ");
    //Serial.println(analogRead(IR_L));
    turnLR(0,175);
  }
  else {
    frontFR(170,170);
  }

}

void frontFR(int F, int R){
  analogWrite(MOTOR_AR1,F);
  analogWrite(MOTOR_AR2,0);
  analogWrite(MOTOR_AL1,F);
  analogWrite(MOTOR_AL2,0);
  analogWrite(MOTOR_BR1,R);
  analogWrite(MOTOR_BR2,0);
  analogWrite(MOTOR_BL1,R);
  analogWrite(MOTOR_BL2,0);
}

void back(int L, int R){
  analogWrite(MOTOR_AR1,0);
  analogWrite(MOTOR_AR2,R);
  analogWrite(MOTOR_AL1,0);
  analogWrite(MOTOR_AL2,L);
  analogWrite(MOTOR_BR1,0);
  analogWrite(MOTOR_BR2,R);
  analogWrite(MOTOR_BL1,0);
  analogWrite(MOTOR_BL2,L);

}
void stop(){
  analogWrite(MOTOR_AR1,255);
  analogWrite(MOTOR_AR2,255);
  analogWrite(MOTOR_AL1,255);
  analogWrite(MOTOR_AL2,255);
  analogWrite(MOTOR_BR1,255);
  analogWrite(MOTOR_BR2,255);
  analogWrite(MOTOR_BL1,255);
  analogWrite(MOTOR_BL2,255);

}

void turnLR(int L, int R){
  analogWrite(MOTOR_AR1,R);
  analogWrite(MOTOR_AR2,0);
  analogWrite(MOTOR_AL1,L);
  analogWrite(MOTOR_AL2,0);
  analogWrite(MOTOR_BR1,R);
  analogWrite(MOTOR_BR2,0);
  analogWrite(MOTOR_BL1,L);
  analogWrite(MOTOR_BL2,0);
}

void spinL(int L, int R){
  analogWrite(MOTOR_AR1,0);
  analogWrite(MOTOR_AR2,R);
  analogWrite(MOTOR_AL1,L);
  analogWrite(MOTOR_AL2,0);
  analogWrite(MOTOR_BR1,0);
  analogWrite(MOTOR_BR2,R);
  analogWrite(MOTOR_BL1,L);
  analogWrite(MOTOR_BL2,0);
}

void spinR(int L, int R){
  analogWrite(MOTOR_AR1,R);
  analogWrite(MOTOR_AR2,0);
  analogWrite(MOTOR_AL1,0);
  analogWrite(MOTOR_AL2,L);
  analogWrite(MOTOR_BR1,R);
  analogWrite(MOTOR_BR2,0);
  analogWrite(MOTOR_BL1,0);
  analogWrite(MOTOR_BL2,L);
}

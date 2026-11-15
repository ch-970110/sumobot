#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads;
#define SDA_PIN 21
#define SCL_PIN 22

unsigned long previousMillis = 0;
void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void spinL(int L, int R);
void spinR(int L, int R);
void stop(void);
bool turnLeft = true; //true = turn left

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
} 

void loop() {
  unsigned long currentMillis = millis();
  if (ads.readADC_SingleEnded(0) >= 650 || ads.readADC_SingleEnded(1) >= 650){
    stop();
    back(175);
    delay(450);
    spinL(150,150);
    delay(250);
  }
  else if  (analogRead(IR_MM)>=1500){
    front(255,255);
  }
  else if (analogRead(IR_MR)>=700){
    turnLR(175,0);
  }
  else if (analogRead(IR_ML)>=700){
    turnLR(0,175);
  }
  else if (analogRead(IR_R)>=1000){
    turnLR(175,0);
  }
  else if (analogRead(IR_L)>=1000){
    turnLR(0,175);
  }
  else {
    if (currentMillis - previousMillis >= 200) {
      previousMillis = currentMillis;
      if (turnLeft) {
        spinL(100, 100);
        delay(100);
        stop();
        turnLeft = false;
      } 
      else {
        spinR(100, 100);
        delay(100);
        stop();
        turnLeft = true;
      }
  }

}

void front(int spd){
  analogWrite(MOTOR_AR1,spd);
  analogWrite(MOTOR_AR2,0);
  analogWrite(MOTOR_AL1,spd);
  analogWrite(MOTOR_AL2,0);
  analogWrite(MOTOR_BR1,spd);
  analogWrite(MOTOR_BR2,0);
  analogWrite(MOTOR_BL1,spd);
  analogWrite(MOTOR_BL2,0);
}

void back(int spd){
  analogWrite(MOTOR_AR1,0);
  analogWrite(MOTOR_AR2,spd);
  analogWrite(MOTOR_AL1,0);
  analogWrite(MOTOR_AL2,spd);
  analogWrite(MOTOR_BR1,0);
  analogWrite(MOTOR_BR2,spd);
  analogWrite(MOTOR_BL1,0);
  analogWrite(MOTOR_BL2,spd);

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

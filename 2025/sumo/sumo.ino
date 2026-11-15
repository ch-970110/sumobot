#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;
#define SDA_PIN 21
#define SCL_PIN 22

unsigned long previousMillis = 0;
void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void spinL(int L, int R);
void spinR(int L, int R);
void stop(void);
bool turnLeftNext = true; //true = turn left
enum targetDirection {NONE, LEFT, RIGHT, CENTRE, R, L};
targetDirection dir = NONE;

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
  delay(3000);
} 

bool line() {
  if (ads2.readADC_SingleEnded(2) >= 100 || ads2.readADC_SingleEnded(3) >= 100) {
    return true;
  }
  return false;
}

targetDirection targetDetection() {
  int IR_MM = ads2.readADC_SingleEnded(0);
  int IR_MR = ads1.readADC_SingleEnded(1);
  int IR_ML = ads1.readADC_SingleEnded(2);
  int IR_R = ads1.readADC_SingleEnded(0);
  int IR_L = ads1.readADC_SingleEnded(3);

  if (IR_MM >= 500) return CENTRE;
  if (IR_MR >= 500) return RIGHT;
  if (IR_ML >= 500) return LEFT;
  if (IR_R >= 500) return R;
  if (IR_L >= 500) return L;
  
  return NONE;
}

void loop() {
  unsigned long currentMillis = millis();
  Serial.print(ads1.readADC_SingleEnded(3));
  Serial.print("||");
  Serial.print(ads1.readADC_SingleEnded(2));
  Serial.print("||");
  Serial.print(ads2.readADC_SingleEnded(0));
  Serial.print("||");
  Serial.print(ads1.readADC_SingleEnded(1));
  Serial.print("||");
  Serial.println(ads1.readADC_SingleEnded(0));
  Serial.print(ads2.readADC_SingleEnded(3));
  Serial.print("||");
  Serial.println(ads2.readADC_SingleEnded(2));
  delay(100);
  targetDirection dir = targetDetection();
  if (line() == true){
    stop();
    back(200);
    delay(400);
    spinL(150, 150);
    delay(250);
  }  
  else if (dir == CENTRE) {
    front(255);
  }
  else if (dir == RIGHT) {
    turnLR(175, 0);
  }
  else if (dir == LEFT) {
    turnLR(0, 175);
  }
  else if (dir == R) {
    spinL(175, 175);
  }
  else if (dir == L) {
    spinR(175, 175);
  }
  else {
    front(100);
    /*if (turnLeftNext) {
      while (millis() - currentMillis < 300 && !line() && targetDetection() == NONE) {
        spinL(150, 150);
      }
      stop();
      turnLeftNext = false;
    }
    else {
      while (millis() - currentMillis < 300 && !line() && targetDetection() == NONE) {
        spinR(150, 150);
      }
      stop();
      turnLeftNext = true;
    }*/
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
void spinL(int L, int R){
  analogWrite(MOTOR_R1,0);
  analogWrite(MOTOR_R2,R);
  analogWrite(MOTOR_L1,L);
  analogWrite(MOTOR_L2,0);
}
void spinR(int L, int R){
  analogWrite(MOTOR_R1,R);
  analogWrite(MOTOR_R2,0);
  analogWrite(MOTOR_L1,0);
  analogWrite(MOTOR_L2,L);
}
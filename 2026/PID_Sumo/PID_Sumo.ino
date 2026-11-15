#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;
#define SDA_PIN 21
#define SCL_PIN 22
#define P_in 0.5
#define P_out 0.7


void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void spinL(int L, int R);
void spinR(int L, int R);
void stop(void);

float FLM(int, int, int, int);
float FRM(int, int, int, int);
float BLM(int, int, int, int);
float BRM(int, int, int, int);
int IR_M, IR_MR, IR_ML, IR_L, IR_R, IR_B;

int MOTOR_FL1 = 27;
int MOTOR_FL2 = 13;
int MOTOR_FR1 = 26;
int MOTOR_FR2 = 25;
int MOTOR_BL1 = 4;
int MOTOR_BL2 = 18;
int MOTOR_BR1 = 19;
int MOTOR_BR2 = 23;

void setup() {
  pinMode(MOTOR_FL1, OUTPUT);
  pinMode(MOTOR_FL2, OUTPUT);
  pinMode(MOTOR_FR1, OUTPUT);
  pinMode(MOTOR_FR2, OUTPUT);
  pinMode(MOTOR_BL1, OUTPUT);
  pinMode(MOTOR_BL2, OUTPUT);
  pinMode(MOTOR_BR1, OUTPUT);
  pinMode(MOTOR_BR2, OUTPUT);
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!ads1.begin(0x48)) 
  {
    Serial.println("Failed to initialize ADS1.");
    while (1);
  }
  if (!ads2.begin(0x49)) 
  {
    Serial.println("Failed to initialize ADS2.");
    while (1);
  }
  //delay(3000);
}

void loop() {
  Serial.print(ads1.readADC_SingleEnded(3));
  Serial.print(" | ");
  Serial.print(ads1.readADC_SingleEnded(2));
  Serial.print(" | ");
  Serial.print(ads2.readADC_SingleEnded(0));
  Serial.print(" | ");
  Serial.print(ads1.readADC_SingleEnded(1));
  Serial.print(" | ");
  Serial.println(ads1.readADC_SingleEnded(0));
  /*int IR_M = mapValue(ads2.readADC_SingleEnded(0), 22, 1047);
  int IR_MR = mapValue(ads1.readADC_SingleEnded(1), 57, 1070);
  int IR_ML = mapValue(ads1.readADC_SingleEnded(2), 14, 1053);
  int IR_R = mapValue(ads1.readADC_SingleEnded(0), 25, 1043);
  int IR_L = mapValue(ads1.readADC_SingleEnded(3), 41, 1052);*/
  if (ads2.readADC_SingleEnded(2) >= 100 || ads2.readADC_SingleEnded(3) >= 100) {
    stop();
    back(180);
    delay(300);
    turnLR(150, -150);
    delay(200);
  }
  else {
    turnLR((int)LM(IR_L, IR_ML, IR_MR, IR_R), (int)RM(IR_L, IR_ML, IR_MR, IR_R));
  }
  /*Serial.print((int)LM(IR_L, IR_ML, IR_MR, IR_R));
  Serial.print(" || ");
  Serial.println((int)RM(IR_L, IR_ML, IR_MR, IR_R));*/
}

int mapValue(int rawData, int min_input, int max_input) {
  return (rawData - min_input) * 255 / (max_input - min_input);
}

float LM(int L1, int L2, int R2, int R1) {
  float lm;
  lm = ((L1 * P_out) + (L2 * P_in)) - ((R2 * P_in) + (R1 * P_out));
  return lm;
}

float RM(int L1, int L2, int R2, int R1) {
  float rm;
  rm = ((R2 * P_in) + (R1 * P_out)) - ((L2 * P_in) + (L1 * P_out));
  return rm;
}

void front(int spd){
  analogWrite(MOTOR_R1,spd);
  analogWrite(MOTOR_R2,0);
  analogWrite(MOTOR_FL1,spd);
  analogWrite(MOTOR_FL2,0);
B}
Bvoid back(int spd){
  analogWrite(MOTOR_R1,0);
  analogWrite(MOTOR_R2,spd);
  analogWrite(MOTOR_FL1,0);
  analogWrite(MOTOR_FL2,spd);
B}
Bvoid stop(){
  analogWrite(MOTOR_R1,255);
  analogWrite(MOTOR_R2,255);
  analogWrite(MOTOR_FL1,255);
  analogWrite(MOTOR_FL2,255);
B}
Bvoid turnLR(int L, int R) {
  if (L>=0) {
    analogWrite(MOTOR_FL1, L);
    analogWrite(MOTOR_FL2, 0);
  B}
  Belse {
    analogWrite(MOTOR_FL1, 0);
    analogWrite(MOTOR_FL2, L * -1);
  B}
  Bif (R>=0) {
    analogWrite(MOTOR_R1, R);
    analogWrite(MOTOR_R2, 0);
  }
  else {
    analogWrite(MOTOR_R1, 0);
    analogWrite(MOTOR_R2, R * -1);
  }
}
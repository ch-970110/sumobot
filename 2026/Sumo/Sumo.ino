#include <dummy.h>

#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;
#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_PIN 33
/*#define P_in 0.5
#define P_out 0.7*/

void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void stop(void);

/*float FLM(int, int, int, int);
float FRM(int, int, int, int);
float BLM(int, int, int, int);
float BRM(int, int, int, int);*/
//int IR_M, IR_MR, IR_ML, IR_L, IR_R, IR_B;

int MOTOR_FL1 = 27;
int MOTOR_FL2 = 13;
int MOTOR_FR1 = 26;
int MOTOR_FR2 = 25;
int MOTOR_BL1 = 18;
int MOTOR_BL2 = 4;
int MOTOR_BR1 = 23;
int MOTOR_BR2 = 19;

void setup() {
  pinMode(MOTOR_FL1, OUTPUT);
  pinMode(MOTOR_FL2, OUTPUT);
  pinMode(MOTOR_FR1, OUTPUT);
  pinMode(MOTOR_FR2, OUTPUT);
  pinMode(MOTOR_BL1, OUTPUT);
  pinMode(MOTOR_BL2, OUTPUT);
  pinMode(MOTOR_BR1, OUTPUT);
  pinMode(MOTOR_BR2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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
  Serial.println("Waiting for button press to start...");
  while (digitalRead(BUTTON_PIN) == LOW) {
    // This loop runs endlessly doing nothing until the button is pressed (goes LOW)
    delay(10); 
  }
  
  Serial.println("Button pressed! Starting robot in 3 second...");
  delay(3000); 
  front(70);
  delay(200);// Gives you 3 second to move your hand away from the robot
  /*while (ads1.readADC_SingleEnded(3) <= 1300) {
    back(40);
  }
  stop();*/
}

void loop() {
  /*Serial.print("LIR: ");
  Serial.print(ads2.readADC_SingleEnded(3));
  Serial.print(" | MLIR: ");
  Serial.print(ads1.readADC_SingleEnded(2));
  Serial.print(" | MIR: ");
  Serial.print(ads1.readADC_SingleEnded(1));
  Serial.print(" | MRIR: ");
  Serial.print(ads1.readADC_SingleEnded(0));
  Serial.print(" | RIR: ");
  Serial.println(ads2.readADC_SingleEnded(0));
  Serial.print("LC: ");
  Serial.print(ads2.readADC_SingleEnded(2));
  Serial.print(" | RC: ");
  Serial.println(ads2.readADC_SingleEnded(1));
  Serial.print("BIR: ");
  Serial.print(ads1.readADC_SingleEnded(3));
  /*Serial.print(" | BC: ");
  Serial.println(ads1.readADC_SingleEnded(3));
  int IR_M = mapValue(ads2.readADC_SingleEnded(0), 22, 1047);
  int IR_MR = mapValue(ads1.readADC_SingleEnded(1), 57, 1070);
  int IR_ML = mapValue(ads1.readADC_SingleEnded(2), 14, 1053);
  int IR_R = mapValue(ads1.readADC_SingleEnded(0), 25, 1043);
  int IR_L = mapValue(ads1.readADC_SingleEnded(3), 41, 1052);*/
  if (ads2.readADC_SingleEnded(2) >= 1300 || ads2.readADC_SingleEnded(1) >= 1300) {
    //Serial.println("Line");
    stop();
    back(150);
    delay(400);
    turnLR(90, -90);
    delay(400);
  }  else if (ads1.readADC_SingleEnded(1) >= 500) {
    Serial.println("Kill");
    front(255);
  } else if (ads1.readADC_SingleEnded(2) >= 450) { 
    Serial.println("Left");
    turnLR(-60, 60);
  } else if (ads1.readADC_SingleEnded(0) >= 450) {
    Serial.println("Right");
    turnLR(60, -60);
  } else if (ads2.readADC_SingleEnded(3) >= 450) { 
    Serial.println("Left");
    turnLR(-90, 90);
  } else if (ads2.readADC_SingleEnded(0) >= 450) {
    Serial.println("Right");
    turnLR(90, -90);
  } else if (ads1.readADC_SingleEnded(3) >= 700) { // || ads2.readADC_SingleEnded(1)
    Serial.println("Back");
    turnLR(100, 0);
    delay(200);
    turnLR(0, 100);
    delay(200);
  }else {
    Serial.println("N/A");
    front(100);
  }
  //delay(500);
  /*Serial.print((int)LM(IR_L, IR_ML, IR_MR, IR_R));
  Serial.print(" || ");
  Serial.println((int)RM(IR_L, IR_ML, IR_MR, IR_R));*/
}

/*int mapValue(int rawData, int min_input, int max_input) {
  return (rawData - min_input) * 255 / (max_input - min_input);
}*/

/*float LM(int L1, int L2, int R2, int R1) {
  float lm;
  lm = ((L1 * P_out) + (L2 * P_in)) - ((R2 * P_in) + (R1 * P_out));
  return lm;
}

float RM(int L1, int L2, int R2, int R1) {
  float rm;
  rm = ((R2 * P_in) + (R1 * P_out)) - ((L2 * P_in) + (L1 * P_out));
  return rm;
}*/



void front(int spd){
  analogWrite(MOTOR_FR1,spd);
  analogWrite(MOTOR_FR2,0);
  analogWrite(MOTOR_FL1,spd);
  analogWrite(MOTOR_FL2,0);
  analogWrite(MOTOR_BR1,spd);
  analogWrite(MOTOR_BR2,0);
  analogWrite(MOTOR_BL1,spd);
  analogWrite(MOTOR_BL2,0);
}

void back(int spd){
  analogWrite(MOTOR_FR1,0);
  analogWrite(MOTOR_FR2,spd);
  analogWrite(MOTOR_FL1,0);
  analogWrite(MOTOR_FL2,spd);
  analogWrite(MOTOR_BR1,0);
  analogWrite(MOTOR_BR2,spd);
  analogWrite(MOTOR_BL1,0);
  analogWrite(MOTOR_BL2,spd);
}

void stop(){
  analogWrite(MOTOR_FR1,255);
  analogWrite(MOTOR_FR2,255);
  analogWrite(MOTOR_FL1,255);
  analogWrite(MOTOR_FL2,255);
  analogWrite(MOTOR_BR1,255);
  analogWrite(MOTOR_BR2,255);
  analogWrite(MOTOR_BL1,255);
  analogWrite(MOTOR_BL2,255);
}

void turnLR(int L, int R) {
  if (L >= 0) {
    analogWrite(MOTOR_FL1, L);
    analogWrite(MOTOR_FL2, 0);
    analogWrite(MOTOR_BL1, L);
    analogWrite(MOTOR_BL2, 0);
  }
  else {
    analogWrite(MOTOR_FL1, 0);
    analogWrite(MOTOR_FL2, L * -1);
    analogWrite(MOTOR_BL1, 0);
    analogWrite(MOTOR_BL2, L * -1);
  }
  if (R >= 0) {
    analogWrite(MOTOR_FR1, R);
    analogWrite(MOTOR_FR2, 0);
    analogWrite(MOTOR_BR1, R);
    analogWrite(MOTOR_BR2, 0);
  }
  else {
    analogWrite(MOTOR_FR1, 0);
    analogWrite(MOTOR_FR2, R * -1);
    analogWrite(MOTOR_BR1, 0);
    analogWrite(MOTOR_BR2, R * -1);
  }
}
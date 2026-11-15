#include <dummy.h>
#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>


Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;

#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_PIN 33

int MOTOR_L1 = 13;
int MOTOR_L2 = 27;
int MOTOR_R1 = 4;
int MOTOR_R2 = 18;

int tacticSelection = 0;

// ==================== 追蹤參數 ====================
const int MAX_SPEED = 100;          // 測試階段最大速度
const int SENSOR_FLOOR = 200;       // 0~200 視為背景
const int SENSOR_MAX = 1300;        // 最大有效感測值
const int TARGET_THRESHOLD = 250;   // 超過此值視為看到敵人

const int SEARCH_SPEED = 60;        // 沒看到敵人時前進速度
const int TRACK_BASE_SPEED = 60;    // 追蹤時最低前進速度
const int MAX_STEER = 90;           // 最大轉向修正

float KP = 50.0;                    // 比例轉向係數

// ==================== 感測器 ====================
int IR_L, IR_ML, IR_M, IR_MR, IR_R, IR_B;
int LINE_L, LINE_R;

float targetError = 0;
float lastTargetError = 0;

unsigned long debugTimer = 0;

// ==================== 函式宣告 ====================
void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void stopMotor();
void executeTactic(int tactic);
int sensorSignal(int value);
void trackEnemy(int L, int ML, int M, int mr, int R);


void setup() {
  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS1.");
    while (1);
  }

  if (!ads2.begin(0x49)) {
    Serial.println("Failed to initialize ADS2.");
    while (1);
  }

  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }

  Serial.println("=== TACTIC SELECTION PHASE ===");
  Serial.println("Cover IR Sensor to select: Left = 1 | Middle = 0 | Right = 2");
  Serial.println("Press button when ready to LOCK IN.");

  while (digitalRead(BUTTON_PIN) == HIGH) {
    int leftVal = ads1.readADC_SingleEnded(0);
    int middleVal = ads1.readADC_SingleEnded(3);
    int rightVal = ads2.readADC_SingleEnded(0);

    if (leftVal >= 400 && tacticSelection != 1) {
      tacticSelection = 1;
      Serial.println("Selected Tactic -> 1 (LEFT FLANK)");
    }
    else if (rightVal >= 400 && tacticSelection != 2) {
      tacticSelection = 2;
      Serial.println("Selected Tactic -> 2 (RIGHT FLANK)");
    }
    else if (middleVal >= 400 && tacticSelection != 0) {
      tacticSelection = 0;
      Serial.println("Selected Tactic -> 0 (DIRECT CHARGE)");
    }

    delay(200);
  }

  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }

  Serial.println("------------------------------");
  Serial.print("TACTIC LOCKED IN: ");
  Serial.println(tacticSelection);
  Serial.println("Starting in 3 seconds...");

  delay(3000);
  executeTactic(tacticSelection);
}


void loop() {

  // ==================== 讀取感測器 ====================
  IR_L = ads1.readADC_SingleEnded(0);
  IR_ML = ads1.readADC_SingleEnded(1);
  IR_M = ads1.readADC_SingleEnded(3);
  IR_MR = ads2.readADC_SingleEnded(1);
  IR_R = ads2.readADC_SingleEnded(0) - 120;

  LINE_L = ads1.readADC_SingleEnded(2);
  LINE_R = ads2.readADC_SingleEnded(2);
  IR_B = ads2.readADC_SingleEnded(3);


  // ==================== 擂台邊界 ====================
  if (LINE_L >= 800 || LINE_R >= 800) {
    stopMotor();
    //delay(10);

    back(200);
    delay(500);

    //back(70);
    //delay(120);

    if (LINE_L >= 800 && LINE_R < 800) {
      turnLR(200, -200);
      delay(300);
    }
    else if (LINE_R >= 800 && LINE_L < 800) {
      turnLR(-200, 200);
      delay(300);
    }
    else {
      turnLR(200, -200);
      delay(300);
    }

    //return;
  }


  // ==================== 前方敵人判斷 ====================
  int maxFront = max(max(IR_L, IR_ML), max(IR_M, max(IR_MR, IR_R)));

  if (maxFront >= TARGET_THRESHOLD) {
    trackEnemy(IR_L, IR_ML, IR_M, IR_MR, IR_R);
  }
  else if (IR_B >= 700) {
    turnLR(150, -150);
    delay(100);
  }
  else {
    front(SEARCH_SPEED);
    targetError = 0;
  }


  // ==================== Serial 監控 ====================
  if (millis() - debugTimer >= 100) {
    debugTimer = millis();

    Serial.print("L:");
    Serial.print(IR_L);
    Serial.print(" ML:");
    Serial.print(IR_ML);
    Serial.print(" M:");
    Serial.print(IR_M);
    Serial.print(" MR:");
    Serial.print(IR_MR);
    Serial.print(" R:");
    Serial.print(IR_R);
    Serial.print(" | Error:");
    Serial.println(targetError, 2);
  }
}


// =====================================================
// 去除感測器背景值
// =====================================================
int sensorSignal(int value) {
  if (value <= SENSOR_FLOOR) return 0;

  value = constrain(value, SENSOR_FLOOR, SENSOR_MAX);
  return value - SENSOR_FLOOR;
}


// =====================================================
// 五感測器線性追蹤
// =====================================================
void trackEnemy(int rawL, int rawML, int rawM, int rawMR, int rawR) {

  float L = sensorSignal(rawL);
  float ML = sensorSignal(rawML);
  float M = sensorSignal(rawM);
  float mr = sensorSignal(rawMR);
  float R = sensorSignal(rawR);

  float total = L + ML + M + mr + R;

  if (total <= 0) {
    front(SEARCH_SPEED);
    return;
  }

  // 感測器位置權重：
  // L = -2、ML = -1、M = 0、MR = +1、R = +2
  targetError = (L * -2.0 + ML * -1.0 + M * 0.0 + mr * 1.0 + R * 2.0) / total;

  lastTargetError = targetError;


  // ==================== 比例轉向 ====================
  float steering = targetError * KP;
  steering = constrain(steering, -MAX_STEER, MAX_STEER);


  // ==================== 前進速度 ====================
  // 中間感測器越強，代表敵人越接近正前方，速度越快
  float middleStrength = M / (SENSOR_MAX - SENSOR_FLOOR);
  middleStrength = constrain(middleStrength, 0.0, 1.0);

  int baseSpeed = TRACK_BASE_SPEED + middleStrength * (MAX_SPEED - TRACK_BASE_SPEED);
  baseSpeed = constrain(baseSpeed, TRACK_BASE_SPEED, MAX_SPEED);


  // ==================== 左右差速 ====================
  float leftSpeed = baseSpeed + steering;
  float rightSpeed = baseSpeed - steering;


  // 如果任何一邊超過 120，兩邊同比例縮小
  float maxMotor = max(abs(leftSpeed), abs(rightSpeed));

  if (maxMotor > MAX_SPEED) {
    float scale = (float)MAX_SPEED / maxMotor;
    leftSpeed *= scale;
    rightSpeed *= scale;
  }

  leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

  turnLR((int)leftSpeed, (int)rightSpeed);


  // ==================== Debug ====================
  if (millis() - debugTimer >= 100) {
    Serial.print(" E:");
    Serial.print(targetError, 2);
    Serial.print(" Base:");
    Serial.print(baseSpeed);
    Serial.print(" Steer:");
    Serial.print(steering);
    Serial.print(" Motor L:");
    Serial.print(leftSpeed);
    Serial.print(" R:");
    Serial.println(rightSpeed);
  }
}


// =====================================================
// 開場戰術
// =====================================================
void executeTactic(int tactic) {

  switch (tactic) {

    case 0:
      front(100);
      delay(200);

      //turnLR(-80, 80);
      //delay(400);

      //front(100);
      //delay(200);
      break;


    case 1:
      turnLR(100, 120);
      delay(280);

      turnLR(-90, 90);
      delay(200);

      front(90);
      delay(200);
      break;


    case 2:
      turnLR(120, 100);
      delay(280);

      turnLR(90, -90);
      delay(250);

      front(90);
      delay(200);
      break;
  }
}


// =====================================================
// 前進
// =====================================================
void front(int spd) {
  spd = constrain(spd, 0, MAX_SPEED);

  analogWrite(MOTOR_R1, spd);
  analogWrite(MOTOR_R2, 0);
  analogWrite(MOTOR_L1, spd);
  analogWrite(MOTOR_L2, 0);
}


// =====================================================
// 後退
// =====================================================
void back(int spd) {
  spd = constrain(spd, 0, MAX_SPEED);

  analogWrite(MOTOR_R1, 0);
  analogWrite(MOTOR_R2, spd);
  analogWrite(MOTOR_L1, 0);
  analogWrite(MOTOR_L2, spd);
}


// =====================================================
// 馬達煞車
// =====================================================
void stopMotor() {
  analogWrite(MOTOR_R1, 255);
  analogWrite(MOTOR_R2, 255);
  analogWrite(MOTOR_L1, 255);
  analogWrite(MOTOR_L2, 255);
}


// =====================================================
// 左右馬達控制
//
// +120 = 正轉最大速度
//    0 = 停止
// -120 = 反轉最大速度
// =====================================================
void turnLR(int L, int R) {

  L = constrain(L, -MAX_SPEED, MAX_SPEED);
  R = constrain(R, -MAX_SPEED, MAX_SPEED);

  if (L >= 0) {
    analogWrite(MOTOR_L1, L);
    analogWrite(MOTOR_L2, 0);
  }
  else {
    analogWrite(MOTOR_L1, 0);
    analogWrite(MOTOR_L2, -L);
  }

  if (R >= 0) {
    analogWrite(MOTOR_R1, R);
    analogWrite(MOTOR_R2, 0);
  }
  else {
    analogWrite(MOTOR_R1, 0);
    analogWrite(MOTOR_R2, -R);
  }
}
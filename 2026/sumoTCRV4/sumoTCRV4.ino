#include <dummy.h>
#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#undef MR

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
// 正面鎖定敵人時的全速攻擊
const int ATTACK_SPEED = 255;

// 一般追蹤敵人時的直走速度
const int TRACK_SPEED = 90;

// 追蹤時最高馬達速度
const int TRACK_MAX_SPEED = 110;

// 沒看到敵人的搜尋速度
const int SEARCH_SPEED = 70;

// 線邊逃脫
const int LINE_BACK_SPEED = 200;
const int LINE_BACK_SLOW_SPEED = 150;
const int LINE_TURN_SPEED = 200;

// 後方敵人轉身
const int BACK_TURN_SPEED = 150;

// ==================== 追蹤參數 ====================
const int SENSOR_MAX = 1300;
const int MIDDLE_SENSOR_FLOOR = 300;
const int TARGET_THRESHOLD = 350;
const int SIDE_SENSOR_FLOOR = 400;

// 正面攻擊判斷
const int FRONT_ATTACK_THRESHOLD = 450;

// 誤差小於這個值，代表敵人基本在正前方
const float ATTACK_ERROR = 0.30;

// 線性轉向
float KP = 30.0;
const int MAX_STEER = 180;
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
void trackEnemy(int L, int ML, int M, int MR, int R);


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
    int backVal = ads2.readADC_SingleEnded(3);

    if (leftVal >= 800 && tacticSelection != 1) {
      tacticSelection = 1;
      Serial.println("Selected Tactic -> 1 (LEFT FLANK)");
    }
    else if (rightVal >= 800 && tacticSelection != 2) {
      tacticSelection = 2;
      Serial.println("Selected Tactic -> 2 (RIGHT FLANK)");
    }
    else if (middleVal >= 800 && tacticSelection != 0) {
      tacticSelection = 0;
      Serial.println("Selected Tactic -> 0 (DIRECT CHARGE)");
    }
    else if (backVal >= 800 && tacticSelection != 3) {
      tacticSelection = 3;
      Serial.println("Selected Tactic -> 3 (REVERSE CHARGE)")
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

  IR_L = ads1.readADC_SingleEnded(0);
  IR_ML = ads1.readADC_SingleEnded(1);
  IR_M = ads1.readADC_SingleEnded(3);
  IR_MR = ads2.readADC_SingleEnded(1);
  IR_R = ads2.readADC_SingleEnded(0) - 120;

  LINE_L = ads1.readADC_SingleEnded(2);
  LINE_R = ads2.readADC_SingleEnded(2);
  IR_B = ads2.readADC_SingleEnded(3);
  Serial.print(" LEFT:");
  Serial.print(LINE_L);
  Serial.print(" RIGHT:");
  Serial.print(LINE_R);

  // ==================== 擂台邊界 ====================
  if (LINE_L >=800|| LINE_R >=800) {
    stopMotor();
    delay(10);

    // 第一段快速後退，克服前進慣性
    back(LINE_BACK_SPEED);
    delay(200);

    // 第二段持續後退，確保真的離開黑線
    back(LINE_BACK_SLOW_SPEED);
    delay(100);

    if (LINE_L >= 800 && LINE_R >= 800) {
      turnLR(LINE_TURN_SPEED, -LINE_TURN_SPEED);
      delay(120);
    }
    else if (LINE_R >= 800 && LINE_L >= 800) {
      turnLR(-LINE_TURN_SPEED, LINE_TURN_SPEED);
      delay(120);
    }
    else {
      turnLR(LINE_TURN_SPEED, -LINE_TURN_SPEED);
      delay(120);
    }

    //return;
  }


  // ==================== 前方敵人 ====================
  //int maxFront = max(max(IR_L - 150, IR_ML), max(IR_M, max(IR_MR, IR_R - 150)));
  int maxFront = max( max(IR_M, IR_MR),IR_ML);
  if (maxFront >= TARGET_THRESHOLD) {

    trackEnemy(IR_L, IR_ML, IR_M, IR_MR, IR_R);
  }
  else if(IR_L >= 450){
    turnLR(-BACK_TURN_SPEED, BACK_TURN_SPEED);
  }
  else if(IR_R >=450){
    turnLR(BACK_TURN_SPEED, -BACK_TURN_SPEED);
  }

  // ==================== 後方敵人 ====================
  else if (IR_B >= 700) {
    turnLR(BACK_TURN_SPEED, -BACK_TURN_SPEED);
    delay(250);
  }

  // ==================== 沒看到敵人 ====================
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
int middleSensorSignal(int value) {
  if (value <= MIDDLE_SENSOR_FLOOR) return 0;

  value = constrain(value, MIDDLE_SENSOR_FLOOR, SENSOR_MAX);
  return value - MIDDLE_SENSOR_FLOOR;
}

int sideSensorSignal(int value) {
  if (value <= SIDE_SENSOR_FLOOR) return 0;

  value = constrain(value, SIDE_SENSOR_FLOOR, SENSOR_MAX);
  return value - SIDE_SENSOR_FLOOR;
}

// =====================================================
// 五感測器線性追蹤
// =====================================================
void trackEnemy(int rawL, int rawML, int rawM, int rawMR, int rawR) {
  float L = sideSensorSignal(rawL);
  float ML = middleSensorSignal(rawML);
  float M = middleSensorSignal(rawM);
  float MR = middleSensorSignal(rawMR);
  float R = sideSensorSignal(rawR);

  float total = L*0 + ML + M + MR + R*0;

  if (total <= 0) {
    front(SEARCH_SPEED);
    return;
  }

  // ==================== 計算敵人方向 ====================
  targetError = ((L * 0) + ML * -1.0 + M * 0.0 + MR * 1.0 + (R * 0)) / total;


  // =====================================================
  // 正面攻擊模式
  //
  // 1. 中間感測器數值夠高
  // 2. 敵人方向誤差夠小
  //
  // 只有這時候才全速
  // =====================================================
  if (rawM >= FRONT_ATTACK_THRESHOLD && abs(targetError) <= ATTACK_ERROR) {
    front(ATTACK_SPEED);
    return;
  }


  // ==================== 一般線性追蹤 ====================
  float steering = targetError * KP;
  steering = constrain(steering, -MAX_STEER, MAX_STEER);

  float leftSpeed = TRACK_SPEED + steering;
  float rightSpeed = TRACK_SPEED - steering;


  // ==================== 追蹤速度限制 ====================
  float maxMotor = max(abs(leftSpeed), abs(rightSpeed));

  if (maxMotor > TRACK_MAX_SPEED) {
    float scale = (float)TRACK_MAX_SPEED / maxMotor;
    leftSpeed *= scale;
    rightSpeed *= scale;
  }

  leftSpeed = constrain(leftSpeed, -TRACK_MAX_SPEED, TRACK_MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -TRACK_MAX_SPEED, TRACK_MAX_SPEED);

  turnLR((int)leftSpeed, (int)rightSpeed);
}


// =====================================================
// 開場戰術
// =====================================================
void executeTactic(int tactic) {

  switch (tactic) {

    case 0:
      front(200);
      delay(100);

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

    case 3:
      back(200);
      delay(200);
      stopMotor();
      delay(10);
      front(150);
      delay(150);
  }
}


// =====================================================
// 前進
// =====================================================
void front(int spd) {
  spd = constrain(spd, 0, 255);
  
 analogWrite(MOTOR_R1, spd);
  analogWrite(MOTOR_R2, 0);

  analogWrite(MOTOR_L1, spd);
  analogWrite(MOTOR_L2, 0);
}


// =====================================================
// 後退
// =====================================================
void back(int spd) {
  spd = constrain(spd, -255, 255);

  analogWrite(MOTOR_R1, 0);
  analogWrite(MOTOR_R2, (spd-20)<-254?-255:(spd-20));
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

  L = constrain(L, -255, 255);
  R = constrain(R, -255, 255);

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
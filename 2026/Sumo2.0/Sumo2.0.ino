#include <dummy.h>
#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads1;
Adafruit_ADS1015 ads2;

#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_PIN 33
#define P_in 0.5
#define P_out 0.7

int tacticSelection = 0;

int MOTOR_L1 = 13;
int MOTOR_L2 = 27;
int MOTOR_R1 = 4;
int MOTOR_R2 = 18;

float currentTime;

void front(int spd);
void back(int spd);
void turnLR(int L, int R);
void stop(void);
void ExecuteTactic(int tactic);

float LM(int, int, int, int);
float RM(int, int, int, int);
int IR_M, IR_MR, IR_ML, IR_L, IR_R, IR_B;


void setup() {
  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);
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
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10); // Wait until button is released
  }

  Serial.println("=== TACTIC SELECTION PHASE ===");
  Serial.println("Cover IR Sensor to select: Left = 1 | Middle = 0 | Right = 2");
  Serial.println("Press button when ready to LOCK IN.");

  // 2. SELECTION LOOP: Loop until button is pressed (goes LOW)
  while (digitalRead(BUTTON_PIN) == HIGH) {
    int leftVal   = ads1.readADC_SingleEnded(0); // Left IR (ads1 pin 0)
    int middleVal = ads1.readADC_SingleEnded(3); // Middle IR (ads1 pin 3)
    int rightVal  = ads2.readADC_SingleEnded(0); // Right IR (ads2 pin 0)

    // Read sensors to change tactic
    if (leftVal >= 400 && tacticSelection != 1) {
      tacticSelection = 1; // Left Flank
      Serial.println("Selected Tactic -> 1 (LEFT FLANK)");
    } 
    else if (rightVal >= 400 && tacticSelection != 2) {
      tacticSelection = 2; // Right Flank
      Serial.println("Selected Tactic -> 2 (RIGHT FLANK)");
    } 
    else if (middleVal >= 400 && tacticSelection != 0) {
      tacticSelection = 0; // Direct Charge
      Serial.println("Selected Tactic -> 0 (DIRECT CHARGE)");
    }

    delay(200); // Small sampling delay
  }

  // 3. WAIT FOR BUTTON RELEASE: Prevent accidental double-triggers
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }

  Serial.println("------------------------------");
  Serial.print("TACTIC LOCKED IN: ");
  Serial.println(tacticSelection);
  Serial.println("Starting in 3 seconds...");

  delay(3000); // 3-second mandatory delay before match starts

  // Execute opening maneuver
  executeTactic(tacticSelection);
}

void loop() {
  // int duration;
  // currentTime = millis();
  /*Serial.print("LIR: ");
  Serial.print(ads1.readADC_SingleEnded(0));
  Serial.print(" | MLIR: ");
  Serial.print(ads1.readADC_SingleEnded(1));
  Serial.print(" | MIR: ");
  Serial.print(ads1.readADC_SingleEnded(3));
  Serial.print(" | MRIR: ");
  Serial.print(ads2.readADC_SingleEnded(1));
  Serial.print(" | RIR: ");
  Serial.println(ads2.readADC_SingleEnded(0));
  Serial.print("LC: ");
  Serial.print(ads1.readADC_SingleEnded(2));
  Serial.print(" | RC: ");
  Serial.println(ads2.readADC_SingleEnded(2));
  Serial.print("BIR: ");
  Serial.println(ads2.readADC_SingleEnded(3));*/
  /*int IR_L = mapValue(ads1.readADC_SingleEnded(0), 70, 1042);
  int IR_ML = mapValue(ads1.readADC_SingleEnded(1), 15, 1035);
  int IR_M = mapValue(ads1.readADC_SingleEnded(3), 90, 1041);
  int IR_MR = mapValue(ads2.readADC_SingleEnded(1), 25, 1039);
  int IR_R = mapValue(ads2.readADC_SingleEnded(0), 50, 1042);*/

  if (ads1.readADC_SingleEnded(2) <= 100 || ads2.readADC_SingleEnded(2) <= 100) {
    //Serial.println("Line");
    stop();
    back(200);
    delay(220);
    back(80);
    delay(150);
    turnLR(100, -80);
    delay(180);
  }  else if (ads1.readADC_SingleEnded(3) >= 450) {
    Serial.println("Kill");
    front(255);
  } else if (ads1.readADC_SingleEnded(0) >= 400) { 
    //Serial.println("Left");
    turnLR(-70, 70);
  } else if (ads2.readADC_SingleEnded(0) >= 400) {
    //Serial.println("Right");
    turnLR(70, -70);
  } else if (ads1.readADC_SingleEnded(1) >= 400) { 
    //Serial.println("Left");
    turnLR(-90, 90);
  } else if (ads2.readADC_SingleEnded(1) >= 400) {
    //Serial.println("Right");
    turnLR(90, -90);
  } else if (ads2.readADC_SingleEnded(3) >= 700) {
    //Serial.println("Back");
    turnLR(100, 0);
    delay(500);
    turnLR(0, 100);
    delay(500);
  } else {
    //Serial.println("N/A");
    front(80);
  }
  /*Serial.print(LM(IR_L, IR_ML, IR_MR, IR_R));
  Serial.print(" || ");
  Serial.println(RM(IR_L, IR_ML, IR_MR, IR_R));*/
  //delay(500);
  // duration = millis() - currentTime;
  // Serial.println(duration);
}

/*int mapValue(int rawData, int min_input, int max_input) {
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
}*/

void executeTactic (int tactic) {
  switch (tactic) {
    case 0: // Direct chaarge
      front(200);
      delay(200);
      turnLR(-90, 90);
      delay(400);
      front(120);
      delay(200);
    case 1: // Left flank
      turnLR(120, 180);
      delay(280);
      turnLR(-120, 120);
      delay(200);
      front(100);
      delay(200);
    case 2: // Right flank
      turnLR(180, 120);
      delay(280);
      turnLR(120, -120);
      delay(250);
      front(100);
      delay(200);
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

void turnLR(int L, int R) {
  if (L >= 0) {
    analogWrite(MOTOR_L1, L);
    analogWrite(MOTOR_L2, 0);
  }
  else {
    analogWrite(MOTOR_L1, 0);
    analogWrite(MOTOR_L2, L * -1);
  }
  if (R >= 0) {
    analogWrite(MOTOR_R1, R);
    analogWrite(MOTOR_R2, 0);
  }
  else {
    analogWrite(MOTOR_R1, 0);
    analogWrite(MOTOR_R2, R * -1);
  }
}
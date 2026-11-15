void func_forward(int spd);
void func_reverse(int L, int R);
void func_left(int L, int R);
void func_right(int L, int R);
void func_stop(void);
void func_spin(int L, int R);
void func_ESCL(void);
void func_ESCR(void);
float IR(int pin);
float ultraSound(int trigP, int echoP); 
float duration, ultraDistance;
//float volts, IRDistance;
float FLW, FRW, LIR, RIR, CIR, BIR, LSO, RSO;

void setup() {
  Serial.begin(9600);
  pinMode(39, INPUT);  //FL wire (FLW)
  pinMode(36, INPUT);  //FR wire (FRW)
  pinMode(32, INPUT);  //left side IR (LIR)
  pinMode(33, INPUT);  //right side IR (RIR)
  pinMode(23, INPUT); //left sonar (LSO) //trig = input & echo = output
  pinMode(22, OUTPUT); //left sonar (LSO)
  pinMode(21, INPUT);  //right sonar (RSO)
  pinMode(5, OUTPUT);  //right sonar (RSO)
  pinMode(35, INPUT);  //centre IR (CIR)
  pinMode(34, INPUT);  //back IR (BIR)
  pinMode(27, OUTPUT); //left motor 
  pinMode(13, OUTPUT); //left motor
  pinMode(19, OUTPUT); //right motor 
  pinMode(18, OUTPUT); //right motor 
  delay(2700);
}

void loop() {
  FLW = analogRead(39);
  FRW = analogRead(36);
  LSO = ultraSound(22, 23);
  RSO = ultraSound(5, 21);
  LIR = analogRead(32);
  RIR = analogRead(33);
  CIR = analogRead(35);
  BIR = analogRead(34);
  /*Serial.print("FLW: ");
  Serial.print(FLW);
  Serial.print("|| FRW: ");
  Serial.println(FRW);
  delay(500);
  Serial.print("cm || LSO: ");
  Serial.print(LSO);
  Serial.print("cm || CIR: ");
  Serial.println(CIR);
  Serial.print("cm || RSO: ");
  Serial.print(RSO);
  Serial.print("cm || RIR: ");
  Serial.print(RIR);
  Serial.println("cm");
  Serial.print("BIR: "); 
  Serial.print(BIR);
  Serial.println("cm"); *///make this comment during actual fight, cuz use up cpu
  //delay(1000); //delay to lag for observing values
  if (FLW > 30|| FRW > 80) {
    Serial.println("wire"); 
    func_reverse(200, 200);
    delay(500);
    func_spin(200, 200);
    delay(500);
  }
  else {
    if (CIR >= 2500 && CIR < 4095) {
      Serial.println("Kill"); 
      func_forward(255);
    }
    else if (RSO > 0 && RSO <= 40) {
      Serial.println("Right");
      func_right(255, 80);
    }
    else if (LSO > 0 && LSO <= 40) {
      Serial.println("Left");
      func_left(80, 255);
    }
    else if (RIR >= 3800 && RIR < 4095) {
      Serial.println("ESC Right");
      func_ESCR();
    }
    else if (LIR >= 3800 && LIR < 4095) {
      Serial.println("ESC Left");
      func_ESCL();
    }
    else if (BIR >= 3800 && BIR < 4095) {
      Serial.println("ESC back");
      func_ESCB();
    }
    else {
      Serial.println("Forward");
      func_forward(150);
    }
  }
}

float ultraSound(int trigP, int echoP) {
   digitalWrite(trigP, LOW);
   delayMicroseconds(5);
   digitalWrite(trigP, HIGH);     
   delayMicroseconds(10);
   digitalWrite(trigP, LOW);     
   duration = pulseIn(echoP, HIGH);  
   ultraDistance = (duration/2) / 29.1;  
   return ultraDistance;
}

/*float IR(int pin) {
  volts = analogRead(pin)*0.0048828125; // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  IRDistance = 65*pow(volts, -1.10); // worked out from graph 65 = theoretical distance / (1/Volts)S 
  //delay(100);
  return IRDistance;
}*/

void func_ESCB() {
  func_spin(200,200);
  delay(750);
}

void func_ESCL() {
  func_reverse(50, 255);
  delay(1000);
}

void func_ESCR() {
  func_reverse(255, 50);
  delay(1000);
}

void func_forward(int spd) {
  analogWrite(18 , spd);
  analogWrite(19 , 0);
  analogWrite(27 , 0);
  analogWrite(13 , spd);
}

  void func_stop(void) {
  analogWrite(18 , 0);
  analogWrite(19 , 0);
  analogWrite(27 , 0);
  analogWrite(13 , 0);
}

void func_reverse(int L, int R) {
  analogWrite(18 , 0);
  analogWrite(19 , R);
  analogWrite(27 , L);
  analogWrite(13 , 0);
}

void func_right(int L, int R) {
  analogWrite(18 , R);
  analogWrite(19 , 0);
  analogWrite(27 , 0);
  analogWrite(13 , L);
}

void func_left(int L, int R) {
  analogWrite(18 , R);
  analogWrite(19 , 0);
  analogWrite(27 , 0);
  analogWrite(13 , L);
}

void func_spin(int L, int R) {
  analogWrite(18 , R);
  analogWrite(19 , 0);
  analogWrite(27 , L);
  analogWrite(13 , 0);
}
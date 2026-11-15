void func_left(double L, double R);
void func_forward(double L, double R);
void func_right(double L, double R);
void func_reverse(double L, double R);
float ultralSound(int trigP, int echoP);         //超音波函式宣告
int trigP_1 = 13,  echoP_1 = 12;                 //超音波1號Trig Pin、Echo Pin
int trigP_2 = 11,  echoP_2 = 10;                 //超音波2號Trig Pin、Echo Pin
int light_R1,light_R2;
int ultR,ultL;
float duration, distance;
 
void setup() {
  Serial.begin (9600);            //鮑率設定
  pinMode(trigP_1, OUTPUT);     
  pinMode(trigP_2, OUTPUT);   
  pinMode(echoP_1, INPUT);
  pinMode(echoP_2, INPUT);
  
}
void loop()
{
 
 //====這邊是超音波1號的程式=========
  Serial.print("UTL : ");  
  Serial.print( ultralSound(trigP_1, echoP_1));   //印出超音波1號偵測到的距離
  Serial.print("cm__");
  
  //====這邊是超音波2號的程式=========
  Serial.print("UTR : ");  
  Serial.print(ultralSound(trigP_2, echoP_2));    //印出超音波2號偵測到的距離
  Serial.print("cm");

  Serial.print("LTR : ");  
  Serial.print(analogRead(A3));    //印出顏感1號偵測到的距離
  Serial.print("___");
  Serial.print("LTL : ");  
  Serial.println(analogRead(A2));    //印出顏感2號偵測到的距離
 
  
   /*
  delay(250);     測試回傳訊號時，為方便觀看，所以故意延遲時間，
                    讓回傳列印變化慢一點。寫到機器時這一個一定要刪
                    除掉，否則將嚴重影響機器判斷速度。
                  */
  light_R1 = analogRead(A3);
  light_R2 = analogRead(A2);                
  ultL = ultralSound(trigP_1,  echoP_1);
  ultR = ultralSound(trigP_2,  echoP_2);
  if (light_R1 < 100 || light_R2  < 100){
  Serial.println("wire");
  func_reverse(255,255);
  delay(350);
  func_left(255,0);
  delay(350);
}
else {
  if (ultR < 25 && ultL < 25){
    Serial.println("F");
    func_forward(255,255);
  }
  else if (ultL < 25){
    Serial.println("L");
    func_right(50,255);
  }
  else if (ultR < 25){
    Serial.println("R");
    func_left(255,50);
  }
  else {
    Serial.println("NA");
    func_forward(255,255);
  }


}


}
/*
 * 「ultralSound」是超音波程式，下面是電訊號運作過程，先打訊號給感測器，再接收回傳訊號。
 * 程式會透過回傳的訊號持續時間來計算距離。
 */
float ultralSound(int trigP, int echoP){  
    digitalWrite(trigP, LOW);
    delayMicroseconds(5);
    digitalWrite(trigP, HIGH);     // 提供 Trig 高電位，持續 10us
    delayMicroseconds(10);
    digitalWrite(trigP, LOW);      // 拉低電位

    
    duration = pulseIn(echoP, HIGH);   // pulseIn是用來讀取收到高電位時的時間
    distance = (duration/2) / 29.1;    //計算距離公式
    return distance;
}
void func_left(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,1);
    
    digitalWrite(4,0);
    
    digitalWrite(5,0);
    
    digitalWrite(7,1);
    
}

void func_forward(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,1);
    
    digitalWrite(4,0);
    
    digitalWrite(5,1);
    
    digitalWrite(7,0);
    
}

void func_right(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,0);
    
    digitalWrite(4,1);
    
    digitalWrite(5,1);
    
    digitalWrite(7,0);
    
}

void func_reverse(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,0);
    
    digitalWrite(4,1);
    
    digitalWrite(5,0);
    
    digitalWrite(7,1);
    
}

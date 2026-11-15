void func_left(double L, double R);
void func_forward(double L, double R);
void func_right(double L, double R);
void func_reverse(double L, double R);
int light_Left,light_Right;
int IRR,IRL,IRC;
float duration, distance;
 
void setup() {
  Serial.begin (9600);            //鮑率設定
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
}
void loop()
{
 
 //====這邊是紅外線Left的程式=========
  Serial.print("IRL : ");  
  Serial.print(digitalRead(11));   //印出紅外線Left偵測到的距離
  Serial.print(" __ ");
  
  //====這邊是紅外線Centre的程式=========
  Serial.print("IRC : ");  
  Serial.print(digitalRead(12));   //印出紅外線Centre偵測到的距離
  Serial.print(" __ ");

  //====這邊是紅外線Right的程式=========
  Serial.print("IRR : ");  
  Serial.print(digitalRead(13));    //印出紅外線Right偵測到的距離
  Serial.print(" // ");

  Serial.print("LTR : ");  
  Serial.print(analogRead(A2));    //印出顏感Right偵測到的距離
  Serial.print(" __ ");
  Serial.print("LTL : ");  
  Serial.println(analogRead(A3));    //印出顏感Left偵測到的距離
 
  /*
   
  delay(250);     測試回傳訊號時，為方便觀看，所以故意延遲時間，
                    讓回傳列印變化慢一點。寫到機器時這一個一定要刪
                    除掉，否則將嚴重影響機器判斷速度。
                  */
  light_Left = analogRead(A3);
  light_Right = analogRead(A2);                
  IRL = digitalRead(11);
  IRC = digitalRead(12);
  IRR = digitalRead(13);
  if (light_Left < 500 || light_Right  < 500 ){
  Serial.println("wire");
  func_reverse(200,200);
  delay(350);
  func_left(200,0);
  delay(350);
}
else {
  if (IRR == 1 && IRL == 1 && IRC == 1){
    Serial.println("Forward");
    func_forward(255,255);
  }
  else if (IRR == 1 && IRL == 1){
    Serial.println("NA");
    func_forward(255,255);
  }
  else if (IRL == 1 ||IRL == 1 && IRC == 1){
    Serial.println("Left");
    func_left(0,200);
  }
  else if (IRR == 1 || IRR == 1 && IRC == 1){
    Serial.println("Right");
    func_right(200,0);
  }
  else if (IRC == 1){
    Serial.println("Forward");
    func_forward(255,255);
  }
  else {
    Serial.println("NA");
    func_forward(255,255);
  }


}


}


void func_left(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(5,0);
    
    analogWrite(6,0);
    
    analogWrite(9,R);
    
}

void func_forward(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(5,0);
    
    analogWrite(6,0);
    
    analogWrite(9,R);
    
}

void func_right(double L, double R)
{
    analogWrite(3,0);
    
    analogWrite(5,R);
    
    analogWrite(6,L);
    
    analogWrite(9,0);
    
}

void func_reverse(double L, double R)
{
    analogWrite(3,0);
    
    analogWrite(5,R);
  
    analogWrite(6,L);
    
    analogWrite(9,0);
    
}

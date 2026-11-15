#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
void func_left(double L, double R);
void func_forward(double L, double R);
void func_right(double L, double R);
void func_reverse(double L, double R);
double left_sonar;
double right_sonar;
double currentTime = 0;
double lastTime = 0;
double getLastTime(){
    	return currentTime = millis()/1000.0 - lastTime;
}
int ult_R ,ult_L,light_R1 ,light_R2 ;
float getDistance(int trig,int echo){
    pinMode(trig,OUTPUT);
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
    pinMode(echo, INPUT);
    return pulseIn(echo,HIGH,30000)/58.0;
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
void setup{
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

}
void loop(){
ult_R = ultralSound(trigP_1, echoP_1); 
ult_L = ultralSound(trigP_2, echoP_2);
light_R1 = analogRead(light1P);
light_R2 = analogRead(light2P);

if (light_R1 > 40 || light_R2 > 40){
  func_reverse(255,255);
  delay(350);
  func_left(255,0);
  delay(350);
}
else {
  if (ult1 < 25 && ult2 < 25){
    func_forward(255,255);
  }
  else if (ultR < 25){
    func_right(50,255);
  }
  else if (ultL < 25){
    func_left(255,50);
  }
  else {
    func_forward(255,255);
  }
}
}
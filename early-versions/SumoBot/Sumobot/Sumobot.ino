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


void setup(){
    delay(50);
    pinMode(3,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(A0+3,INPUT);
    pinMode(A0+2,INPUT);
    digitalWrite(2,1);
    digitalWrite(4,1);
    digitalWrite(5,1);
    digitalWrite(7,1);
    analogWrite(3,255);
    analogWrite(6,255);
    delay(240);
    
}

void loop(){
    
    lastTime = millis()/1000.0;
    if((10000) > (getLastTime())){
        while(!(((analogRead(A0+3)) < (40)) || ((analogRead(A0+2)) < (40))))
        {
            loop();
            left_sonar = 0;
            right_sonar = 0;
            if(((1) < (getDistance(11,10))) && ((getDistance(11,10)) < (60))){
                left_sonar = 1;
            }else{
                left_sonar = 0;
            }
            if(((1) < (getDistance(13,12))) && ((getDistance(13,12)) < (60))){
                right_sonar = 1;
            }else{
                right_sonar = 0;
            }
        }
        if(((((left_sonar)==(0))) && (((right_sonar)==(0)))) || ((((left_sonar)==(1))) && (((right_sonar)==(1))))){
            func_forward(255,255);
        }else{
            if((((left_sonar)==(1))) && (((right_sonar)==(0)))){
                func_forward(255,50);
            }else{
                func_forward(50,255);
            }
        }
    }else{
        while(!((((analogRead(A0+3)) < (40)) || ((analogRead(A0+2)) < (40))) || ((5000) < (getLastTime()))))
        {
            loop();
            left_sonar = 0;
            right_sonar = 0;
            if(((1) < (getDistance(11,10))) && ((getDistance(11,10)) < (60))){
                left_sonar = 1;
            }else{
                left_sonar = 0;
            }
            if(((1) < (getDistance(13,12))) && ((getDistance(13,12)) < (60))){
                right_sonar = 1;
            }else{
                right_sonar = 0;
            }
            if(((((left_sonar)==(0))) && (((right_sonar)==(0)))) || ((((left_sonar)==(1))) && (((right_sonar)==(1))))){
                func_forward(255,255);
            }else{
                if((((left_sonar)==(1))) && (((right_sonar)==(0)))){
                    func_forward(255,50);
                }else{
                    func_forward(50,255);
                }
            }
        }
    }
    func_reverse(255,255);
    delay(38);
    func_right(255,255);
    delay(20);
    lastTime = millis()/1000.0;
    while(!(((((1) < (getDistance(11,10))) && ((getDistance(11,10)) < (60))) || (((1) < (getDistance(13,12))) && ((getDistance(13,12)) < (60)))) || ((6000) < (getLastTime()))))
    {
        loop();
        func_right(255,255);
    }
    
    loop();
}


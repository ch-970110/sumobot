#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>


double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
void __func__24038_36681(double L, double R);
void __func__21069_36914(double L, double R);
void __func__24460_36864(double L, double R);
void __func__21491_36681(double L, double R);
double __var__49_50_51;
double __var__50_51_52;
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


void __func__24038_36681(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,1);
    
    digitalWrite(4,0);
    
    digitalWrite(5,0);
    
    digitalWrite(7,1);
    
}

void __func__21069_36914(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,1);
    
    digitalWrite(4,0);
    
    digitalWrite(5,1);
    
    digitalWrite(7,0);
    
}

void __func__24460_36864(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,0);
    
    digitalWrite(4,1);
    
    digitalWrite(5,0);
    
    digitalWrite(7,1);
    
}

void __func__21491_36681(double L, double R)
{
    analogWrite(3,L);
    
    analogWrite(6,R);
    
    digitalWrite(2,0);
    
    digitalWrite(4,1);
    
    digitalWrite(5,1);
    
    digitalWrite(7,0);
    
}


void setup(){
    _delay(0.5);
    __var__49_50_51 = 0;
    __var__50_51_52 = 0;
    pinMode(3,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(A0+3,INPUT);
    digitalWrite(2,1);
    digitalWrite(4,1);
    digitalWrite(5,1);
    digitalWrite(7,1);
    analogWrite(3,255);
    analogWrite(6,255);
    _delay(2.45);
    
}

void loop(){
    
    lastTime = millis()/1000.0;
    if((10000) < (getLastTime())){
        while(!((analogRead(A0+3)) < (40)))
        {
            _loop();
            if((getDistance(11,10)) < (60)){
                __var__49_50_51 = 1;
            }else{
                __var__49_50_51 = 0;
            }
            if((getDistance(13,12)) < (60)){
                __var__50_51_52 = 1;
            }else{
                __var__50_51_52 = 0;
            }
        }
        if(((((__var__49_50_51)==(0))) && (((__var__50_51_52)==(0)))) || ((((__var__49_50_51)==(1))) && (((__var__50_51_52)==(1))))){
            __func__21069_36914(255,255);
        }else{
            if((((__var__49_50_51)==(1))) && (((__var__50_51_52)==(0)))){
                __func__21069_36914(255,200);
            }else{
                __func__21069_36914(200,255);
            }
        }
    }else{
        while(!(((analogRead(A0+3)) < (40)) || ((5000) < (getLastTime()))))
        {
            _loop();
            if((getDistance(11,10)) < (55)){
                __var__49_50_51 = 1;
            }else{
                __var__49_50_51 = 0;
            }
            if((getDistance(13,12)) < (55)){
                __var__50_51_52 = 1;
            }else{
                __var__50_51_52 = 0;
            }
            if(((((__var__49_50_51)==(0))) && (((__var__50_51_52)==(0)))) || ((((__var__49_50_51)==(1))) && (((__var__50_51_52)==(1))))){
                __func__21069_36914(255,255);
            }else{
                if((((__var__49_50_51)==(1))) && (((__var__50_51_52)==(0)))){
                    __func__21069_36914(255,35);
                }else{
                    __func__21069_36914(35,255);
                }
            }
        }
    }
    __func__24460_36864(255,255);
    _delay(0.38);
    __func__21491_36681(255,255);
    _delay(0.2);
    lastTime = millis()/1000.0;
    while(!((((getDistance(11,10)) < (50)) || ((getDistance(13,12)) < (50))) || ((6000) < (getLastTime()))))
    {
        _loop();
        __func__21491_36681(255,255);
    }
    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    
}

